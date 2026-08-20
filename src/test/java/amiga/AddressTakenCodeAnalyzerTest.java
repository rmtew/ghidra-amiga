package amiga;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import ghidra.app.services.AnalysisPriority;

public class AddressTakenCodeAnalyzerTest {

	@Test
	public void recognizesUntypedPrintableNulTerminatedText() {
		assertTrue(AddressTakenCodeAnalyzer.isPrintableNulTerminatedAscii(
				new byte[] { 's', 'a', 'v', 'e', 0 }, 5));
		assertTrue(AddressTakenCodeAnalyzer.isPrintableNulTerminatedAscii(
				new byte[] { 'M', 'u', 's', 'i', 'c', '.', 't', 'a', 's', 'k', 0 }, 11));
	}

	@Test
	public void doesNotTreatCodeOrUnterminatedBytesAsText() {
		assertFalse(AddressTakenCodeAnalyzer.isPrintableNulTerminatedAscii(
				new byte[] { 0x4e, 0x55, (byte) 0xff, (byte) 0xfc }, 4));
		assertFalse(AddressTakenCodeAnalyzer.isPrintableNulTerminatedAscii(
				new byte[] { 'c', 'o', 'd', 'e' }, 4));
	}

	@Test
	public void recognizesGhidraAndDocumentationReturnMnemonicCasing() {
		assertTrue(M68kControlFlow.isReturnMnemonic("rts"));
		assertTrue(M68kControlFlow.isReturnMnemonic("RTE"));
		assertFalse(M68kControlFlow.isReturnMnemonic("jmp"));
	}

	@Test
	public void defaultsToAddressTakenDiscoveryForHunkImportsOnly() {
		assertTrue(AddressTakenCodeAnalyzer.isAmigaHunkExecutable("Amiga Hunk Executable"));
		assertFalse(AddressTakenCodeAnalyzer.isAmigaHunkExecutable("Raw Binary"));
		assertFalse(AddressTakenCodeAnalyzer.isAmigaHunkExecutable(null));
	}

	@Test
	public void runsAfterReferenceAnalysisEstablishesTheEvidence() {
		assertTrue(new AddressTakenCodeAnalyzer().getPriority().priority() >
				AnalysisPriority.REFERENCE_ANALYSIS.priority());
	}
}
