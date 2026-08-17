package amiga;

import docking.ActionContext;
import docking.action.DockingAction;
import docking.action.MenuData;
import ghidra.app.plugin.PluginCategoryNames;
import ghidra.app.plugin.ProgramPlugin;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.framework.plugintool.PluginInfo;
import ghidra.framework.plugintool.PluginTool;
import ghidra.framework.plugintool.util.PluginStatus;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Bookmark;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Program;
import ghidra.util.task.TaskMonitor;

/** Provides modeless review and promotion of Address-Taken Code candidates. */
//@formatter:off
@PluginInfo(
		status = PluginStatus.RELEASED,
		packageName = "Amiga",
		category = PluginCategoryNames.ANALYSIS,
		shortDescription = "Reviews address-taken 68000 code candidates",
		description = "Displays address-taken code candidates in a modeless review table."
)
//@formatter:on
public class AddressTakenCodeReviewPlugin extends ProgramPlugin {

	private AddressTakenCodeReviewProvider provider;

	public AddressTakenCodeReviewPlugin(PluginTool tool) {
		super(tool);
	}

	@Override
	protected void init() {
		provider = new AddressTakenCodeReviewProvider(this);
		DockingAction showReview = new DockingAction("Address-Taken Code Review", getName()) {
			@Override
			public void actionPerformed(ActionContext context) {
				provider.showReview();
			}

			@Override
			public boolean isEnabledForContext(ActionContext context) {
				return currentProgram != null;
			}
		};
		showReview.setMenuBarData(new MenuData(new String[] { "Analysis", "Address-Taken Code Review" }));
		tool.addAction(showReview);
	}

	@Override
	protected void programActivated(Program program) {
		super.programActivated(program);
		provider.setProgram(program);
	}

	@Override
	protected void programClosed(Program program) {
		provider.setProgram(null);
		super.programClosed(program);
	}

	@Override
	public void dispose() {
		provider.dispose();
		super.dispose();
	}

	void goToCandidate(Address address) {
		goTo(address);
	}

	boolean promote(Bookmark bookmark) {
		Address address = bookmark.getAddress();
		int transaction = currentProgram.startTransaction("Promote address-taken code candidate");
		boolean commit = false;
		try {
			DisassembleCommand disassemble = new DisassembleCommand(address, null, true);
			if (!disassemble.applyTo(currentProgram, TaskMonitor.DUMMY)) {
				return false;
			}
			CreateFunctionCmd create = new CreateFunctionCmd(address);
			if (!create.applyTo(currentProgram, TaskMonitor.DUMMY) || create.getFunction() == null) {
				return false;
			}
			Function function = create.getFunction();
			if (function.getComment() == null || function.getComment().isBlank()) {
				function.setComment("Address-taken code promoted after analyst review. " + bookmark.getComment());
			}
			currentProgram.getBookmarkManager().removeBookmark(bookmark);
			commit = true;
			return true;
		} finally {
			currentProgram.endTransaction(transaction, commit);
		}
	}

	void dismiss(Bookmark bookmark) {
		currentProgram.getBookmarkManager().removeBookmark(bookmark);
	}
}
