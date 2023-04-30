#pragma once
class SpatialIndexer
{
public:

	struct LongCoord
	{
		long long int x;
		long long int y;

		LongCoord operator-(const LongCoord& coord) const;
		long long int length();
	};

	long long int IndexFromCoord(SpatialIndexer::LongCoord coord, long long int sideLength);

private:
	bool CheckTriangle(SpatialIndexer::LongCoord vA, SpatialIndexer::LongCoord vB, SpatialIndexer::LongCoord vC, SpatialIndexer::LongCoord coord, long long int sideLength);

	long long int GetIndex(long long int sideLength, SpatialIndexer::LongCoord origin, SpatialIndexer::LongCoord coord, LongCoord reflectionFactor);

};

