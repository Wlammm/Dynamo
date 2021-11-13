#pragma once

#include "BSTSetNode.hpp"

namespace CommonUtilities
{
	template <class T>
	class BSTSet
	{
	public:
		BSTSet() = default;
		~BSTSet();
		const BSTSetNode<T>* GetRoot() const;
		bool HasElement(const T& aData) const;
		void Insert(const T& aData);
		void Remove(const T& aData);
		void DSWBalance();

	private:
		bool HasElement(const T& aData, BSTSetNode<T>* aNode) const;
		BSTSetNode<T>* Insert(const T& aData, BSTSetNode<T>* aNode);
		BSTSetNode<T>* Remove(const T& aData, BSTSetNode<T>* aNode);
		BSTSetNode<T>* MinValueNode(BSTSetNode<T>* aNode) const;
		bool RotateRight(BSTSetNode<T>* aNode);
		bool RotateLeft(BSTSetNode<T>* aNode);

		BSTSetNode<T>* myRoot = nullptr;
	};

	template<class T>
	BSTSet<T>::~BSTSet()
	{
	}

	template<class T>
	const BSTSetNode<T>* BSTSet<T>::GetRoot() const
	{
		return myRoot;
	}

	template<class T>
	bool BSTSet<T>::HasElement(const T& aData) const
	{
		return HasElement(aData, myRoot);
	}
	
	template<class T>
	void BSTSet<T>::Insert(const T& aData)
	{
		if (HasElement(aData))
		{
			return;
		}

		myRoot = Insert(aData, myRoot);
	}
	
	template<class T>
	void BSTSet<T>::Remove(const T& aData)
	{
		myRoot = Remove(aData, myRoot);
	}
	
	template<class T>
	void BSTSet<T>::DSWBalance()
	{
		BSTSetNode<T>* currentNode = myRoot;
		int nodeCount = 0;

		while (currentNode != nullptr)
		{
			while (currentNode->myLeft != nullptr)
			{
				RotateRight(currentNode);
			}
			currentNode = currentNode->myRight;
			nodeCount++;
		}

		nodeCount; // Check so backbone successfull!

		int expected = ceil(log2(nodeCount)) - nodeCount;
		BSTSetNode<T>* currentNode1 = myRoot;

		for (int i = 0; i < expected; ++i)
		{
			if (i == 0)
			{
				RotateLeft(currentNode1);
				currentNode1 = myRoot;
			}
			else
			{
				RotateLeft(currentNode1->myRight);
				currentNode1 = currentNode1->myRight;
			}
		}

		int times = nodeCount;
		while (times > 1)
		{
			times /= 2;

			RotateLeft(myRoot);
			BSTSetNode<T>* currentNode2 = myRoot;

			for (int i = 0; i < times - 1; ++i)
			{
				RotateLeft(currentNode2->myLeft);
				currentNode2 = currentNode2->myRight;
			}
		}
	}

	template<class T>
	bool BSTSet<T>::HasElement(const T& aData, BSTSetNode<T>* aNode) const
	{
		if (aNode == nullptr)
		{
			return false;
		}

		if (aNode->myData == aData)
		{
			return true;
		}

		if (aData < aNode->myData)
		{
			return HasElement(aData, aNode->myLeft);
		}

		if (aNode->myData < aData)
		{
			return HasElement(aData, aNode->myRight);
		}
	}
	template<class T>
	BSTSetNode<T>* BSTSet<T>::Insert(const T& aData, BSTSetNode<T>* aNode)
	{
		if (aNode == nullptr)
		{
			aNode = new BSTSetNode<T>();
			aNode->myData = aData;
			return aNode;
		}

		if (aNode->myData < aData)
		{
			aNode->myRight = Insert(aData, aNode->myRight);
		}
		else
		{
			aNode->myLeft = Insert(aData, aNode->myLeft);
		}

		return aNode;
	}
	template<class T>
	inline BSTSetNode<T>* BSTSet<T>::Remove(const T& aData, BSTSetNode<T>* aNode)
	{
		if (aNode == nullptr)
		{
			return aNode;
		}

		if (aData < aNode->myData)
		{
			aNode->myLeft = Remove(aData, aNode->myLeft);
		}
		else if (aNode->myData < aData)
		{
			aNode->myRight = Remove(aData, aNode->myRight);
		}
		else
		{
			if (aNode->myLeft == nullptr)
			{
				return aNode->myRight;
			}

			if (aNode->myRight == nullptr)
			{
				return aNode->myLeft;
			}

			BSTSetNode<T>* temp = MinValueNode(aNode->myRight);
			aNode->myData = temp->myData;

			aNode->myRight = Remove(temp->myData, aNode->myRight);
		}

		return aNode;
	}

	template<class T>
	inline BSTSetNode<T>* BSTSet<T>::MinValueNode(BSTSetNode<T>* aNode) const
	{
		BSTSetNode<T>* current = aNode;

		while (current != nullptr && current->myLeft != nullptr)
		{
			current = current->myLeft;
		}

		return current;
	}
	template<class T>
	inline bool BSTSet<T>::RotateRight(BSTSetNode<T>* aNode)
	{
		if (aNode == nullptr ||  aNode->myLeft == nullptr)
		{
			return false;
		}

		T tempData = aNode->myData;
		aNode->myData = aNode->myLeft->myData;
		aNode->myLeft->myData = tempData;

		BSTSetNode<T>* originalLeft = aNode->myLeft;
		aNode->myLeft = originalLeft->myLeft;
		originalLeft->myLeft = originalLeft->myRight;
		originalLeft->myRight = aNode->myRight;
		aNode->myRight = originalLeft;

		return true;
	}
	template<class T>
	inline bool BSTSet<T>::RotateLeft(BSTSetNode<T>* aNode)
	{
		if (aNode == nullptr || aNode->myRight == nullptr)
		{
			return false;
		}

		T tempData = aNode->myData;
		aNode->myData = aNode->myRight->myData;
		aNode->myRight->myData = tempData;

		BSTSetNode<T>* originalRight = aNode->myRight;
		aNode->myRight = originalRight->myRight;
		originalRight->myRight = originalRight->myLeft;
		originalRight->myLeft = aNode->myLeft;
		aNode->myLeft = originalRight;

		return true;
	}
}

namespace CU = CommonUtilities;