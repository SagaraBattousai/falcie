#include <cstring>
#include <gtest/gtest.h>

extern "C" {
#include <pulse/linalg.h>
}

TEST(AnimaMatrix, MatrixMult)
{
	float in1[6] = { 7,  8,  9, 10, 11, 12 };
	float in2[6] = { 27, 28, 29, 30, 31, 32 };
	float expected[4] = { 700, 724, 961, 994 };
	float actual[4] = { 0 };
	matrix_multiply(in1, in2, actual, 2, 3, 2);
	EXPECT_EQ(std::memcmp(actual, expected, 4), 0);
}

TEST(AnimaMatrix, VectorViaMatrixMult)
{
	float in1[6] = { 7,  8,  9, 10, 11, 12 };
	float in2[6] = { 27, 28, 29, 30, 31, 32 };
	float expected[1] = { 1699 };
	float actual[1] = { 0 };
	matrix_multiply(in1, in2, actual, 1, 6, 1);
	EXPECT_EQ(std::memcmp(actual, expected, 1), 0);
}

TEST(AnimaMatrix, VectorDot)
{
	float in1[6] = { 7,  8,  9, 10, 11, 12 };
	float in2[6] = { 27, 28, 29, 30, 31, 32 };
	float expected = 1699;
	float actual = vector_dot(in1, in2, 6);
	EXPECT_EQ(actual, expected);
}
