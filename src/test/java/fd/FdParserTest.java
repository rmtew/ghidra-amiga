package fd;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;

import org.junit.Test;

import fd.FdParser;

public class FdParserTest {
	@Test
	public void testExecFd() {
		var funcTable = FdParser.readFdFile("data/fd/exec_lib.fd");
		assertEquals(147, funcTable.getFunctions().length);
	}

	@Test
	public void testExecSfd() {
		var funcTable = FdParser.readSfdFile("data/sfd/exec_lib.sfd");
		assertEquals(125, funcTable.getFunctions().length);
		var func = funcTable.getFunctionByName("AVL_FindNextNodeByKey");
		assertNotNull(func);
		assertEquals("exec_library", func.getLib());
		assertEquals(-0x37e, func.getBias());
		assertEquals(3, func.getArgs().size());

		assertEquals("root", func.getArgs().get(0).name);
		assertEquals("CONST struct AVLNode *", func.getArgs().get(0).type);
		assertEquals("a0", func.getArgs().get(0).reg);

		assertEquals("key", func.getArgs().get(1).name);
		assertEquals("APTR", func.getArgs().get(1).type);
		assertEquals("a1", func.getArgs().get(1).reg);

		assertEquals("func", func.getArgs().get(2).name);
		assertEquals("APTR", func.getArgs().get(2).type);
		assertEquals("a2", func.getArgs().get(2).reg);
	}

	@Test
	public void testAllSfd() {
		File dir = new File("data/sfd");
		for(var entry : dir.listFiles()) {
			var funcTable = FdParser.readSfdFile(entry.getPath());
			assertNotNull(entry.getPath(), funcTable);
		}
	}

	@Test
	public void preservesRastPortPointerSyntax() {
		var funcTable = FdParser.readSfdFile("data/sfd/graphics_lib.sfd");
		var function = funcTable.getFunctionByName("BltTemplate");
		assertNotNull(function);
		assertEquals("struct RastPort *", function.getArgs().get(3).type);
		for (File entry : new File("data/sfd").listFiles()) {
			funcTable = FdParser.readSfdFile(entry.getPath());
			assertNotNull(entry.getPath(), funcTable);
			for (var candidate : funcTable.getFunctions()) {
				assertSeparatedPointerTokens(candidate.getName(false), candidate.getReturnType());
				for (var arg : candidate.getArgs()) {
					if (arg.type.contains("RastPort")) {
						assertSeparatedPointerTokens(candidate.getName(false), arg.type);
					}
				}
			}
		}
	}

	@Test
	public void parsesCommentedSfdWithLowercaseLibraryKeyAndAttachedPointerName() throws Exception {
		Path sfd = Files.createTempFile("Picasso96_card", ".sfd");
		try {
			Files.writeString(sfd, String.join("\n",
					"* fd2sfd comment",
					"==libname Picasso96_card",
					"==bias 30",
					"==public",
					"BOOL FindCard(struct BoardInfo *bi, APTR *tooltypes) (a0,a1)",
					"==end"));
			var table = FdParser.readSfdFile(sfd.toString());
			assertNotNull(table);
			var function = table.getFunctionByName("FindCard");
			assertNotNull(function);
			assertEquals("picasso96_card", function.getLib());
			assertEquals("struct BoardInfo *", function.getArgs().get(0).type);
			assertEquals("bi", function.getArgs().get(0).name);
			assertEquals("a0", function.getArgs().get(0).reg);
			assertEquals("APTR *", function.getArgs().get(1).type);
			assertEquals("tooltypes", function.getArgs().get(1).name);
			assertEquals("a1", function.getArgs().get(1).reg);
		}
		finally {
			Files.deleteIfExists(sfd);
		}
	}

	@Test
	public void loadsImportedPicasso96MmuLibAndOpenPciDefinitions() {
		assertNotNull(FdParser.readFdFile("data/fd/Picasso96API_lib.fd"));
		assertNotNull(FdParser.readFdFile("data/fd/mmu_lib.fd"));
		assertNotNull(FdParser.readFdFile("data/fd/mmu_resource.fd"));
		var openPci = FdParser.readSfdFile("data/sfd/openpci.sfd");
		assertNotNull(openPci);
		assertEquals("openpci_library", openPci.getFunctionByName("FindBoardA").getLib());
		var picassoCard = FdParser.readSfdFile("data/sfd/Picasso96_card.sfd");
		assertNotNull(picassoCard);
		assertEquals("a1", picassoCard.getFunctionByName("FindCard").getArgs().get(1).reg);
	}

	private static void assertSeparatedPointerTokens(String function, String type) {
		if (!type.contains("RastPort")) {
			return;
		}
		assertEquals("unexpected whitespace in " + function, type.trim(), type.replaceAll("\\s+", " ").trim());
		for (String token : type.split(" ")) {
			assertTrue("pointer token was not separated in " + function, token.equals("*") || !token.contains("*"));
		}
	}
}
