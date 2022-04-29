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
	void DecreasePriority(T x, int32 NewP);
private:
	TArray<std::pair<T, int32>> v;
	inline int32 Parent(int32 i) { return i / 2; }
	inline int32 Left(int32 i) { return i * 2; }
	inline int32 Right(int32 i) { return i * 2 + 1; }
	
	//Let the value v[i] float down the heap, and recursively maintain the heap property on v[i] subtrees
	void MinHeapifyDown(int32 i);
	void MinHeapifyUp(int32 i);
};

template<class T>
void TMinPriorityQueue<T>::DecreasePriority(T x,int32 NewP)
{
	int32 i = 0;
	while (i < v.Num())
	{
		if (v[i].first == x)
			break;
		i++;
	}
	
	if (v[i].second <= NewP)
		return;
	v[i].second = NewP;
	MinHeapifyUp(i);
}

template<class T>
void TMinPriorityQueue<T>::PopMin()
{
	ensure(!IsEmpty());
	v[0] = v[v.Num() - 1];
	v.Pop();
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
	MinHeapifyUp(v.Num() - 1);
}

template<class T>
void TMinPriorityQueue<T>::MinHeapifyUp(int32 i)
{
	int32 pi = Parent(i);
	while (i > 0 && v[pi].second > v[i].second) {
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
		std::swap(v[i], v[Smallest]);
		MinHeapifyDown(Smallest);
	}
}
