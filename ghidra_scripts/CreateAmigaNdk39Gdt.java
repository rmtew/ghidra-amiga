// Builds the NDK 3.9 data-type archive from the vendored headers.
//
// Usage (headless or GUI script runner):
//   CreateAmigaNdk39Gdt.java <repository-root> <output-gdt-path>
//
// The header manifest is deliberately complete for declaration headers and
// excludes only clib/, proto/, pragma/, and pragmas/ forwarding declarations.
// It fails rather than silently omitting a newly-vendored declaration header.
//
//@category Data Types

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

import ghidra.app.script.GhidraScript;
import ghidra.app.util.cparser.C.CParserUtils;
import ghidra.program.model.data.FileDataTypeManager;

public class CreateAmigaNdk39Gdt extends GhidraScript {
	private static final String INCLUDE_ROOT = "3rdparty/NDK_3.9/include_h";
	private static final String HEADER_MANIFEST = "data/ndk39_headers.txt";
	/*
	 * hdwrench.h uses SAS/C register annotations, including one on a callback
	 * parameter.  They describe a calling convention, not a C data type, and
	 * Ghidra's C parser does not accept that nested form.  Neutralising the
	 * annotations lets the original header contribute its declarations without
	 * modifying or omitting the vendored NDK source.
	 */
	private static final String[] PARSER_ARGUMENTS = { "-v0", "-Dregister=", "-D__asm=", "-D__saveds=",
			"-D__a0=", "-D__a1=", "-D__a2=", "-D__a3=", "-D__a4=", "-D__a5=", "-D__a6=", "-D__a7=",
			"-D__d0=", "-D__d1=", "-D__d2=", "-D__d3=", "-D__d4=", "-D__d5=", "-D__d6=", "-D__d7=" };

	@Override
	protected void run() throws Exception {
		String[] arguments = getScriptArgs();
		if (arguments.length != 2) {
			throw new IllegalArgumentException("Usage: CreateAmigaNdk39Gdt.java <repository-root> <output-gdt-path>");
		}
		Path repository = Path.of(arguments[0]).toAbsolutePath().normalize();
		Path includeRoot = repository.resolve(INCLUDE_ROOT);
		Path manifest = repository.resolve(HEADER_MANIFEST);
		Path output = Path.of(arguments[1]).toAbsolutePath().normalize();
		List<String> headers = readAndValidateManifest(includeRoot, manifest);

		Files.createDirectories(output.getParent());
		deleteArchive(output);
		String[] headerFiles = headers.stream().map(includeRoot::resolve).map(Path::toString).toArray(String[]::new);
		FileDataTypeManager archive = CParserUtils.parseHeaderFiles(null, headerFiles,
				new String[] { includeRoot.toString() }, PARSER_ARGUMENTS, output.toString(),
				"68000:BE:32:default", "default", monitor);
		archive.close();
		Files.deleteIfExists(Path.of(output + "_CParser.out"));
		println("Created " + output + " from " + headers.size() + " NDK declaration headers.");
	}

	private static List<String> readAndValidateManifest(Path includeRoot, Path manifest) throws IOException {
		if (!Files.isDirectory(includeRoot) || !Files.isRegularFile(manifest)) {
			throw new IOException("Repository root does not contain " + INCLUDE_ROOT + " and " + HEADER_MANIFEST);
		}
		List<String> headers = Files.readAllLines(manifest).stream().map(String::strip)
				.filter(value -> !value.isEmpty() && !value.startsWith("#")).toList();
		Set<String> listed = new LinkedHashSet<>(headers);
		if (listed.size() != headers.size()) {
			throw new IOException("Duplicate header entry in " + HEADER_MANIFEST);
		}
		for (String header : headers) {
			if (header.startsWith("/") || header.contains("..") || !Files.isRegularFile(includeRoot.resolve(header))) {
				throw new IOException("Missing or unsafe manifest header: " + header);
			}
		}
		Set<String> declarationHeaders;
		try (var paths = Files.walk(includeRoot)) {
			declarationHeaders = paths.filter(Files::isRegularFile).map(includeRoot::relativize)
					.map(Path::toString).map(value -> value.replace(File.separatorChar, '/'))
					.filter(value -> value.endsWith(".h") && !isForwardingHeader(value))
					.collect(Collectors.toCollection(LinkedHashSet::new));
		}
		if (!listed.equals(declarationHeaders)) {
			Set<String> missing = new LinkedHashSet<>(declarationHeaders);
			missing.removeAll(listed);
			Set<String> stale = new LinkedHashSet<>(listed);
			stale.removeAll(declarationHeaders);
			throw new IOException("NDK header manifest mismatch; missing=" + missing + ", stale=" + stale);
		}
		return headers;
	}

	private static boolean isForwardingHeader(String header) {
		return header.startsWith("clib/") || header.startsWith("proto/") || header.startsWith("pragma/") ||
				header.startsWith("pragmas/");
	}

	private static void deleteArchive(Path output) throws IOException {
		Files.deleteIfExists(output);
		Files.deleteIfExists(Path.of(output + ".ulock"));
		Files.deleteIfExists(Path.of(output + "_CParser.out"));
	}
}
