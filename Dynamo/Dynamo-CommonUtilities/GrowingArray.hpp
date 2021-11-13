#pragma once
#include <assert.h>
#include <initializer_list>

namespace CommonUtilities
{
	template<typename T>
	class GrowingArray
	{
	public:
		GrowingArray();

		GrowingArray(const std::initializer_list<T>& aInitList);
		~GrowingArray();

		T operator[](const int& anIndex);

		void Grow();

		void Add(T aObject);
		void Remove(T aObject);
		void RemoveIndex(const int& anIndex);
		bool Contains(T aObject);
		void Reserve(const int& aSize);
		void Fill();
		void Clear();
		void Insert(T aObject, const int& aIndex);

		const int& Size() const;
		const int& ReservedSize() const;

		T* GetElementAtIndex(const int& anIndex);
		const int GetIndexOfElement(T aObject);

	private:
		T* myArray;
		int mySize = 2;
		int myElementCount = 0;
		const int myGrowFactor = 2;
	};

	template<typename T>
	GrowingArray<T>::GrowingArray()
	{
		mySize = 2;
		myElementCount = 0;

		myArray = new T[mySize];
	}

	template<typename T>
	GrowingArray<T>::GrowingArray(const std::initializer_list<T>& aInitList)
	{
		mySize = 2;
		myElementCount = 0;
		myArray = new T[mySize];

		for (auto iter = aInitList.begin(); iter != aInitList.end(); ++iter)
		{
			Add(*iter);
		}
	}

	template<typename T>
	GrowingArray<T>::~GrowingArray()
	{
		delete[](myArray);
	}

	template<typename T>
	T GrowingArray<T>::operator[](const int& anIndex)
	{
		return myArray[anIndex];
	}

	template<typename T>
	void GrowingArray<T>::Grow()
	{
		const int oldSize = mySize;
		mySize *= myGrowFactor;
		T* tempArray = new T[mySize];

		for (int i = 0; i < oldSize; i++)
		{
			tempArray[i] = myArray[i];
		}
		delete[] myArray;
		myArray = tempArray;
	}

	template<typename T>
	void GrowingArray<T>::Add(T aObject)
	{
		myElementCount++;
		if (myElementCount == mySize)
		{
			Grow();
			myArray[myElementCount - 1] = aObject;
			return;
		}

		myArray[myElementCount - 1] = aObject;
	}

	template<typename T>
	void GrowingArray<T>::Remove(T aObject)
	{
		if (myElementCount == 0)
		{
			return;
		}

		for (int i = 0; i < myElementCount; ++i)
		{
			if (myArray[i] == aObject)
			{
				RemoveIndex(i);
				return;
			}
		}
	}

	template<typename T>
	void GrowingArray<T>::RemoveIndex(const int& anIndex)
	{
		assert(anIndex >= 0 && anIndex < mySize && "Out of bounds.");

		for (int i = anIndex + 1; i < myElementCount; ++i)
		{
			myArray[i - 1] = myArray[i];
		}

		myElementCount--;
	}

	template<typename T>
	bool GrowingArray<T>::Contains(T aObject)
	{
		for (int i = 0; i < myElementCount; ++i)
		{
			if (myArray[i] == aObject)
			{
				return true;
			}
		}
		return false;
	}

	template<typename T>
	void GrowingArray<T>::Reserve(const int& aSize)
	{
		const int oldSize = mySize;
		mySize = aSize;
		T* tempArray = new T[mySize];

		for (int i = 0; i < oldSize; i++)
		{
			if (i >= mySize)
			{
				break;
			}

			tempArray[i] = myArray[i];
		}
		delete[] myArray;
		myArray = tempArray;
	}

	template<typename T>
	void GrowingArray<T>::Fill()
	{
		for (; myElementCount <= mySize; myElementCount++)
		{
			myArray[myElementCount - 1] = T();
		}
	}

	template<typename T>
	void GrowingArray<T>::Clear()
	{
		Reserve(2);
		myElementCount = 0;
	}

	template<typename T>
	void GrowingArray<T>::Insert(T aObject, const int& aIndex)
	{
		myElementCount++;
		if (mySize == myElementCount)
		{
			Grow();
		}

		for (int i = myElementCount; i >= aIndex; --i)
		{
			myArray[i] = myArray[i - 1];
		}

		myArray[aIndex] = aObject;
	}

	template<typename T>
	const int& GrowingArray<T>::Size() const
	{
		return myElementCount;
	}

	template<typename T>
	const int& GrowingArray<T>::ReservedSize() const
	{
		return mySize;
	}

	template<typename T>
	T* GrowingArray<T>::GetElementAtIndex(const int& anIndex)
	{
		assert(anIndex >= 0 && anIndex < mySize && "Out of bounds.");
		return myArray[anIndex];
	}
	template<typename T>
	const int GrowingArray<T>::GetIndexOfElement(T aObject)
	{
		for (int i = 0; i < myElementCount; ++i)
		{
			if (myArray[i] == aObject)
			{
				return i;
			}
		}
	}
}

namespace CU = CommonUtilities;