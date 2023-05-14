#include "Vector4D.h"

Engine::Math::Vector4D::Vector4D()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Engine::Math::Vector4D::Vector4D(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}

Engine::Math::Vector4D Engine::Math::Vector4D::operator+(Vector4D& p_other)
{
	return Vector4D(w + p_other.w, x + p_other.x, y + p_other.y, z + p_other.z);
}

Engine::Math::Vector4D Engine::Math::Vector4D::operator-(Vector4D& p_other)
{
	return Vector4D(w - p_other.w, x - p_other.x, y - p_other.y, z - p_other.z);
}