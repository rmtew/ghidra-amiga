package amiga;

import java.util.HashSet;
import java.util.Locale;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.services.AbstractAnalyzer;
import ghidra.app.services.AnalysisPriority;
import ghidra.app.services.AnalyzerType;
import ghidra.app.util.importer.MessageLog;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.BookmarkManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.Reference;
import ghidra.util.exception.CancelledException;
import ghidra.util.task.TaskMonitor;

/**
 * Finds high-confidence 68000 functions whose address is taken as data.
 *
 * <p>Address-taken functions are normally missed by recursive-descent analysis
 * because their callers are indirect. This analyser deliberately promotes only
 * a narrow, auditable subset: a PC-visible data reference from executable code
 * to executable bytes whose decoded control flow reaches a 68000 return.
 * It makes no claim about the table or global which stores
 * the address, nor about any eventual indirect caller.  A stored address is
 * not by itself proof of code: the target must be executable, non-textual,
 * and have a reachable 68000 return when pseudo-disassembled.</p>
 */
public class AddressTakenCodeAnalyzer extends AbstractAnalyzer {

	private static final int MAX_REVIEW_MESSAGES = 5;
	private static final int MAX_ASCII_STRING_BYTES = 128;
	static final String BOOKMARK_TYPE = "Analysis";
	static final String BOOKMARK_CATEGORY = "Address-Taken Code Candidate";
	private static final Pattern PC_RELATIVE_LEA = Pattern.compile("^LEA.*\\,PC\\),A([0-7])$");
	private static final Pattern ADDRESS_REGISTER_STORE = Pattern.compile("^MOVE\\.LA([0-7]),\\(.*\\)$");

	public AddressTakenCodeAnalyzer() {
		super("Address-Taken Code Discovery",
				"Finds high-confidence 68000 functions referenced only through data pointers",
				AnalyzerType.INSTRUCTION_ANALYZER);
		// The evidence is a DATA reference on the PC-relative LEA. Run after the
		// reference analyzers have established those references during initial
		// auto-analysis, not merely after functions have been created.
		setPriority(AnalysisPriority.REFERENCE_ANALYSIS.after());
		setSupportsOneTimeAnalysis();
	}

	@Override
	public boolean getDefaultEnablement(Program program) {
		return isAmigaHunkExecutable(program.getExecutableFormat());
	}

	@Override
	public boolean canAnalyze(Program program) {
		return is68000Language(program.getLanguageID().getIdAsString());
	}

	@Override
	public boolean added(Program program, AddressSetView set, TaskMonitor monitor, MessageLog log)
			throws CancelledException {
		Listing listing = program.getListing();
		BookmarkManager bookmarks = program.getBookmarkManager();
		Set<Address> examinedTargets = new HashSet<>();
		int executableDataReferences = 0;
		int promoted = 0;
		int alreadyFunctions = 0;
		int skippedText = 0;
		int needsReview = 0;

		InstructionIterator instructions = listing.getInstructions(set, true);
		while (instructions.hasNext()) {
			monitor.checkCancelled();
			Instruction instruction = instructions.next();
			Instruction store = instruction.getNext();
			if (!isAddressInstalledIntoMemory(instruction, store)) {
				continue;
			}
			for (Reference reference : instruction.getReferencesFrom()) {
				if (!RefType.DATA.equals(reference.getReferenceType())) {
					continue;
				}
				Address target = reference.getToAddress();
				if (!isExecutable(program, target) || !examinedTargets.add(target)) {
					continue;
				}
				executableDataReferences++;
				if (program.getFunctionManager().getFunctionAt(target) != null) {
					removeReviewBookmark(bookmarks, target);
					alreadyFunctions++;
					continue;
				}
				if (isTextTarget(program, target)) {
					removeReviewBookmark(bookmarks, target);
					skippedText++;
					continue;
				}
				if (!M68kControlFlow.hasReachableReturn(program, target, monitor)) {
					needsReview++;
					bookmarks.setBookmark(target, BOOKMARK_TYPE, BOOKMARK_CATEGORY,
							"Address stored from " + instruction.getAddress() +
							"; executable target did not decode to a body with a reachable 68000 return. " +
							"Review before creating a function.");
					if (needsReview <= MAX_REVIEW_MESSAGES) {
						log.appendMsg(String.format(
							"Address-taken code candidate at %s from %s was not promoted: no reachable 68000 return was decoded.",
							target, instruction.getAddress()));
					}
					continue;
				}
				if (createFunction(program, target, instruction.getAddress(), monitor, log)) {
					removeReviewBookmark(bookmarks, target);
					promoted++;
				}
				else {
					needsReview++;
					bookmarks.setBookmark(target, BOOKMARK_TYPE, BOOKMARK_CATEGORY,
							"Address stored from " + instruction.getAddress() +
							"; executable target looked like a returned routine but Ghidra could not create a function. " +
							"Review before retrying.");
				}
			}
		}
		if (needsReview != 0) {
			log.appendMsg(String.format(
					"Address-Taken Code Discovery: examined %d stored executable-address targets; already functions %d; " +
					"promoted %d; skipped text %d; needs review %d.",
					executableDataReferences, alreadyFunctions, promoted, skippedText, needsReview));
		}
		return true;
	}

	static boolean is68000Language(String languageId) {
		return languageId != null && languageId.startsWith("68000:");
	}

	/**
	 * Address-taken code is common in Hunk programs, particularly where overlay
	 * and compiler linkage leave function pointers in writable data. Enable the
	 * conservative discovery pass for this loader's imports only; other 68000
	 * inputs retain Ghidra's opt-in default.
	 */
	static boolean isAmigaHunkExecutable(String executableFormat) {
		return AmigaHunkLoader.EXECUTABLE_FORMAT.equals(executableFormat);
	}

	/**
	 * Returns true only for {@code lea target(PC),An; move.l An,(memory)}. The
	 * store is the evidence that the address escapes into a pointer-bearing
	 * location; a bare LEA can be an ordinary local control/data reference.
	 */
	static boolean isAddressInstalledIntoMemory(Instruction lea, Instruction store) {
		if (lea == null || store == null || !lea.getAddress().getAddressSpace().equals(store.getAddress().getAddressSpace())) {
			return false;
		}
		return isAddressInstalledIntoMemory(lea.toString(), store.toString());
	}

	static boolean isAddressInstalledIntoMemory(String leaText, String storeText) {
		if (leaText == null || storeText == null) {
			return false;
		}
		Matcher addressLoad = PC_RELATIVE_LEA.matcher(normalizeInstruction(leaText));
		Matcher addressStore = ADDRESS_REGISTER_STORE.matcher(normalizeInstruction(storeText));
		return addressLoad.matches() && addressStore.matches() && addressLoad.group(1).equals(addressStore.group(1));
	}

	private static String normalizeInstruction(String instruction) {
		return instruction.replace(" ", "").toUpperCase(Locale.ROOT);
	}

	private static boolean isExecutable(Program program, Address address) {
		MemoryBlock block = program.getMemory().getBlock(address);
		return block != null && block.isExecute();
	}

	private static void removeReviewBookmark(BookmarkManager bookmarks, Address target) throws CancelledException {
		bookmarks.removeBookmarks(new AddressSet(target, target), BOOKMARK_TYPE, BOOKMARK_CATEGORY,
				TaskMonitor.DUMMY);
	}

	private static boolean isTextTarget(Program program, Address target) {
		Data data = program.getListing().getDefinedDataAt(target);
		if (data != null && data.hasStringValue()) {
			return true;
		}
		byte[] bytes = new byte[MAX_ASCII_STRING_BYTES];
		try {
			int length = program.getMemory().getBytes(target, bytes);
			return isPrintableNulTerminatedAscii(bytes, length);
		}
		catch (MemoryAccessException exception) {
			return false;
		}
	}

	/** Identifies ordinary NUL-terminated text even if it has not been typed as a string. */
	static boolean isPrintableNulTerminatedAscii(byte[] bytes, int length) {
		int boundedLength = Math.min(bytes.length, length);
		for (int index = 0; index < boundedLength; index++) {
			int value = bytes[index] & 0xff;
			if (value == 0) {
				return index >= 3;
			}
			if (value < 0x20 || value > 0x7e) {
				return false;
			}
		}
		return false;
	}

	private static boolean createFunction(Program program, Address target, Address provenance, TaskMonitor monitor,
			MessageLog log) throws CancelledException {
		DisassembleCommand disassemble = new DisassembleCommand(target, null, true);
		if (!disassemble.applyTo(program, monitor)) {
			log.appendMsg("Address-taken code candidate at " + target + " could not be disassembled.");
			return false;
		}
		CreateFunctionCmd createFunction = new CreateFunctionCmd(target);
		if (!createFunction.applyTo(program, monitor)) {
			log.appendMsg("Address-taken code candidate at " + target + " could not be created as a function.");
			return false;
		}
		Function function = createFunction.getFunction();
		if (function == null) {
			return false;
		}
		if (function.getComment() == null || function.getComment().isBlank()) {
			function.setComment("Address-taken code: referenced as data from " + provenance +
					". Target decoded to a body with a reachable 68000 return; no indirect caller is inferred.");
		}
		return true;
	}
}
