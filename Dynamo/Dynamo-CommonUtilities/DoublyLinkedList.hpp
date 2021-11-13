#pragma once

#include "DoublyLinkedListNode.hpp"
#include <assert.h>

namespace CommonUtilities
{
	template <typename T>
	class DoublyLinkedList
	{
	public:
		// Skapar en tom lista
		DoublyLinkedList();

		// Frigör allt minne som listan allokerat
		~DoublyLinkedList();

		// Returnerar antalet element i listan
		int GetSize() const;

		// Returnerar första noden i listan, eller nullptr om listan är tom
		DoublyLinkedListNode<T>* GetFirst();

		// Returnerar sista noden i listan, eller nullptr om listan är tom
		DoublyLinkedListNode<T>* GetLast();

		// Skjuter in ett nytt element först i listan
		void InsertFirst(const T& aValue);

		// Skjuter in ett nytt element sist i listan
		void InsertLast(const T& aValue);

		// Skjuter in ett nytt element innan aNode
		void InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue);

		// Skjuter in ett nytt element efter aNode
		void InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue);

		// Plockar bort noden ur listan och frigör minne. (Det är ok att anta att
		// aNode är en nod i listan, och inte från en annan lista)
		void Remove(DoublyLinkedListNode<T>* aNode);
		// Hittar första elementet i listan som har ett visst värde. Jämförelsen
		// görs med operator==. Om inget element hittas returneras nullptr.
		DoublyLinkedListNode<T>* FindFirst(const T& aValue);

		// Hittar sista elementet i listan som har ett visst värde. Jämförelsen
		// görs med operator==. Om inget element hittas returneras nullptr.
		DoublyLinkedListNode<T>* FindLast(const T& aValue);

		// Plockar bort första elementet i listan som har ett visst värde.
		// Jämförelsen görs med operator==. Om inget element hittas görs ingenting.
		// Returnerar true om ett element plockades bort, och false annars.
		bool RemoveFirst(const T& aValue);

		// Plockar bort sista elementet i listan som har ett visst värde.
		// Jämförelsen görs med operator==. Om inget element hittas görs ingenting.
		// Returnerar true om ett element plockades bort, och false annars.
		bool RemoveLast(const T& aValue);


	private:
		DoublyLinkedListNode<T>* myFirst = nullptr;
		unsigned int mySize = 0;
	};

	template<typename T>
	inline DoublyLinkedList<T>::DoublyLinkedList()
	{
	}

	template<typename T>
	inline DoublyLinkedList<T>::~DoublyLinkedList()
	{
		/*DoublyLinkedListNode<T>* node = GetLast();
		while (node->myPrevious != nullptr)
		{
			DoublyLinkedListNode<T>* previousNode = node->myPrevious;
			delete node;
			node = previousNode;
		}*/
	}

	template<typename T>
	inline int DoublyLinkedList<T>::GetSize() const
	{
		return mySize;
	}

	template<typename T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetFirst()
	{
		return myFirst;
	}

	template<typename T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::GetLast()
	{
		if (mySize == 0)
		{
			return nullptr;
		}

		DoublyLinkedListNode<T>* node = myFirst;

		while (node != nullptr)
		{
			if (node->myNext == nullptr)
				break;

			node = node->myNext;
		}

		return node;
	}

	template<typename T>
	inline void DoublyLinkedList<T>::InsertFirst(const T& aValue)
	{
		DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);

		if (myFirst == nullptr)
		{
			myFirst = newNode;
			mySize++;
			return;
		}

		myFirst->myPrevious = newNode;
		newNode->myNext = myFirst;

		myFirst = newNode;
		mySize++;
	}

	template<typename T>
	inline void DoublyLinkedList<T>::InsertLast(const T& aValue)
	{
		DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);

		DoublyLinkedListNode<T>* node = myFirst;

		if (myFirst == nullptr)
		{
			myFirst = newNode;
			mySize++;
			return;
		}

		while (node->myNext != nullptr)
		{
			node = node->myNext;
		}

		node->myNext = newNode;
		newNode->myPrevious = node;
		mySize++;
	}

	template<typename T>
	inline void DoublyLinkedList<T>::InsertBefore(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);
		newNode->myNext = aNode;

		DoublyLinkedListNode<T>* node = myFirst;

		if (myFirst == nullptr)
		{
			myFirst = newNode;
			mySize++;
			return;
		}

		if (myFirst == aNode)
		{
			newNode->myNext = myFirst;
			myFirst->myPrevious = newNode;
			myFirst = newNode;
		}

		while (node != nullptr)
		{
			if (node == aNode)
			{
				newNode->myPrevious = node->myPrevious;
				node->myPrevious = newNode;
				mySize++;
				return;
			}

			node = node->myNext;
		}
	}

	template<typename T>
	inline void DoublyLinkedList<T>::InsertAfter(DoublyLinkedListNode<T>* aNode, const T& aValue)
	{
		DoublyLinkedListNode<T>* newNode = new DoublyLinkedListNode<T>(aValue);
		newNode->myPrevious = aNode;
		newNode->myNext = aNode->myNext;
		aNode->myNext = newNode;
		mySize++;
	}

	template<typename T>
	inline void DoublyLinkedList<T>::Remove(DoublyLinkedListNode<T>* aNode)
	{
		if (mySize == 1)
		{
			mySize = 0;
			delete myFirst;
			myFirst = nullptr;
			return;
		}

		if (aNode == myFirst)
		{
			myFirst = myFirst->myNext;
			myFirst->myPrevious = nullptr;
			delete aNode;
			aNode = nullptr;
			mySize--;
			return;
		}

		if (aNode->myNext != nullptr)
		{
			aNode->myPrevious->myNext = aNode->myNext;
			aNode->myNext->myPrevious = aNode->myPrevious;
		}
		else
		{
			aNode->myPrevious->myNext = nullptr;
		}

		delete aNode;
		aNode = nullptr;
		mySize--;
	}

	template<typename T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindFirst(const T& aValue)
	{
		if (mySize == 0)
		{
			return nullptr;
		}

		DoublyLinkedListNode<T>* node = myFirst;

		while (node->myData != aValue)
		{
			if (node->myNext == nullptr)
			{
				return nullptr;
			}

			node = node->myNext;
		}

		return node;
	}

	template<typename T>
	inline DoublyLinkedListNode<T>* DoublyLinkedList<T>::FindLast(const T& aValue)
	{
		DoublyLinkedListNode<T>* result = nullptr;
		DoublyLinkedListNode<T>* node = myFirst;

		while (node != nullptr)
		{
			if (node->myData == aValue)
			{
				result = node;
			}

			if (node->myNext == nullptr)
				break;

			node = node->myNext;
		}

		return result;
	}

	template<typename T>
	inline bool DoublyLinkedList<T>::RemoveFirst(const T& aValue)
	{
		DoublyLinkedListNode<T>* node = FindFirst(aValue);
		if (node == nullptr)
		{
			return false;
		}

		Remove(node);
		return true;
	}

	template<typename T>
	inline bool DoublyLinkedList<T>::RemoveLast(const T& aValue)
	{
		if (mySize == 0)
		{
			return false;
		}

		DoublyLinkedListNode<T>* node = FindLast(aValue);
		if (node == nullptr)
		{
			return false;
		}

		Remove(node);
		return true;
	}
}
namespace CU = CommonUtilities;