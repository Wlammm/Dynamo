#pragma once

namespace CommonUtilities
{
	template<class T>
	class BSTSet;

	template <class T>
	class BSTSetNode
	{
	public:
		BSTSetNode(const BSTSetNode&) = delete;
		BSTSetNode& operator=(const BSTSetNode&) = delete;
		const T& GetData() const;
		const BSTSetNode<T>* GetLeft() const;
		const BSTSetNode<T>* GetRight() const;

	private:
		BSTSetNode<T>() { }
		friend class BSTSet<T>;
		
		BSTSetNode(const T& aValue);

		const BSTSetNode<T>* Find(const T& aValue) const;
		const BSTSetNode<T>* Insert(const T& aValue);
		void Remove(const T& aValue);

		BSTSetNode<T>* myRight = nullptr;
		BSTSetNode<T>* myLeft = nullptr;
		T myValue;
	};

	template<class T>
	BSTSetNode<T>::BSTSetNode<T>(const T& aValue)
	{
		myValue = aValue;
	}

	template<class T>
	const BSTSetNode<T>* BSTSetNode<T>::Find(const T& aValue) const
	{
		if (!(aValue < myValue) && !(myValue < aValue))
		{
			return this;
		}

		if (myValue < aValue && myRight != nullptr)
		{
			return myRight->Find(aValue);
		}

		if (aValue < myValue && myLeft != nullptr)
		{
			return myLeft->Find(aValue);
		}

		return nullptr;
	}

	template<class T>
	const BSTSetNode<T>* BSTSetNode<T>::Insert(const T& aValue)
	{
		if (!(aValue < myValue) && !(myValue < aValue))
		{
			return this;
		}

		if (myValue < aValue)
		{
			if (myRight != nullptr)
			{
				myRight->Insert(aValue);
			}
			else
			{
				myRight = new BSTSetNode<T>();
				myRight->myValue = aValue;
			}
		}

		if (aValue < myValue)
		{
			if (myLeft != nullptr)
			{
				myLeft->Insert(aValue);
			}
			else
			{
				myLeft = new BSTSetNode<T>();
				myLeft->myValue = aValue;
			}
		}
	}

	template<class T>
	void BSTSetNode<T>::Remove(const T& aValue)
	{
		if (myValue < aValue)
		{
			if (myRight == nullptr)
			{
				return;
			}

			if (!(myRight->GetData() < aValue) && !(aValue < myRight->GetData()))
			{
				BSTSetNode<T>* nodeToDelete = myRight;

				if (nodeToDelete->GetRight() != nullptr)
				{
					myRight = nodeToDelete->myRight;

					if (nodeToDelete->GetLeft() != nullptr)
					{
						BSTSetNode<T>* node = myRight;
						while (node->GetLeft() != nullptr)
						{
							node = node->myLeft;
						}
						node->myLeft = nodeToDelete->myLeft;
					}
				}
				else if (nodeToDelete->GetLeft() != nullptr)
				{
					myLeft = nodeToDelete->myLeft;
				} 
				else
				{
					myRight = nullptr;
				}

				nodeToDelete->myLeft = nullptr;
				nodeToDelete->myRight = nullptr;
				delete nodeToDelete;
				return;
			}
			else
			{
				myRight->Remove(aValue);
			}
		}
		
		if (aValue < myValue)
		{
			if (myLeft == nullptr)
			{
				return;
			}

			if (!(aValue < myLeft->GetData()) && !(myLeft->GetData() < aValue))
			{
				BSTSetNode<T>* nodeToDelete = myLeft;

				if (nodeToDelete->GetRight() != nullptr)
				{
					myLeft = nodeToDelete->myRight;

					if (nodeToDelete->GetLeft() != nullptr)
					{
						BSTSetNode<T>* node = myLeft;
						while (node->GetLeft() != nullptr)
						{
							node = node->myLeft;
						}
						node->myLeft = nodeToDelete->myLeft;
					}
				}
				else if (nodeToDelete->GetLeft() != nullptr)
				{
					myLeft = nodeToDelete->myLeft;
				}
				else
				{
					myLeft = nullptr;
				}

				nodeToDelete->myLeft = nullptr;
				nodeToDelete->myRight = nullptr;
				delete nodeToDelete;
				return;
			}
			else
			{
				myLeft->Remove(aValue);
			}
		}
	}

	template<class T>
	const BSTSetNode<T>* BSTSetNode<T>::GetRight() const
	{
		return myRight;
	}

	template<class T>
	const BSTSetNode<T>* BSTSetNode<T>::GetLeft() const
	{
		return myLeft;
	}
	template<class T>
	const T& BSTSetNode<T>::GetData() const
	{
		return myValue;
	}
}

namespace CU = CommonUtilities;