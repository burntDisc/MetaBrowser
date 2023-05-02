#include "SpatialIndexer.h"
#include <cmath>
//https://www.youtube.com/watch?v=pw_50szQfA0
int SpatialIndexer::GetIndex(int level, int index, int baseLength, int maxIndex)
{
	int levelsFromMid = (baseLength / 2) - level - 1;
	if (maxIndex == 0)
	{
		return 0;
	}
	else if (baseLength / 2 <= level)
	{
		//top flipped horizontal
		int adjustedLevel = level - baseLength / 2;
		int adjustedIndex = (adjustedLevel * 2) - index;
		return 3 * (maxIndex + 1) / 4 + GetIndex(adjustedLevel, adjustedIndex, baseLength / 2, maxIndex / 4);
	}
	else if (index < levelsFromMid * 2 + 1)
	{ // left
		return GetIndex(level, index, baseLength / 2, maxIndex / 4);
	}
	else if (index < baseLength)
	{
		//middle flipped vertical
		return (maxIndex + 1) / 4 + GetIndex(baseLength/2-level, index, baseLength / 2, maxIndex / 4);
	}
	else
	{ // right
		return (maxIndex + 1) / 2 + GetIndex(level, index - baseLength, baseLength / 2, maxIndex / 4);
	}
}
 
int SpatialIndexer::IndexFromPosition(int level, int index, int baseLength, int iterations)
{
	int maxIndex = pow(4, iterations) - 1;
	int spatialIndex =  GetIndex(level, index, baseLength, maxIndex);
	return spatialIndex;
}