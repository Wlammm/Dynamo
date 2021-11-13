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
		BSTSetNode<T>() {}
		friend class BSTSet<T>;

		T myData;
		BSTSetNode<T>* myLeft = nullptr;
		BSTSetNode<T>* myRight = nullptr;
	};

	template<class T>
	inline const T& BSTSetNode<T>::GetData() const
	{
		return myData;
	}
	template<class T>
	inline const BSTSetNode<T>* BSTSetNode<T>::GetLeft() const
	{
		return myLeft;
	}
	template<class T>
	inline const BSTSetNode<T>* BSTSetNode<T>::GetRight() const
	{
		return myRight;
	}
}

namespace CU = CommonUtilities;