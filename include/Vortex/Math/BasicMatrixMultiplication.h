#pragma once
#include "BasicVector.h"
#include "BasicMatrix.h"

namespace Vortex::Math {
	template<typename T, SizeType Rows, SizeType Columns>
	constexpr BasicVector<T, Columns> operator*(
		const BasicMatrix<T, Rows, Columns>& matrix,
		const BasicVector<T, Columns>& vector
	) {

		BasicVector<T, Columns> out;
		for (SizeType i = 0; i < Rows; i++) {
			for (SizeType j = 0; j < Columns; j++) {
				out[i] += matrix(i, j) * vector[j];
			}
		}
		return out;
	}

	template<typename T, SizeType Rows, SizeType Columns>
	constexpr BasicVector<T, Columns> operator*(
		const BasicVector<T, Columns>& vector,
		const BasicMatrix<T, Rows, Columns>& matrix
	) {
		return matrix * vector;
	}

	template<typename T, SizeType M, SizeType S, SizeType N>
	constexpr BasicMatrix<T, M, N> operator*(
		const BasicMatrix<T, M, S>& lhs,
		const BasicMatrix<T, S, N>& rhs
	) {

		//a is m x s
		//b is s x n
		//r is m x n
		//for (i = 1 to m)
		//   for (j = 1 to n)
		//      result(i, j) = 0
		//      for (k = 1 to s)
		//         result(i, j) += a(i, k) * b(k, j)

		BasicMatrix<T, M, N> out;
		for (SizeType i = 0; i < M; i++) {
			for (SizeType j = 0; j < N; j++) {

				out(i, j) = 0;
				for (int k = 0; k < S; k++) {
					out(i, j) += lhs(i, k) * rhs(k, j);
				}
			}
		}
		return out;
	}
}