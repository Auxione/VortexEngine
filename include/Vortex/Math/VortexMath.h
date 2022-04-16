#pragma once

#include "Vortex/Math/VortexMathConstants.h"
#include "Vortex/Math/VortexMathFunctions.h"
#include "Vortex/Math/BasicAngle.h"
#include "Vortex/Math/BasicVector.h"
#include "Vortex/Math/BasicMatrix.h"
#include "Vortex/Math/BasicMatrixMultiplication.h"
#include "Vortex/Math/BasicMatrixInvert.h"

#include "Vortex/Math/BasicRectangle.h"

#include "Vortex/Math/Color.h"
#include "Vortex/Math/Random.h"

namespace Vortex::Math {
	using Angle = BasicAngle<float>;

	using Vector2Int = BasicVector<Int32, 2>;
	using Vector3Int = BasicVector<Int32, 3>;
	using Vector4Int = BasicVector<Int32, 4>;

	using Vector2 = BasicVector<float, 2>;
	using Vector3 = BasicVector<float, 3>;
	using Vector4 = BasicVector<float, 4>;
	using Matrix2 = BasicMatrix<float, 2, 2>;
	using Matrix3 = BasicMatrix<float, 3, 3>;
	using Matrix4 = BasicMatrix<float, 4, 4>;

	using Rectangle = BasicRectangle<float>;
	using RectangleInt = BasicRectangle<Int32>;
}