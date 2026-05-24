#include "Util.hpp"

#include <cmath>
#include <cassert>

void interpolate(const std::vector<float>& x, const std::vector<float>& y, const std::vector<float>& z, std::vector<float>& w)
{
	assert(x.size() == y.size());
	assert(x.size() >= 2);

	w.resize(z.size());

	std::size_t i = 0;

	for (std::size_t j = 0; j < z.size(); ++j)
	{
		const float zj = z[j];

		if (zj <= x.front())
		{
			const float dx = x[1] - x[0];
			const float dy = y[1] - y[0];

			w[j] = y[0] + (zj - x[0]) * dy / dx;
			continue;
		}

		if (zj >= x.back())
		{
			const std::size_t n = x.size();

			const float dx = x[n - 1] - x[n - 2];
			const float dy = y[n - 1] - y[n - 2];

			w[j] = y[n - 1] + (zj - x[n - 1]) * dy / dx;
			continue;
		}

		while (i + 1 < x.size() && x[i + 1] < zj)
		{
			++i;
		}

		const float t = (zj - x[i]) / (x[i + 1] - x[i]);

		w[j] = y[i] + t * (y[i + 1] - y[i]);
	}
}

float wrapAngle(const float angle)
{
	constexpr float pi = 3.14159265358979323846f;
	constexpr float tau = 2.0f * pi;

	float value = std::fmod(angle + pi, tau);

	if (value < 0.0f)
	{
		value += tau;
	}

	return value - pi;
}

std::vector<float> computeSqrtHanning(const int n)
{
	constexpr float pi = 3.14159265358979323846f;
	constexpr float tau = 2.0f * pi;

	if (n <= 0)
	{
		return {};
	}

	if (n == 1)
	{
		return { 1.0f };
	}

	const float scale = tau / static_cast<float>(n - 1);

	std::vector<float> window(n);

	for (int i = 0; i < n; ++i)
	{
		window[i] = std::sqrt(0.5f * (1.0f - std::cos(scale * i)));
	}

	return window;
}

unsigned int reverseBits(unsigned int value, const int bitCount)
{
	unsigned int reversed = 0;

	for (int i = 0; i < bitCount; ++i)
	{
		reversed <<= 1;

		reversed |= (value & 1);

		value >>= 1;
	}

	return reversed;
}