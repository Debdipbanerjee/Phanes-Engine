#pragma once
#include <array>
#include <cmath>
#include <math.h>

#include "Vector4D.h"

namespace Engine
{
	namespace Math
	{
		class Matrix
		{
		private:
			float	m11, m12, m13, m14,
					m21, m22, m23, m24,
					m31, m32, m33, m34,
					m41, m42, m43, m44;
			
			// default constructor
			Matrix();
			void MatrixScalarMultiply(float num);
			void InverseOperations(Matrix& Temp) const;

		public:
			
			// value based constructor
			Matrix(float i11, float i12, float i13, float i14,
				float i21, float i22, float i23, float i24,
				float i31, float i32, float i33, float i34,
				float i41, float i42, float i43, float i44) :
				m11(i11), m12(i12), m13(i13), m14(i14),
				m21(i21), m22(i22), m23(i23), m24(i24),
				m31(i31), m32(i32), m33(i33), m34(i34),
				m41(i41), m42(i42), m43(i43), m44(i44)
			{};

			// copy constructor
			Matrix(const Matrix& other)
			{
				m11 = other.m11;
				m12 = other.m12;
				m13 = other.m13;
				m14 = other.m14;
				m21 = other.m21;
				m22 = other.m22;
				m23 = other.m23;
				m24 = other.m24;
				m31 = other.m31;
				m32 = other.m32;
				m33 = other.m33;
				m34 = other.m34;
				m41 = other.m41;
				m42 = other.m42;
				m43 = other.m43;
				m44 = other.m44;
			}

			// identity matrix
			static Matrix IdentityMatrix();

			// matrix Operations
			float MultiplyRowColumn(float r_1, float r_2, float r_3, float r_4, float c_1, float c_2, float c_3, float c_4);
			void TransposeMatrix();
			Matrix GetNewTransposeMatrix() const;
			void InvertMatrix();
			Matrix GetNewInvertMatrix() const;
			Vector4D VectorRightMulitply(const Vector4D& other);
			Vector4D VectorLeftMulitply(const Vector4D& other);

			// operator overloading
			Matrix operator*(const Matrix& m_other);	//	Matrix Multiplication
			Matrix& operator=(const Matrix& m_other);

			// rotation
			static Matrix CreateRotationX(float RadiansInX);
			static Matrix CreateRotationY(float RadiansInY);
			static Matrix CreateRotationZ(float RadiansInZ);

			// translation
			static Matrix CreateTranslation(float x, float y, float z);

			// scale
			static Matrix CreateScale(float scaleX, float scaleY, float scaleZ);
		};
	}
}