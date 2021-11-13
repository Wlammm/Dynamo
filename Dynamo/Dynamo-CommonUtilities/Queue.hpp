#pragma once

#include <vector>
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class Queue
	{
	public:
		//Skapar en tom kö
		Queue();
		//Returnerar antal element i kön
		int GetSize() const;
		//Returnerar elementet längst fram i kön. Kraschar med en assert om kön är
		//tom
		const T& GetFront() const;
		//Returnerar elementet längst fram i kön. Kraschar med en assert om kön är
		//tom
		T& GetFront();
		//Lägger in ett nytt element längst bak i kön
		void Enqueue(const T& aValue);
		//Tar bort elementet längst fram i kön och returnerar det. Kraschar med en
		//assert om kön är tom.
		T Dequeue();

		// Kollar ifall kön är tom.
		const bool& IsEmpty() const;

	private:
		void Grow();
		void Fill(const int& aStartIndex);

		std::vector<T> myVector;
		int myWriteIndex = 0;
		int myReadIndex = 0;
		int mySize = 0;
	};

	template<class T>
	inline const bool& Queue<T>::IsEmpty() const
	{
		return mySize == 0;
	}

	template<class T>
	inline Queue<T>::Queue()
	{
		myVector.reserve(8);
		Fill(0);
	}

	template<class T>
	inline int Queue<T>::GetSize() const
	{
		return mySize;
	}
	
	template<class T>
	inline const T& Queue<T>::GetFront() const
	{
		assert(GetSize() > 0 && "Queue is empty.");
		return myVector[myReadIndex];
	}
	
	template<class T>
	inline T& Queue<T>::GetFront()
	{
		assert(GetSize() > 0 && "Queue is empty.");
		return myVector[myReadIndex];
	}
	
	template<class T>
	inline void Queue<T>::Enqueue(const T& aValue)
	{
		if (myWriteIndex + 1 == myReadIndex || myWriteIndex % 8 == 0 && myWriteIndex == myVector.size() && myReadIndex == 0)
		{
			Grow();
		}
		else if (myWriteIndex % 8 == 0 && myWriteIndex == myVector.size() && myReadIndex > 0)
		{
			myWriteIndex = 0;
		}

		myVector[myWriteIndex] = aValue;
		myWriteIndex++;
		mySize++;
	}
	
	template<class T>
	inline T Queue<T>::Dequeue()
	{
		assert(GetSize() > 0 && "Queue is empty");

		if (myReadIndex == myVector.size())
		{
			myReadIndex = 0;
		}

		mySize--;
		return myVector[myReadIndex++];
	}

	template<class T>
	inline void Queue<T>::Grow()
	{
		myVector.reserve(myVector.size() * 2);
		Fill(myVector.size());
		myWriteIndex = myVector.size() * .5f;
	}
	template<class T>
	inline void Queue<T>::Fill(const int& aStartIndex)
	{
		for (int i = aStartIndex; i < myVector.capacity(); ++i)
		{
			myVector.push_back(T());
		}
	}
}

namespace CU = CommonUtilities;