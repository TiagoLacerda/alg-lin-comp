#include "FourierTransform.hpp"

#include <cmath>
#include <algorithm>

#include "Util.hpp"

namespace FFT {
	std::vector<std::complex<float>> computeTwiddles(const int n)
	{
		constexpr float pi = 3.14159265358979323846f;
		constexpr float tau = 2.0f * pi;

		std::vector<std::complex<float>> twiddles(n / 2);

		for (int k = 0; k < n / 2; ++k)
		{
			const float angle = -tau * static_cast<float>(k) / static_cast<float>(n);

			twiddles[k] =
			{
				std::cos(angle),
				std::sin(angle)
			};
		}

		return twiddles;
	}

	void fastFourierTransform(std::vector<std::complex<float>>& data, const std::vector<std::complex<float>>& twiddles)
	{
		const int n = static_cast<int>(data.size());

		const int stages = static_cast<int>(std::log2(n));

		for (int i = 0; i < n; ++i)
		{
			int j = static_cast<int>(reverseBits(i, stages));

			if (j > i)
			{
				std::swap(data[i], data[j]);
			}
		}

		for (int size = 2; size <= n; size <<= 1)
		{
			const int halfSize = size >> 1;

			const int twiddleStep = n / size;

			for (int start = 0; start < n; start += size)
			{
				for (int k = 0; k < halfSize; ++k)
				{
					const auto twiddle = twiddles[k * twiddleStep];

					const auto even = data[start + k];

					const auto odd = twiddle * data[start + k + halfSize];

					data[start + k] = even + odd;

					data[start + k + halfSize] = even - odd;
				}
			}
		}
	}

	void inverseFastFourierTransform(std::vector<std::complex<float>>& data, const std::vector<std::complex<float>>& twiddles)
	{
		const int n = static_cast<int>(data.size());

		for (int i = 0; i < n; ++i)
		{
			data[i] = std::conj(data[i]);
		}

		fastFourierTransform(data, twiddles);

		for (int i = 0; i < n; ++i)
		{
			data[i] = std::conj(data[i]) / static_cast<float>(n);
		}
	}
}