#include "Math/Vector2D.h"

#define SIZE 12  //assumes size is a multiple of 4 because MMX and SSE registers will store 4 elements.

Engine::Math::Vector2D::Vector2D()
{
	x = 0;
	y = 0;
}

Engine::Math::Vector2D::Vector2D(float X, float Y)
{
	x = X;
	y = Y;
}

float Engine::Math::Vector2D::Magnitude()
{
	return sqrt(this->x*this->x + this->y * this->y);
}

float Engine::Math::Vector2D::DotProduct_SSE(float a, float b)
{
	__m128 A = _mm_set1_ps(a);
	__m128 B = _mm_set1_ps(b);
	__m128 mulRes, shufReg, sumsReg;
	mulRes = _mm_mul_ps(A, B);

	shufReg = _mm_movehdup_ps(mulRes);
	sumsReg = _mm_add_ps(mulRes, shufReg);
	shufReg = _mm_movehl_ps(shufReg, sumsReg);
	sumsReg = _mm_add_ss(sumsReg, shufReg);
	return  _mm_cvtss_f32(sumsReg);
}

float Engine::Math::Vector2D::CrossProduct_SSE(float a, float b)
{
	__m128 vec0 = _mm_set1_ps(a);
	__m128 vec1 = _mm_set1_ps(b);

	__m128 tmp0 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 0, 2, 1));
	__m128 tmp1 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 tmp2 = _mm_shuffle_ps(vec0, vec0, _MM_SHUFFLE(3, 1, 0, 2));
	__m128 tmp3 = _mm_shuffle_ps(vec1, vec1, _MM_SHUFFLE(3, 0, 2, 1));
	return _mm_cvtss_f32(_mm_sub_ps(
		_mm_mul_ps(tmp0, tmp1),
		_mm_mul_ps(tmp2, tmp3)));
}

Engine::Math::Vector2D Engine::Math::Vector2D::operator/ (float other)
{
	this->x = this->x / other;
	this->y = this->y / other;

	return *this;
}
