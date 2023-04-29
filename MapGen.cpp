#include "MapGen.h"
#include <vector>


bool MapGen::CheckTriangle(MapGen::Triangle triangle, MapGen::longCoord coord)
{
	return true;
}

long long int MapGen::GetQuadrantBounds(long long int sideLength, MapGen::longCoord origin, MapGen::longCoord coord, longCoord reflectionFactor)
{
	// https://www.youtube.com/watch?v=pw_50szQfA0
	//				  5 
	//                /\
	//               /  \
	//              /  D \
	//            2/______\4
	//            /\      /\
	//           /  \ B  /  \
	//          /  A \  /  C \
	//        0/______\/______\3
	//                1

	long long int height = sideLength * sqrt(3) / 2;
	long long int volume = sideLength * height / 2;

	if (volume < 4)
	{
		return 1;
	}

	MapGen::longCoord v0 = origin;
	MapGen::longCoord v1 = 
	{ 
		origin.x + sideLength / 2 * reflectionFactor.x, 
		origin.y 
	};

	MapGen::longCoord v2 =
	{
		origin.x + sideLength / 4 * reflectionFactor.x,
		origin.y + height / 2 * reflectionFactor.y
	};

	MapGen::longCoord v3 =
	{
		origin.x + sideLength * reflectionFactor.x,
		origin.y
	};

	MapGen::longCoord v4 = 
	{
		origin.x + sideLength * 3 / 4 * reflectionFactor.x,
		origin.y + height / 2 * reflectionFactor.y
	};

	MapGen::longCoord v5 =
	{
		origin.x + sideLength / 2 * reflectionFactor.x,
		origin.y + height * reflectionFactor.y
	};

	Triangle triA = { v0,v1,v2 };
	Triangle triB = { v2,v4,v1 };
	Triangle triC = { v1,v3,v4 };
	Triangle triD = { v4,v2,v5 };


	if (volume > 4)
	{
		if (CheckTriangle(triA, coord))
			return GetQuadrantBounds(sideLength / 2, origin, coord, reflectionFactor);

		else if (CheckTriangle(triB, coord))
			return volume / 4 + GetQuadrantBounds(sideLength / 2, v2, coord, { reflectionFactor.x,-reflectionFactor.y });

		else if (CheckTriangle(triC, coord))
			return volume / 2 + GetQuadrantBounds(sideLength / 2, v1, coord, reflectionFactor);

		else if (CheckTriangle(triD, coord))
			return 3 * volume / 4 + GetQuadrantBounds(sideLength / 2, v4, coord, { -reflectionFactor.x, reflectionFactor.y });
	}
}
