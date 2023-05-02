#include "SpatialIndexer.h"
#include <cmath>

int SpatialIndexer::GetIndex(int level, int index, int baseLength, int maxIndex)
{
	int levelsFromMid = (baseLength / 2) - level;
	if (maxIndex == 0)
	{
		return 0;
	}
	else if (level >= baseLength / 2)
	{
		int adjustedLevel = level - baseLength / 2;
		int adjustedIndex = (adjustedLevel * 2) - index;
		return 3 * maxIndex / 4 + GetIndex(adjustedLevel, adjustedIndex, baseLength / 2, maxIndex / 4);
	}
	else if (index < levelsFromMid * 2 + 1)
	{
		return GetIndex(level, index, baseLength / 2, maxIndex / 4);
	}
	else if (index < baseLength / 2)
	{
		return maxIndex / 4 + GetIndex(baseLength/2-level, index, baseLength / 2, maxIndex / 4);
	}
	else
	{
		return maxIndex / 2 + GetIndex(level, index - baseLength / 2, baseLength / 2, maxIndex / 4);
	}
}
 
int SpatialIndexer::IndexFromPosition(int level, int index, int baseLength, int iterations)
{
	int maxIndex = pow(4, iterations);
	return GetIndex(level, index, baseLength, maxIndex);
}