#pragma once

#include <vector>

/*
* MAX HEAP
*/
namespace CommonUtilities
{
	template <class T>
	class Heap
	{
	public:
		int GetSize() const; //returnerar antal element i heapen
		void Enqueue(const T& aElement); //lägger till elementet i heapen
		const T& GetTop() const; //returnerar det största elementet i heapen
		T Dequeue(); //tar bort det största elementet ur heapen och returnerar det

	private:
		void BubbleUp(const int& aIndex);
		void BubbleDown(const int& aIndex);

		std::vector<T> myArray;
	};
	template<class T>
	inline int Heap<T>::GetSize() const
	{
		return myArray.size();
	}
	template<class T>
	inline void Heap<T>::Enqueue(const T& aElement)
	{
		myArray.emplace_back(aElement);
		BubbleUp(myArray.size() - 1);
	}

	template<class T>
	inline const T& Heap<T>::GetTop() const
	{
		return myArray[0];
	}

	template<class T>
	inline T Heap<T>::Dequeue()
	{
		T val = myArray[0];
		myArray[0] = myArray.back();
		myArray.pop_back();
		BubbleDown(0);

		return val;
	}

	template<class T>
	inline void Heap<T>::BubbleUp(const int& aIndex)
	{
		if (aIndex == 0)
		{
			return;
		}

		if (myArray[(aIndex - 1) / 2] < myArray[aIndex])
		{
			T copy = myArray[(aIndex - 1) / 2];
			myArray[(aIndex - 1) / 2] = myArray[aIndex];
			myArray[aIndex] = copy;
			BubbleUp((aIndex - 1) / 2);
		}
	}

	template<class T>
	inline void Heap<T>::BubbleDown(const int& aIndex)
	{
		const int leftChild = 2 * aIndex + 1;
		const int rightChild = 2 * aIndex + 2;

		int largest = aIndex;

		if (leftChild < myArray.size() && myArray[largest] < myArray[leftChild])
		{
			largest = leftChild;
		}

		if (rightChild < myArray.size() && myArray[largest] < myArray[rightChild])
		{
			largest = rightChild;
		}

		if (largest != aIndex)
		{
			T copy = myArray[aIndex];
			myArray[aIndex] = myArray[largest];
			myArray[largest] = copy;
			BubbleDown(largest);
		}
	}
}