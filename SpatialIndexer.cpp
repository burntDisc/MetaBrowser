#include "SpatialIndexer.h"
#include <vector>
#include <iostream>


SpatialIndexer::LongCoord SpatialIndexer::LongCoord::operator-(const LongCoord& coord) const
{
	return
	{
		x - coord.x,
		y - coord.y
	};
}

long long int SpatialIndexer::LongCoord::length()
{
	return sqrt(x * x + y * y);
}


bool SpatialIndexer::CheckTriangle(SpatialIndexer::LongCoord vA, SpatialIndexer::LongCoord vB, SpatialIndexer::LongCoord vC, SpatialIndexer::LongCoord coord, long long int sideLength)
{
	long long int distA = (vA - coord).length();
	long long int distB = (vB - coord).length();
	long long int distC = (vC - coord).length();

	return (distA < sideLength &&
			distB < sideLength &&
			distC < sideLength);
}

long long int SpatialIndexer::GetIndex(long long int sideLength, SpatialIndexer::LongCoord origin, SpatialIndexer::LongCoord coord, LongCoord reflectionFactor)
{
	// space filling curve type beat
	// https://www.youtube.com/watch?v=pw_50szQfA0
	//				  5 
	//                /\
	//               /  \
	//              /  D \
	//            2/______\4
	//            /\      /\
	//           /  \  B /  \
	//          /  A \  /  C \
	//        0/______\/______\3
	//                1

	long long int height = sideLength * sqrt(3) / 2;
	long long int volume = sideLength * height / 2;


	SpatialIndexer::LongCoord v0 = origin;

	SpatialIndexer::LongCoord v1 = 
	{ 
		origin.x + sideLength / 2 * reflectionFactor.x, 
		origin.y 
	};

	SpatialIndexer::LongCoord v2 =
	{
		origin.x + sideLength / 4 * reflectionFactor.x,
		origin.y + height / 2 * reflectionFactor.y
	};

	SpatialIndexer::LongCoord v3 =
	{
		origin.x + sideLength * reflectionFactor.x,
		origin.y
	};

	SpatialIndexer::LongCoord v4 = 
	{
		origin.x + sideLength * 3 / 4 * reflectionFactor.x,
		origin.y + height / 2 * reflectionFactor.y
	};

	SpatialIndexer::LongCoord v5 =
	{
		origin.x + sideLength / 2 * reflectionFactor.x,
		origin.y + height * reflectionFactor.y
	};

	if (CheckTriangle(v0, v1, v2, coord, sideLength / 2))
		return
		volume > 4 ?
		GetIndex(sideLength / 2, origin, coord, reflectionFactor) :
		0;

	else if (CheckTriangle(v2, v4, v1, coord, sideLength / 2))
		return
		volume > 4 ?
		volume / 4 + GetIndex(sideLength / 2, v2, coord, { reflectionFactor.x,-reflectionFactor.y }) :
		1;

	else if (CheckTriangle(v1, v3, v4, coord, sideLength / 2))
		return
		volume > 4 ?
		volume / 2 + GetIndex(sideLength / 2, v1, coord, reflectionFactor) :
		2;
	else// (CheckTriangle(v4, v2, v5, coord, sideLength / 2))
		return
		volume > 4 ?
		3 * volume / 4 + GetIndex(sideLength / 2, v4, coord, { -reflectionFactor.x, reflectionFactor.y }) :
		3;
}
 
long long int SpatialIndexer::IndexFromCoord(SpatialIndexer::LongCoord coord, long long int sideLength)
{
	return GetIndex(sideLength, { 0,0 }, coord, { 1,1 });
}