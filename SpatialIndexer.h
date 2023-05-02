#pragma once
class SpatialIndexer
{
public:

	static  int IndexFromPosition(int level, int index, int baseLength, int iterations);

private:

	static int GetIndex(int level, int index, int baseLength, int maxIndex);
};

