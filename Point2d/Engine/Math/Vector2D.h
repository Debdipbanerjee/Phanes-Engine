#pragma once
#include <math.h>
#include <pmmintrin.h>

namespace Engine
{
	namespace Math
	{

		class Vector2D
		{

		private:

		public:

			float x, y;
			
			Vector2D();
			Vector2D(float X, float Y);
			float Magnitude();

			// SSE Dot product
			static float DotProduct_SSE(float a, float b);

			// SSE Cross product
			static float CrossProduct_SSE(float a, float b);

			//operator overloading
			Vector2D operator/ (float other);

		};

		
	}
}