module;

#include <cstdint>

#ifdef USE_AVX_INTRIN
#include <immintrin.h>
#endif // USE_AVX_INTRIN


export module anima:matrix;

import <array>;
import <vector>;


export namespace pulse
{
	template <typename T, std::int64_t Dim1, std::int64_t Dim2>
	class Matrix;

	template<typename T, std::int64_t Dim1, std::int64_t Dim2, std::int64_t Dim3>
	Matrix<T, Dim1, Dim3> operator*(const Matrix<T, Dim1, Dim2>&, const Matrix<T, Dim2, Dim3>&);

	template<typename T, std::int64_t Dim1, std::int64_t Dim2>
	Matrix<T, Dim1, Dim2> operator+(Matrix<T, Dim1, Dim2>, const Matrix<T, Dim1, Dim2>&);

	template <typename T, std::int64_t Dim1, std::int64_t Dim2>
	class Matrix
	{
	public:
		Matrix();
		Matrix(std::array<std::array<T, Dim2>, Dim1> values);
		//??? T(); Want ref to self but dims are swapped so .... Coud carry .T ref to self?

		//As much as I hate defining in class declaration it seems MSVC has a bug
		//that requires it when const overriding and operator :'(

		const std::array<T, Dim2>& operator[](std::int64_t index) const
		{
			return this->data[index];
		};

		//Not sure I want writeable version
		std::array<T, Dim2>& operator[](std::int64_t index)
		{
			return this->data[index];
		};

		//Doesnt need to be friend  So
		//template<typename T, std::int64_t Dim1, std::int64_t Dim2, std::int64_t Dim3>
		//friend Matrix<T, Dim1, Dim3> operator* <T, Dim1, Dim2, Dim3> (
		//	const Matrix<T, Dim1, Dim2>&,
		//	const Matrix<T, Dim2, Dim3>&);

		template<std::int64_t Dim3>
		Matrix<T, Dim1, Dim3>& operator*=(const Matrix<T, Dim2, Dim3>& rhs);

		Matrix& operator+=(const Matrix&);

		friend Matrix operator+ <T, Dim1, Dim2> (Matrix, const Matrix&);

	private:
		std::array<std::array<T, Dim2>, Dim1> data;
	};

	template <typename T, std::int64_t Dim1>
	using Vector = Matrix<T, Dim1, 1>;

	template <typename T, std::int64_t Dim1, std::int64_t Dim2>
	Matrix<T, Dim1, Dim2>::Matrix()
		: Matrix(std::array<std::array<T, Dim2>, Dim1>{})
	{}

	template <typename T, std::int64_t Dim1, std::int64_t Dim2>
	Matrix<T, Dim1, Dim2>::Matrix(
		std::array<std::array<T, Dim2>, Dim1> values)
		: data(values)
	{}

	template<typename T, std::int64_t Dim1, std::int64_t Dim2>
	template<std::int64_t Dim3>
	Matrix<T, Dim1, Dim3>& pulse::Matrix<T, Dim1, Dim2>::operator*=(const Matrix<T, Dim2, Dim3>& rhs)
	{
		*this = (*this * rhs);
		return *this;
	}

	template<typename T, std::int64_t Dim1, std::int64_t Dim2, std::int64_t Dim3>
	Matrix<T, Dim1, Dim3> operator*(
		const Matrix<T, Dim1, Dim2>& lhs,
		const Matrix<T, Dim2, Dim3>& rhs)
	{
		//Zero init output
		std::array<std::array<T, Dim3>, Dim1> mout{};
		//#ifdef USE_AVX_INTRIN
			//
		//#else

		for (std::int64_t i = 0; i < Dim1; i++)
		{
			for (std::int64_t j = 0; j < Dim2; j++)
			{
				for (std::int64_t k = 0; k < Dim3; k++)
				{
					mout[i][k] += lhs[i][j] * rhs[j][k];
				}
			}
		}

		//#endif

		return Matrix<T, Dim1, Dim3>(mout);
	}

	template<typename T, std::int64_t Dim1, std::int64_t Dim2>
	Matrix<T, Dim1, Dim2>& Matrix<T, Dim1, Dim2>::operator+=(const Matrix<T, Dim1, Dim2>& rhs)
	{
		//#ifdef USE_AVX_INTRIN
			//
		//#else

		for (std::int64_t i = 0; i < Dim1; i++)
		{
			for (std::int64_t j = 0; j < Dim2; j++)
			{
				(*this)[i][j] = (*this)[i][j] + rhs[i][j];
			}
		}

		//#endif

		return *this;
	}

	template<typename T, std::int64_t Dim1, std::int64_t Dim2>
	Matrix<T, Dim1, Dim2> operator+(Matrix<T, Dim1, Dim2> lhs, const Matrix<T, Dim1, Dim2>& rhs)
	{
		lhs += rhs;
		return lhs;
	}



		/* Can't trust __AVX__ to be defined for non MSVC.
		 * Therefore setting USE_AVX_INTRIN from CMAKE if compiled with AVX.
		 */
		 /*

		 #ifdef USE_AVX_INTRIN

			 constexpr int AVX_512_PACK_SIZE = 16;
			 constexpr int AVX_512_PACK_SHIFT = 4; //Used for multipying by 16

			 //Overflow if not mult of 16
			 static inline void inner_intrin_matrix_mult(__m512 sv, float *rowB, float *rowC)
			 {
				 //Can I assume aligned? i.e. do I need to use the u versions (loadu storeu etc)
				 __m512 cv = _mm512_load_ps(rowC);
				 __m512 bv = _mm512_load_ps(rowB);
				 cv = _mm512_fmadd_ps(bv, sv, cv);
				 _mm512_store_ps(rowC, cv);
			 }

			 void matrix_multiply(float *in1, float *in2, float *out,
				 const int64_t dim1, const int64_t dim2, const int64_t dim3)
			 {
				 const int64_t packed_width = dim3 >> AVX_512_PACK_SHIFT; //i.e. /16

				 for (int64_t i = 0; i < dim1; i++)
				 {
					 int64_t out_row_start = i * dim3;

					 for (int64_t j = 0; j < dim2; j++)
					 {
						 __m512 sv = _mm512_set1_ps(in1[i * dim2 + j]); //A_ij
						 int64_t in_row_start = j * dim3;

						 //Can only work on 16 elements per row at a time
						 for (int64_t packed_segment = 0; packed_segment < packed_width; packed_segment++)
						 {
							 int64_t packed_offset = packed_segment << AVX_512_PACK_SHIFT; //i.e. * 16

							 inner_intrin_matrix_mult(
								 sv,
								 &in2[in_row_start + packed_offset],
								 &out[out_row_start + packed_offset]
							 );
						 }

						 // Handle remaining coloumns when the number of elements is not a multiple of
						 // The packing size. In this case 16.
						 int64_t columns_remaining = dim3 % AVX_512_PACK_SIZE;
						 if (columns_remaining != 0)
						 {
							 int64_t remaining_offset = dim3 - columns_remaining;
							 // Don't want the possibility of reading from bad location so we
							 // pad the input row too.
							 float in_padded_row[AVX_512_PACK_SIZE] = { 0 };
							 float out_padded_row[AVX_512_PACK_SIZE] = { 0 };

							 memcpy(in_padded_row, &in2[in_row_start + remaining_offset],
								 sizeof(float) * columns_remaining);
							 memcpy(out_padded_row, &out[out_row_start + remaining_offset],
								 sizeof(float) * columns_remaining);

							 inner_intrin_matrix_mult(sv, in_padded_row, out_padded_row);

							 memcpy(&out[out_row_start + remaining_offset], out_padded_row,
								 sizeof(float) * columns_remaining);
						 }
					 }
				 }
			 }

		 #endif

		 #else
		 #include <stdlib.h>
		 #include <anima/matrix.h>



		 #endif







		 #include <anima/linalg.h>

		 //TODO: AVX implemntation
		 float vector_dot(float *in1, float *in2, const int64_t dim)
		 {
			 float out = 0;
			 matrix_multiply(in1, in2, &out, 1, dim, 1);
			 return out;
		 }

		 void scalar_vector_sum(const float scalar, float *vector, float *out, const int64_t dim)
		 {
			 for (int64_t i = 0; i < dim; i++)
			 {
				 *(out + i) = scalar + *(vector + i);
			 }
		 }

		 //TODO: AVX implemntation
		 void scalar_vector_mult(const float scalar, float *vector, float *out, const int64_t dim)
		 {
			 for (int64_t i = 0; i < dim; i++)
			 {
				 *(out + i) = scalar * *(vector + i);
			 }
		 }

		 //TODO: AVX implemntation
		 void vector_vector_sum(float *vector1, float *vector2, float *out, const int64_t dim)
		 {
			 for (int64_t i = 0; i < dim; i++)
			 {
				 out[i] = vector1[i] + vector2[i];
			 }
		 }

		 //TODO: AVX implemntation
		 void vector_vector_sub(float *vector1, float *vector2, float *out, const int64_t dim)
		 {
			 for (int64_t i = 0; i < dim; i++)
			 {
				 out[i] = vector1[i] - vector2[i];
			 }
		 }


		 //TODO: AVX implemntation
		 void vector_vector_mult(float *vector1, float *vector2, float *out, const int64_t dim)
		 {
			 for (int64_t i = 0; i < dim; i++)
			 {
				 out[i] = vector1[i] * vector2[i];
			 }
		 }

		 //TODO: AVX implemntation
		 //in1 is [m * 1] and  in2 is [1 * n]. out is [m * n]
		 //Can't be mutable as in1 nd in2 are reused (double looped) therefore
		 //overwriting would change values.
		 void broadcast_vectors(const float *in1, const float *in2, float *out,
			 const int64_t dim1, const int64_t dim2, combinator_ptr combinator)
		 {
			 for (int64_t i = 0; i < dim1; i++)
			 {
				 for (int64_t j = 0; j < dim2; j++)
				 {
					 out[i * dim2 + j] = combinator(in1[i], in2[j]);
				 }

			 }
		 }

		 //Can it be static? It can, and must thanks to federated_blockchain
		 //also having an add function ...
		 static float add(float a, float b)
		 {
			 return a + b;
		 }

		 static float multiply(float a, float b)
		 {
			 return a * b;
		 }

		 combinator_ptr adder()
		 {
			 return &add;
		 }
		 combinator_ptr mult()
		 {
			 return &multiply;
		 }

		 */

	} //namespace pulse
