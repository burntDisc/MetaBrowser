#pragma once
class MapGen
{
private:
	enum Orientation {unchanged, verticalFlip, horizontalFlip};

	struct longCoord
	{
		long long int x;
		long long int y;

	};

	struct Triangle
	{
		longCoord v0;
		longCoord v1;
		longCoord v2;
	};
public:

	bool CheckTriangle(MapGen::Triangle triangle, MapGen::longCoord coord);

	long long int GetQuadrantBounds(long long int sideLength, MapGen::longCoord origin, MapGen::longCoord coord, longCoord reflectionFactor);


};

