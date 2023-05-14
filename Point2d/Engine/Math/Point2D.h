#pragma once


namespace Engine
{
	namespace Math
	{
		class Point2d {
		public:
			float posX, posY;

			Point2d()
			{
				posX = 0;
				posY = 0;
			}

			Point2d(float X, float Y)
			{
				posX = X;
				posY = Y;
			}
		};
	}
}
