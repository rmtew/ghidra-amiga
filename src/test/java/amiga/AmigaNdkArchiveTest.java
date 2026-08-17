package amiga;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertEquals;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.junit.Test;
import org.junit.BeforeClass;

import ghidra.GhidraApplicationLayout;
import ghidra.framework.Application;
import ghidra.framework.ApplicationConfiguration;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataTypePath;
import ghidra.program.model.data.FileDataTypeManager;
import fd.FdParser;

public class AmigaNdkArchiveTest {

	@BeforeClass
	public static void initializeGhidra() throws Exception {
		if (!Application.isInitialized()) {
			Application.initializeApplication(
					new GhidraApplicationLayout(new File(System.getProperty("ghidra.install.dir"))),
					new ApplicationConfiguration());
		}
	}

	@Test
	public void exposesRastPortAndSupportsItsCanonicalLookup() throws Exception {
		try (FileDataTypeManager archive = FileDataTypeManager.openFileArchive(new File("data/amiga_ndk39.gdt"), true)) {
			assertNotNull(archive.getDataType(new DataTypePath("/clip.h", "RastPort")));

			List<DataType> matches = new ArrayList<>();
			archive.findDataTypes("RastPort", matches);
			assertEquals("RastPort", matches.get(0).getName());
		}
	}

	@Test
	public void resolvesEveryGraphicsRastPortSignature() throws Exception {
		var functions = FdParser.readSfdFile("data/sfd/graphics_lib.sfd").getFunctions();
		try (FileDataTypeManager archive = FileDataTypeManager.openFileArchive(new File("data/amiga_ndk39.gdt"), true)) {
			assertNotNull(AmigaHunkAnalyzer.getAmigaDataType("struct RastPort\t*", archive));
			for (var function : functions) {
				if (function.getReturnType().contains("RastPort")) {
					assertNotNull(AmigaHunkAnalyzer.getAmigaDataType(function.getReturnType(), archive));
				}
				for (var arg : function.getArgs()) {
					if (arg.type.contains("RastPort")) {
						assertNotNull(AmigaHunkAnalyzer.getAmigaDataType(arg.type, archive));
					}
				}
			}
		}
	}
}
