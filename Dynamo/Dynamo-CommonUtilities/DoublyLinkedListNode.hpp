#pragma once

namespace CommonUtilities
{
	template<typename T>
	class DoublyLinkedList;

	template <typename T>
	class DoublyLinkedListNode
	{
	public:
		
		// Copy-konstruktorn och assignment-operatorn är borttagna, så att det enda
		// sättet att skapa en nod är genom att stoppa in ett värde i en lista.
		DoublyLinkedListNode<T>(const DoublyLinkedListNode<T>&) = delete;
		DoublyLinkedListNode<T>& operator=(const DoublyLinkedListNode<T>&) = delete;
	
		// Returnerar nodens värde
		const T& GetValue() const;
		T& GetValue();
	
		// Returnerar nästa nod i listan, eller nullptr om noden är sist i listan
		DoublyLinkedListNode<T>* GetNext() const;
	
		// Returnerar föregående nod i listan, eller nullptr om noden är först i
		// listan
		DoublyLinkedListNode<T>* GetPrevious() const;
	private:
		T myData;
		DoublyLinkedListNode<T>* myPrevious = nullptr;
		DoublyLinkedListNode<T>* myNext = nullptr;
		
		friend class DoublyLinkedList<T>;
		DoublyLinkedListNode(const T& aValue);
		~DoublyLinkedListNode();
	};
	
	template<typename T>
	inline DoublyLinkedListNode<T>::DoublyLinkedListNode(const T& aValue)
	{
		myData = aValue;
	}

	template<typename T>
	inline DoublyLinkedListNode<T>::~DoublyLinkedListNode()
	{
	}

	template<typename T>
	const T& DoublyLinkedListNode<T>::GetValue() const
	{
		return myData;
	}
	
	template<typename T>
	T& DoublyLinkedListNode<T>::GetValue()
	{
		return myData;
	}
	
	template<typename T>
	DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetNext() const
	{
		return myNext;
	}
	
	template<typename T>
	DoublyLinkedListNode<T>* DoublyLinkedListNode<T>::GetPrevious() const
	{
		return myPrevious;
	}
}

namespace CU = CommonUtilities;