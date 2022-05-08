#include "DataStructures/UnionFind.h"

void IntUnionFind::Union(int32 x, int32 y)
{
	int32 rootX = Find(x);
	int32 rootY = Find(y);
	if (rootX != rootY) {
		if (rank[rootX] > rank[rootY]) {
			root[rootY] = rootX;
		}
		else if (rank[rootX] < rank[rootY]) {
			root[rootX] = rootY;
		}
		else {
			root[rootY] = rootX;
			rank[rootX] += 1;
		}
	}
}