#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <utility>

class IntUnionFind {
public:

	FORCEINLINE void Init(int32 sz)
	{
		for (int32 i = 0; i < sz; i++) {
			root.Add(i);
			rank.Add(1);
		}
	}

	FORCEINLINE void Empty()
	{
		root.Empty();
		rank.Empty();
	}

	FORCEINLINE int32 Find(int32 x) {
		if (x == root[x]) {
			return x;
		}
		return root[x] = Find(root[x]);
	}

	void Union(int32 x, int32 y);

	FORCEINLINE bool AreConnected(int32 x, int32 y) {
		return Find(x) == Find(y);
	}

	//To count roots i could count the elements where
	//root[i] == i
private:
	TArray<int32> root;
	TArray<int32> rank;
};

//template<class T>
//class /*GRAPHGAME_API*/ TUnionFind
//{
//	/*GENERATED_BODY()*/
//public:
//	UnionFind(T sz);
//
//	T Find(T x);
//
//	void Union(T x, T y);
//
//	bool AreConnected(T x, T y);
//
//	//To count roots i could count the elements where
//	//root[i] == i
//
//	static void Test();
//private:
//	std::vector<T> root;
//	std::unordered_map<T,int32> rank;
//};
//
//template<class T>
//void TUnionFind<T>::Test()
//{
//	int32 X = 32;
//	UnionFind<int32> u(X);
//	for (int32 i = 1; i < X; i+=2)
//	{
//		u.Union(i, i - 1);
//		ensure(find)
//	}
//}
//
////CTOR
//template<class T>
//TUnionFind<T>::UnionFind(const std::vector<T>& v)
//	: root(v)
//{
//	for (int32 i = 0; i < v.size(); i++) {
//		root[i] = i;
//		rank[i] = 1;
//	}
//}
//
//template<class T>
//T TUnionFind<T>::Find(T x)
//{
//	if (x == root[x]) {
//		return x;
//	}
//	return root[x] = Find(root[x]);
//}
//
//
//
//template<class T>
//void TUnionFind<T>::Union(T x, T y)
//{
//	T rootX = Find(x);
//	T rootY = Find(y);
//	if (rootX != rootY) {
//		if (rank[rootX] > rank[rootY]) {
//			root[rootY] = rootX;
//		}
//		else if (rank[rootX] < rank[rootY]) {
//			root[rootX] = rootY;
//		}
//		else {
//			root[rootY] = rootX;
//			rank[rootX] += 1;
//		}
//	}
//}
//
//template<class T>
//bool TUnionFind<T>::AreConnected(T x, T y)
//{
//	return Find(x) == Find(y);
//}



