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

TEST(AnimaVector, VectorDot)
{
	float in1[6] = { 7,  8,  9, 10, 11, 12 };
	float in2[6] = { 27, 28, 29, 30, 31, 32 };
	float expected = 1699;
	float actual = vector_dot(in1, in2, 6);
	EXPECT_EQ(actual, expected);
}

TEST(AnimaVector, VectorBroadcast)
{
	float in1[3] = { 1, 2, 3 };
	float in2[5] = { 6, 7, 8, 9, 10 };

	float expected[15] = {  6, 12, 18,
						    7, 14, 21,
						    8, 16, 24,
						    9, 18, 27,
						   10, 20, 30};

	float actual[15] = { 0 };

	broadcast_vectors(in1, in2, actual, 3, 5, mult());
	EXPECT_EQ(std::memcmp(actual, expected, sizeof(float) * 15), 0);
}

TEST(AnimaVector, VectorScalarMult)
{
	float scalar = 9;
	float in1[5] = { 6, 7, 8, 9, 10 };

	float expected[5] = { 54, 63, 72, 81, 90 };

	float actual[5] = { 0 };

	scalar_vector_mult(scalar, in1, actual, 5);
	EXPECT_EQ(std::memcmp(actual, expected, sizeof(float) * 5), 0);
}

TEST(AnimaVector, VectorMutableScalarMult)
{
	float scalar = 9;
	float in1[5] = { 6, 7, 8, 9, 10 };

	float expected[5] = { 54, 63, 72, 81, 90 };

	scalar_vector_mult(scalar, in1, in1, 5);
	EXPECT_EQ(std::memcmp(in1, expected, sizeof(float) * 5), 0);

}

