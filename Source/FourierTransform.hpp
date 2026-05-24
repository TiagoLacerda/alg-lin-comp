#pragma once

#include <complex>
#include <vector>

namespace FFT {
	/// <returns>
	/// A vector containing W_n^k = exp(-j·2π·k/n) for k in [0, n/2).
	/// </returns>
	std::vector<std::complex<float>> computeTwiddles(const int n);

	/// <summary>
	/// Computes the in-place radix-2 Cooley-Tukey fast Fourier transform.
	/// </summary>
	/// <param name="data">
	/// Input samples and output spectrum. The size must be a power of two.
	/// </param>
	/// <param name="twiddles">
	/// Twiddle factors generated for a transform of the same size.
	/// </param>
	void fastFourierTransform(std::vector<std::complex<float>>& data, const std::vector<std::complex<float>>& twiddles);

	/// <summary>
	/// Computes the in-place inverse radix-2 Cooley-Tukey fast Fourier transform.
	/// </summary>
	/// <param name="data">
	/// Input spectrum and output samples. The size must be a power of two.
	/// </param>
	/// <param name="twiddles">
	/// Twiddle factors generated for a transform of the same size.
	/// </param>
	void inverseFastFourierTransform(std::vector<std::complex<float>>& data, const std::vector<std::complex<float>>& twiddles);
}