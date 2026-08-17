package amiga;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import ghidra.framework.Application;

/**
 * Header-derived Amiga ABI contracts that are not represented by a library
 * function-definition table.  Types and field names live in data files so the
 * analyser does not manufacture platform types in Java code.
 */
final class AmigaAbiModel {
	private static final String DEVICE_DISPATCH_FILE = "abi/amiga_device.properties";

	static final class Parameter {
		final String name;
		final String type;
		final String register;

		Parameter(String name, String type, String register) {
			this.name = name;
			this.type = type;
			this.register = register;
		}
	}

	static final class DeviceDispatch {
		final String name;
		final String returnType;
		final String returnRegister;
		final int vector;
		final String structureType;
		final String structureField;
		final List<Parameter> parameters;

		DeviceDispatch(String name, String returnType, String returnRegister, int vector, String structureType, String structureField,
				List<Parameter> parameters) {
			this.name = name;
			this.returnType = returnType;
			this.returnRegister = returnRegister;
			this.vector = vector;
			this.structureType = structureType;
			this.structureField = structureField;
			this.parameters = List.copyOf(parameters);
		}
	}

	private AmigaAbiModel() {
	}

	static List<DeviceDispatch> loadDeviceDispatches() throws IOException {
		Properties properties = new Properties();
		File file = getDataFile(DEVICE_DISPATCH_FILE);
		try (FileInputStream input = new FileInputStream(file)) {
			properties.load(input);
		}
		List<DeviceDispatch> dispatches = new ArrayList<>();
		for (String id : properties.getProperty("dispatches", "").split(",")) {
			id = id.strip();
			if (id.isEmpty()) {
				continue;
			}
			String prefix = "dispatch." + id + ".";
			String name = required(properties, prefix + "name");
			String returnType = required(properties, prefix + "return");
			String returnRegister = properties.getProperty(prefix + "return_register", "").strip();
			int vector = Integer.decode(required(properties, prefix + "vector"));
			String structureType = required(properties, prefix + "structure_type");
			String structureField = required(properties, prefix + "structure_field");
			List<Parameter> parameters = new ArrayList<>();
			for (int index = 0;; index++) {
				String value = properties.getProperty(prefix + "parameter." + index);
				if (value == null) {
					break;
				}
				String[] fields = value.split("\\|", -1);
				if (fields.length != 3) {
					throw new IOException("Invalid ABI parameter " + prefix + "parameter." + index);
				}
				parameters.add(new Parameter(fields[0].strip(), fields[1].strip(), fields[2].strip()));
			}
			if (parameters.isEmpty()) {
				throw new IOException("ABI dispatch " + id + " has no parameters");
			}
			dispatches.add(new DeviceDispatch(name, returnType, returnRegister, vector, structureType, structureField,
					parameters));
		}
		return List.copyOf(dispatches);
	}

	private static String required(Properties properties, String key) throws IOException {
		String value = properties.getProperty(key);
		if (value == null || value.isBlank()) {
			throw new IOException("Missing ABI property " + key);
		}
		return value.strip();
	}

	private static File getDataFile(String name) throws IOException {
		try {
			return Application.getModuleDataFile(name).getFile(false);
		}
		catch (IOException exception) {
			File sourceData = new File("data", name);
			if (sourceData.isFile()) {
				return sourceData;
			}
			throw exception;
		}
	}
}
