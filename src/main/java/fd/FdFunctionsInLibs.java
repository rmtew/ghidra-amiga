package fd;

import java.io.File;
import java.io.IOException;
import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.Map.Entry;

import ghidra.framework.Application;

public class FdFunctionsInLibs {
	private List<FdFunction> funcsList;
	private List<String> libsList;
	private List<Entry<String, FdLibFunctions>> libFuncs;
	private List<String> loadWarnings;
	
	private void initList() {
		funcsList = new ArrayList<>();
		libsList = new ArrayList<>();
		libFuncs = new ArrayList<>();
		loadWarnings = new ArrayList<>();
		
		try {
			File dir;
			try {
				dir = Application.getModuleDataSubDirectory("sfd").getFile(false);
			} catch (IOException e) {
				dir = new File("data/sfd");
			}
			if (!dir.isDirectory()) {
				return;
			}
			
			File[] entries = dir.listFiles((directory, name) -> name.toLowerCase().endsWith(".sfd"));
			if (entries == null) {
				return;
			}
			Arrays.sort(entries, Comparator.comparing(File::getName));
			for (final File entry : entries) {
				FdLibFunctions fd = FdParser.readSfdFile(entry.getPath());
				if(fd != null) {
					var lname = fd.getBaseName().toLowerCase();
					libsList.add(lname);
					libFuncs.add(new AbstractMap.SimpleEntry<String, FdLibFunctions>(lname, fd));
					funcsList.addAll(Arrays.asList(fd.getFunctions()));
				}
				else {
					loadWarnings.add("Skipped unreadable API definition: " + entry.getName());
				}
		    }
		} catch (RuntimeException e) {
			loadWarnings.add("Unable to load Amiga API definitions: " + e.getMessage());
		}
	}
	
	public FdFunctionsInLibs() {
		initList();
	}
	
	public String[] getLibsList(List<String> filter) {
		if (filter == null || filter.size() == 0) {
			return libsList.toArray(String[]::new);
		} else {
			return libsList.stream().filter(e -> filter.contains(e.toLowerCase())).toArray(String[]::new);
		}
	}

	public List<String> getLoadWarnings() {
		return List.copyOf(loadWarnings);
	}
	
	public int findLibIndex(String lib) {
		return libsList.indexOf(lib);
	}
	
	public FdFunction[] getLibsFunctionsByBias(List<String> filter, int bias) {
		if (filter == null || filter.size() == 0) {
			return funcsList.stream().filter(e -> e.getBias() == bias).toArray(FdFunction[]::new);
		} else {
			return funcsList.stream().filter(e -> (filter.contains(e.getLib().toLowerCase()) && e.getBias() == bias)).toArray(FdFunction[]::new);
		}
	}
	
	public FdFunction[] getFunctionsByLibs(List<String> filter) {
		if (filter == null || filter.size() == 0) {
			return funcsList.toArray(FdFunction[]::new);
		} else {
			return funcsList.stream().filter(e -> filter.contains(e.getLib().toLowerCase())).toArray(FdFunction[]::new);
		}
	}
	
	public FdLibFunctions getFunctionTableByLib(String lib) {
		return libFuncs.stream().filter(e -> e.getKey().equals(lib)).map(e -> e.getValue()).findFirst().orElse(null);
	}
}
