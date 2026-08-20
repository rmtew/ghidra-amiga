package amiga;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertEquals;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

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
	public void exposesStandardAudioIoRequestType() throws Exception {
		try (FileDataTypeManager archive = FileDataTypeManager.openFileArchive(new File("data/amiga_ndk39.gdt"), true)) {
			List<DataType> matches = new ArrayList<>();
			archive.findDataTypes("IOAudio", matches);
			assertEquals(1, matches.size());
			assertEquals("IOAudio", matches.get(0).getName());
			assertEquals(0x44, matches.get(0).getLength());
		}
	}

	@Test
	public void headerManifestIncludesEveryVendoredDeclarationHeader() throws Exception {
		Path includeRoot = Path.of("3rdparty", "NDK_3.9", "include_h");
		List<String> manifest = Files.readAllLines(Path.of("data", "ndk39_headers.txt")).stream().map(String::strip)
				.filter(value -> !value.isEmpty() && !value.startsWith("#")).toList();
		Set<String> expected;
		try (var paths = Files.walk(includeRoot)) {
			expected = paths.filter(Files::isRegularFile).map(includeRoot::relativize).map(Path::toString)
					.map(value -> value.replace(File.separatorChar, '/'))
					.filter(value -> value.endsWith(".h") && !isForwardingHeader(value))
					.collect(Collectors.toCollection(LinkedHashSet::new));
		}
		assertEquals(expected, new LinkedHashSet<>(manifest));
		assertEquals(expected.size(), manifest.size());
	}

	private static boolean isForwardingHeader(String header) {
		return header.startsWith("clib/") || header.startsWith("proto/") || header.startsWith("pragma/") ||
				header.startsWith("pragmas/");
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
