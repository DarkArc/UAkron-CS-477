// conv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <thread>
#include <memory>

#include "../../class-common/include/thread.h"
#include "../../class-common/include/image.h"

using namespace cs477;

matrix obtainY(const matrix& x, const matrix& k) {
	matrix y;
	y.create(x.rows + k.rows, x.cols + k.cols);

	for (unsigned row = 0; row < x.rows; row++)
	{
		for (unsigned col = 0; col < x.cols; col++)
		{
			auto yrow = row + k.rows / 2;
			auto ycol = col + k.cols / 2;
			y(yrow, ycol) = x(row, col);
		}
	}

	return y;
}

int computeWeight(const matrix& k) {
	int weight = 0;
	for (unsigned row = 0; row < k.rows; row++)
	{
		for (unsigned col = 0; col < k.cols; col++)
		{
			weight += k(row, col);
		}
	}
	return weight;
}

void completeConv(const int& row, const int& col, const int& weight, const matrix& k, const matrix& y, matrix& x) {
	int t = 0;

	auto yrow = row - k.rows / 2 + 1;
	for (int krow = k.rows - 1; krow >= 0; krow--, yrow++)
	{
		auto ycol = col - k.cols / 2 + 1;
		for (int kcol = k.cols - 1; kcol >= 0; kcol--, ycol++)
		{
			t += y(yrow, ycol) * k(krow, kcol);
		}
	}

	if (weight != 0)
	{
		t /= weight;
	}

	x(row, col) = t;
}

void completeConv(const int& weight, const matrix& k, const matrix& y, matrix& x) {
	const int threadCount = std::thread::hardware_concurrency();
	const int threadUnit = x.rows / threadCount;

	std::vector<future<void>> futures;
	for (unsigned int thread = 0; thread < threadCount; ++thread) {
		auto future = queue_work([&weight, &k, &y, &x, &threadUnit, thread] {
		  for (unsigned int row = threadUnit * thread; row < threadUnit * (thread + 1); ++row) {
			  for (unsigned int col = 0; col < x.cols; ++col) {
				  completeConv(row, col, weight, k, y, x);
			  }
		  }
		});
    futures.push_back(std::move(future));
	}

  when_all(futures.begin(), futures.end()).get();
}

void conv(matrix &x, const matrix &k)
{
	matrix y = obtainY(x, k);

	// Compute sum of k
	int weight= computeWeight(k);

	// Do the convolution
	completeConv(weight, k, y, x);
}


int binomial_coefficient(int n, int k)
{
	if (n <= 1 || k == 0)
	{
		return 1;
	}
	else
	{
		return binomial_coefficient(n - 1, k - 1) * n / k;
	}
}

matrix binomial(int n)
{
	if ((n & 1) == 0)
	{
		throw std::invalid_argument("n must be odd");
	}

	matrix x, y;
	x.create(1, n);
	y.create(n, 1);

	for (int i = 0; i < n / 2; i++)
	{

		x(0, i) = x(0, n - i - 1) = binomial_coefficient(n - 1, i);
		y(i, 0) = y(n - i - 1, 0) = binomial_coefficient(n - 1, i);
	}

	x(0, n / 2) = binomial_coefficient(n - 1, n / 2);
	y(n / 2, 0) = binomial_coefficient(n - 1, n / 2);

	return y * x;
}


int main()
{
	auto bmp = load_image("test.png");
	auto orig = bmp;

	matrix kernel = binomial(3);

	conv(bmp, kernel);

	save_png(bmp, "test.out.png");

    return 0;
}
