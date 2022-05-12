#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <utility>
//#include "PriorityQueue.generated.h"


template<class T>
class /*GRAPHGAME_API*/ TMinPriorityQueue
{
	/*GENERATED_BODY()*/
public:
	void Insert(T val, int32 p);
	FORCEINLINE bool IsEmpty() { return v.Num() == 0; }
	void Empty() { v.Empty(); }
	T GetMin();
	void PopMin();
	/*Currently takes O(N) time
	https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Running_time
	To perform decrease-key steps in a binary heap efficiently, 
	it is necessary to use an auxiliary data structure that maps each vertex to its position in the heap,
	and to keep this structure up to date as the priority queue Q changes. 
	*/
	void DecreasePriority(const T& x, int32 NewP);
	int32 Find(const T& x);
	int32 GetPriority(int32 i);
private:
	TArray<std::pair<T, int32>> v;
	TMap<T, int32> map;
	inline int32 Parent(int32 i) { return i / 2; }
	inline int32 Left(int32 i) { return i * 2; }
	inline int32 Right(int32 i) { return i * 2 + 1; }
	
	//Let the value v[i] float down the heap, and recursively maintain the heap property on v[i] subtrees
	void MinHeapifyDown(int32 i);
	void MinHeapifyUp(int32 i);

	int32 OldFind(const T& x);
};

template<class T>
int32 TMinPriorityQueue<T>::GetPriority(int32 i)
{
	ensure(v.IsValidIndex(i));
	return v[i].second;
}

template<class T>
int32 TMinPriorityQueue<T>::Find(const T& x)
{
	int32 i = map.Contains(x) ? map[x] : -1;

	////#TODO REMOVE
	//int32 oldI = OldFind(x);
	//ensure(oldI == i);
	////----

	return i;
}

template<class T>
int32 TMinPriorityQueue<T>::OldFind(const T& x)
{
	int32 i = 0;
	while (i < v.Num())
	{
		if (v[i].first == x)
			return i;
		i++;
	}
	return -1;
}

template<class T>
void TMinPriorityQueue<T>::DecreasePriority(const T& x,int32 NewP)
{
	int32 i = Find(x);
	ensure(i >= 0);

	if (v[i].second <= NewP)
		return;
	v[i].second = NewP;
	MinHeapifyUp(i);
}

template<class T>
void TMinPriorityQueue<T>::PopMin()
{
	ensure(!IsEmpty());
	//DELETE-MAP
	map.Remove(v[0].first);

	v[0] = v[v.Num() - 1];
	v.Pop();

	//RE-MAP
	if(v.Num() > 0)
		map[v[0].first] = 0;

	MinHeapifyDown(0);
}

template<class T>
T TMinPriorityQueue<T>::GetMin()
{
	return v[0].first;
}

template<class T>
void TMinPriorityQueue<T>::Insert(T val, int32 p)
{
	std::pair<T, int32> NewPair = std::make_pair(val, p);
	v.Push(NewPair);

	//ADD-MAP
	map.Add(val, v.Num() - 1);

	MinHeapifyUp(v.Num() - 1);
}

template<class T>
void TMinPriorityQueue<T>::MinHeapifyUp(int32 i)
{
	int32 pi = Parent(i);
	while (i > 0 && v[pi].second > v[i].second) {
		//RE-MAP
		map[v[i].first] = pi;
		map[v[pi].first] = i;

		std::swap(v[i], v[pi]);
		i = pi;
		pi = Parent(i);
	}
}

template<class T>
void TMinPriorityQueue<T>::MinHeapifyDown(int32 i)
{
	int32 l = Left(i);
	int32 r = Right(i);
	int32 Smallest = i;
	if (l < v.Num() && v[l].second < v[i].second)
		Smallest = l;

	if (r < v.Num() && v[r].second < v[Smallest].second)
		Smallest = r;

	if (Smallest != i) {
		//RE-MAP
		map[v[i].first] = Smallest;
		map[v[Smallest].first] = i;

		std::swap(v[i], v[Smallest]);

		MinHeapifyDown(Smallest);
	}
}
