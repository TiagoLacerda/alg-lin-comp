#pragma once

#include <vector>

/// <summary>
/// Piecewise-linear interpolation of data points (x, y) evaluated at points (z, w).
/// </summary>
/// <param name="x">X-coordinates of the source data points.</param>
/// <param name="y">Y-coordinates of the source data points.</param>
/// <param name="z">X-coordinates at which to evaluate the interpolated function.</param>
/// <param name="w">Output vector receiving the interpolated values for each position in z.</param>
void interpolate(const std::vector<float>& x, const std::vector<float>& y, const std::vector<float>& z, std::vector<float>& w);

/// <summary>
/// Normalizes an angle in radians to the interval [-π, π).
/// </summary>
/// <param name="angle">Angle in radians to wrap.</param>
/// <returns>
/// An angle equivalent to <paramref name="angle"/> modulo 2π, in the range
/// [-π, π). Values equal to π are represented as -π.
/// </returns>
float wrapAngle(const float angle);

/// <summary>
/// Computes a square-root Hann window.
/// </summary>
/// <param name="n">
/// Number of samples in the window.
/// </param>
/// <returns>
/// A vector containing <paramref name="n"/> square-root Hann window coefficients.
/// Returns an empty vector if <paramref name="n"/> is non-positive.
/// </returns>
std::vector<float> computeSqrtHanning(const int n);

/// <summary>
/// Reverses the order of the least-significant bits of an unsigned integer.
/// </summary>
unsigned int reverseBits(unsigned int value, const int bitCount);