#pragma once

namespace Engine
{
	namespace Math
	{
		class  Vector4D
		{
		public:
			float x, y, z, w;

			// constructors
			Vector4D();
			Vector4D(float X, float Y, float Z, float W);

			// operator overloading
			Vector4D operator+ (Vector4D& p_other);
			Vector4D operator- (Vector4D& p_other);

		private:

		};
	}
}