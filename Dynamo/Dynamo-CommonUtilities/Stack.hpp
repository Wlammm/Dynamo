#pragma once

#include <vector>
#include <assert.h>

namespace CommonUtilities
{
	template <class T>
	class Stack
	{
	public:
		//Skapar en tom stack
		Stack();

		//Returnerar antal element i stacken
		int GetSize() const;

		//Returnerar det �versta elementet i stacken.
		const T& GetTop() const;

		//Returnerar det �versta elementet i stacken.
		T& GetTop();

		//L�gger in ett nytt element �verst p� stacken
		void Push(const T& aValue);

		//Tar bort det �versta elementet fr�n stacken och returnerar det.
		T Pop();

	private:
		std::vector<T> myVector;
	};

	template<class T>
	inline Stack<T>::Stack()
	{
	}
	template<class T>
	inline int Stack<T>::GetSize() const
	{
		return myVector.size();
	}
	template<class T>
	inline const T& Stack<T>::GetTop() const
	{
		assert(myVector.size() > 0 && "Stack is empty.");

		return myVector[myVector.size() - 1];
	}
	template<class T>
	inline T& Stack<T>::GetTop()
	{
		assert(myVector.size() > 0 && "Stack is empty.");

		return myVector[myVector.size() - 1];
	}
	template<class T>
	inline void Stack<T>::Push(const T& aValue)
	{
		myVector.push_back(aValue);
	}
	template<class T>
	inline T Stack<T>::Pop()
	{
		assert(myVector.size() > 0 && "Stack is emtpy.");

		T returnValue = myVector[myVector.size() - 1];
		myVector.pop_back();
		return returnValue;
	}
}

namespace CU = CommonUtilities;