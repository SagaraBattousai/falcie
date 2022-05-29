module;

#include <cstdint>
#include <stdexcept>

#ifdef USE_AVX_INTRIN
#include <immintrin.h>
#endif // USE_AVX_INTRIN


export module anima:matrix;

//import <array>;
import <vector>;
import <initializer_list>;
import <utility>;


export namespace pulse
{
	using Dimensions = std::pair<std::int64_t, std::int64_t>;

	template <typename T>
	class Matrix;

	template<typename T>
	Matrix<T> operator*(const Matrix<T>&, const Matrix<T>&);

	template<typename T>
	Matrix<T> operator+(Matrix<T>, const Matrix<T>&);

	template<typename T>
	Matrix<T> operator*(const T&, Matrix<T>);

	template<typename T>
	Matrix<T> operator+(const T&, Matrix<T>);

	template<typename T>
	std::vector<T> operator*(const std::vector<T>&, const Matrix<T>&);

	//for now we'll cheat but next iteration TODO: add reference to transpose data 
	//(which will index differently) aka from whiteboard!
	template<typename T>
	std::vector<T> operator*(const Matrix<T>& lhs, const std::vector<T>& rhs);


	//void broadcast_vectors(const float *in1, const float *in2,
		//float *out, const int64_t dim1, const int64_t dim2, combinator_ptr combinator);


	template <typename T>
	class Matrix
	{
	public:
		Matrix(Dimensions init_shape);
		Matrix(std::vector<T> values);
		Matrix(std::vector<T> values, Dimensions init_shape);

		constexpr std::int64_t TotalElementCount();

		constexpr std::int64_t RowCount();
		constexpr std::int64_t ColumnCount();


		//??? T(); Want ref to self but dims are swapped so .... Coud carry .T ref to self?

		//As much as I hate defining in class declaration it seems MSVC has a bug
		//that requires it when const overriding and operator :'(

		const T& operator[](Dimensions index) const
		{
			return this->data[index.first * this->dims.second + index.second];
		};

		//Writeable version
		T& operator[](Dimensions index)
		{
			return this->data[index.first * this->dims.second + index.second];
		};

		const T& operator[](std::int64_t i) const
		{
			return this->data[i];
		};

		//Writeable version
		T& operator[](std::int64_t i)
		{
			return this->data[i];
		};

		//non-friend non-member are better but...I was making functions
		//that were unneccisary just to stop it being a friend.
		friend Matrix operator*<T>(const Matrix&, const Matrix&);

		Matrix& operator*=(const Matrix& rhs);
		Matrix& operator+=(const Matrix&);

		Matrix& operator*=(const T&);
		Matrix& operator+=(const T&);

		//TODO:
		//This may be messy and evil
		friend std::vector<T> operator*<T>(const std::vector<T>&, const Matrix&);
		//for now we'll cheat but next iteration TODO: add reference to transpose data 
	//(which will index differently) aka from whiteboard!
		friend std::vector<T> operator*<T>(const Matrix&, const std::vector<T>&);



		//I think this'll cause an issue (constexpr could be safer?
		const std::vector<T>& Data() const;

		const Dimensions& Shape();

	private:
		std::vector<T> data;

		//These are constant so maybe constexpr??
		Dimensions dims;
	};


	template <typename T>
	Matrix<T>::Matrix(Dimensions init_shape)
		: Matrix(std::vector<T>(init_shape.first * init_shape.second), init_shape)
	{
	}

	//Just for now: make a Vector a column not row vector (... is this okay or a cheat?)
	// Transpose instead?
	//BUGS in constructure im gessing 
	template <typename T>
	Matrix<T>::Matrix(std::vector<T> values) : Matrix(values, { 1, (std::int64_t)values.size() }) {}


	template <typename T>
	Matrix<T>::Matrix(std::vector<T> values, Dimensions shape)
		: data(values)
		, dims(shape)
	{
		this->data.shrink_to_fit(); //Data's size will not change
	}

	template <typename T>
	const std::vector<T>& Matrix<T>::Data() const
	{
		return this->data;
	}

	template <typename T>
	const Dimensions& Matrix<T>::Shape()
	{
		return this->dims;
	}

	//TODO: Potential bug constexpr
	template <typename T>
	constexpr std::int64_t Matrix<T>::TotalElementCount()
	{
		return this->data.size();
	}

	template <typename T>
	constexpr std::int64_t Matrix<T>::RowCount()
	{
		return this->dims.first;
	}

	template <typename T>
	constexpr std::int64_t Matrix<T>::ColumnCount()
	{
		return this->dims.second;
	}



	template<typename T>
	Matrix<T>& pulse::Matrix<T>::operator*=(const Matrix<T>& rhs)
	{
		*this = (*this * rhs);
		return *this;
	}

	//TODO: improve non AVX code (AKA Modern C++ numerics features)
	template<typename T>
	Matrix<T> operator*(const Matrix<T>& lhs, const Matrix<T>& rhs)
	{

		if (lhs.dims.second != rhs.dims.first)
		{
			throw std::domain_error("The number of columns in Matrix A must \
				match the number of rows in Matrix B");
		}

		std::int64_t newRowCount = lhs.dims.first;
		std::int64_t newColumnCount = rhs.dims.second;

		//Zero init output
		std::vector<T> mout(newRowCount * newColumnCount, 0.f);

		//#ifdef USE_AVX_INTRIN
			//
		//#else

		//Assert lhs.dims.second == rhs.dims.first

		for (std::int64_t i = 0; i < lhs.dims.first; i++)
		{
			for (std::int64_t j = 0; j < lhs.dims.second; j++)
			{
				for (std::int64_t k = 0; k < rhs.dims.second; k++)
				{
					mout[i * rhs.dims.second + k] +=
						lhs[i * lhs.dims.second + j] * rhs[j * rhs.dims.second + k];
				}
			}
		}

		//#endif

		return Matrix<T>(mout, { newRowCount, newColumnCount });
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs)
	{
		//#ifdef USE_AVX_INTRIN
			//
		//#else

		for (std::int64_t i = 0; i < this->dims.first; i++)
		{
			for (std::int64_t j = 0; j < this->dims.second; j++)
			{
				(*this)[i * this->dims.second + j] += rhs[i * this->dims.second + j];
			}
		}

		//#endif

		return *this;
	}

	template<typename T>
	Matrix<T> operator+(Matrix<T> lhs, const Matrix<T>& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator*=(const T& scalar)
	{
		for (auto it = this->data.begin(); it != this->data.end(); ++it)
		{
			*it *= scalar;
		}

		return *this;
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator+=(const T& scalar)
	{
		for (auto it = this->data.begin(); it != this->data.end(); ++it)
		{
			*it += scalar;
		}

		return *this;
	}


	template<typename T>
	Matrix<T> operator*(const T& scalar, Matrix<T> vector)
	{
		vector *= scalar;
		return vector;
	}

	template<typename T>
	Matrix<T> operator+(const T& scalar, Matrix<T> vector)
	{
		vector += scalar;
		return vector;
	}


	template<typename T>
	std::vector<T> operator*(const std::vector<T>& lhs, const Matrix<T>& rhs)
	{
		if (lhs.size() != rhs.dims.first)
		{
			throw std::domain_error("The size of the vector must \
				match the number of rows in the Matrix");
		}

		//Zero init output
		std::vector<T> mout(rhs.dims.second, 0.f); //TODO: Make zero for hash_type (aka int vs float vs ..)

		//#ifdef USE_AVX_INTRIN
			//
		//#else

		for (std::int64_t j = 0; j < rhs.dims.first; j++)
		{
			for (std::int64_t k = 0; k < rhs.dims.second; k++)
			{
				mout[k] += lhs[j] * rhs[j * rhs.dims.second + k];
			}

		}

		return mout;
	}

	//for now we'll cheat but next iteration TODO: add reference to transpose data 
	//(which will index differently) aka from whiteboard!
	template<typename T>
	std::vector<T> operator*(const Matrix<T>& lhs, const std::vector<T>& rhs)
	{
		if (lhs.dims.second != rhs.size())
		{
			throw std::domain_error("The size of the vector must \
				match the number of columns in the Matrix");
		}

		//Zero init output
		std::vector<T> mout(lhs.dims.first, 0.f); //TODO: Make zero for hash_type (aka int vs float vs ..)

		//#ifdef USE_AVX_INTRIN
			//
		//#else

		for (std::int64_t j = 0; j < lhs.dims.first; j++)
		{
			for (std::int64_t k = 0; k < lhs.dims.second; k++)
			{
				mout[j] += lhs[j * lhs.dims.second + k] * rhs[k];
			}

		}

		return mout;
	}

	//technically both T's need not be identical
	//TODO: May be unacceptable!!
	template<typename T>
	std::vector<T>& operator*=(std::vector<T>& lhs, const std::vector<T>& rhs)
	{
		if (lhs.size() != rhs.size())
		{
			throw std::domain_error("The size of Vector A must \
				match the size of Vector B");
		}

		//#ifdef USE_AVX_INTRIN
			//
		//#else

		for (std::int64_t k = 0; k < lhs.size(); k++)
		{
			lhs[k] *= rhs[k];
		}

		return lhs;
	}

	//technically both T's need not be identical
//TODO: May be unacceptable!!
	template<typename T>
	std::vector<T> operator*(std::vector<T> lhs, const std::vector<T>& rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	//I think I can move this to the .cpp :)
	//TODO: AVX implemntation
	//in1 is [m * 1] and  in2 is [1 * n]. out is [m * n]
	//Can't be mutable as in1 nd in2 are reused (double looped) therefore
	//overwriting would change values.
	// 
	// Should bin op be a functional object?????
	template<typename T, typename U, typename V, typename BinOp>
	Matrix<T> Broadcast(const Matrix<U>& lhs, const Matrix<V>& rhs, BinOp binary_op)
	{
		std::int64_t rowCount = lhs.RowCount();
		std::int64_t columnCount = rhs.ColumnCount();

		std::vector<T> out(rowCount * columnCount);
		for (int64_t i = 0; i < rowCount; i++)
		{
			for (int64_t j = 0; j < columnCount; j++)
			{
				//TODO: make more efficiant
				out[i * columnCount + j] = binary_op(lhs[{i, 0}], rhs[{0, j}]);
			}
		}
		return Matrix<T>(out, Dimensions(rowCount, columnCount));
	}

	template<typename T, typename U, typename V, typename BinOp>
	Matrix<T> Broadcast(const std::vector<U>& lhs, const std::vector<V>& rhs, BinOp binary_op)
	{
		std::int64_t rowCount = lhs.size();
		std::int64_t columnCount = rhs.size();

		std::vector<T> out(rowCount * columnCount);
		for (int64_t i = 0; i < rowCount; i++)
		{
			for (int64_t j = 0; j < columnCount; j++)
			{
				//TODO: make more efficiant
				out[i * columnCount + j] = binary_op(lhs[i], rhs[j]);
			}
		}
		return Matrix<T>(out, Dimensions(rowCount, columnCount));
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
