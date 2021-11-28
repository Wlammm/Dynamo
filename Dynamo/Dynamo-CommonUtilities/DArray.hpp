#pragma once

#include <vector>
#include <initializer_list>

using uint = unsigned int;

namespace CommonUtilities
{
	template<typename T>
	class DArray
	{
	public:
		DArray() = default;

		DArray(const uint aSize)
		{
			myVector = std::vector<T>(aSize);
		}

		DArray(const std::initializer_list<T>& aList)
		{
			myVector = std::vector<T>();
			for (auto& val : aList)
			{
				myVector.push_back(val);
			}
		}

		~DArray() = default;

#pragma region Operators
		inline const T& operator[](const uint aIndex) const
		{
			return myVector[aIndex];
		}

		inline T& operator[](const uint aIndex)
		{
			return myVector[aIndex];
		}

		inline const T& operator[](const int aIndex) const
		{
			return myVector[aIndex];
		}

		inline T& operator[](const int aIndex)
		{
			return myVector[aIndex];
		}

		inline void operator=(const DArray<T>& aSecond)
		{
			myVector = aSecond.myVector;
		}

		inline void operator=(const std::vector<T>& aSecond)
		{
			myVector = aSecond;
		}

		template<uint Size>
		inline void operator=(const std::array<T, Size>& aSecond)
		{
			myVector.clear();

			for (int i = 0; i < Size; ++i)
			{
				myVector.push_back(aSecond[i]);
			}
		}

		inline void operator=(const std::initializer_list<T>& aInitList)
		{
			myVector.clear();
			for (auto& val : aInitList)
			{
				myVector.push_back(val);
			}
		}

#pragma endregion

#pragma region Size
		inline const uint size() const
		{
			return static_cast<uint>(myVector.size());
		}

		inline const int sizeI() const
		{
			return static_cast<int>(myVector.size());
		}

		template<typename T>
		inline const T size() const
		{
			return static_cast<T>(myVector.size());
		}
#pragma endregion

#pragma region Add
		inline void Add(const T& aValue)
		{
			myVector.push_back(aValue);
		}

		inline void push_back(const T& aValue)
		{
			myVector.push_back(aValue);
		}

		inline void emplace_back(const T& aValue)
		{
			myVector.emplace_back(aValue);
		}

		inline void AddRange(const DArray<T>& aRange)
		{
			for (int i = 0; i < aRange.sizeI(); ++i)
			{
				myVector.push_back(aRange.myVector[i]);
			}
		}

		inline void AddRange(const std::vector<T>& aRange)
		{
			for (int i = 0; i < aRange.size(); ++i)
			{
				myVector.push_back(aRange[i]);
			}
		}

		template<uint Size>
		inline void AddRange(const std::array<T, Size>& aRange)
		{
			for (int i = 0; i < Size; ++i)
			{
				myVector.push_back(aRange[i]);
			}
		}

		// Unsafe.
		inline void AddRange(T* aPtr, const uint aLength)
		{
			for (int i = 0; i < aLength; ++i)
			{
				myVector.push_back(aPtr[aLength]);
			}
		}
#pragma endregion

#pragma region Remove
		inline void Remove(const T& aValue)
		{
			assert(std::find(myVector.begin(), myVector.end(), aValue) != myVector.end() && "Element does not exist in vector.");

			myVector.erase(std::remove(myVector.begin(), myVector.end(), aValue));
		}

		inline void TryRemove(const T& aValue)
		{
			if (std::find(myVector.begin(), myVector.end(), aValue) != myVector.end())
			{
				myVector.erase(std::remove(myVector.begin(), myVector.end(), aValue));
			}
		}

		inline void RemoveIndex(const uint aIndex)
		{
			assert(myVector.size() > aIndex && aIndex >= 0 && "Index out of range.");

			myVector.erase(myVector.begin() + aIndex);
		}

		inline void RemoveRange(const DArray<T>& aRange)
		{
			for (uint i = 0; i < aRange.SizeI(); ++i)
			{
				Remove(aRange[i]);
			}
		}

		inline void RemoveRange(const std::vector<T>& aRange)
		{
			for (uint i = 0; i < aRange.size(); ++i)
			{
				Remove(aRange[i]);
			}
		}

		template<uint Size>
		inline void RemoveRange(const std::array<T, Size>& aRange)
		{
			for (uint i = 0; i < Size; ++i)
			{
				Remove(aRange[i]);
			}
		}

		inline void RengeRange(T* aRange, uint aLength)
		{
			for (uint i = 0; i < aLength; ++i)
			{
				Remove(aRange[i]);
			}
		}

#pragma endregion

#pragma region Insert
		inline void Insert(const T& aElement, const uint aIndex)
		{
			myVector.insert(myVector.begin() + aIndex, aElement);
		}

		inline void InsertRange(const DArray<T>& aRange, const uint aIndex)
		{
			for (uint i = 0; i < aRange.size(); ++i)
			{
				Insert(aRange[i], aIndex + i);
			}
		}

		inline void InsertRange(const std::vector<T>& aRange, const uint aIndex)
		{
			for (uint i = 0; i < aRange.size(); ++i)
			{
				Insert(aRange[i], aIndex + i);
			}
		}

		template<uint Size>
		inline void InsertRange(const std::array<T, Size>& aRange, const uint aIndex)
		{
			for (uint i = 0; i < Size; ++i)
			{
				Insert(aRange[i], aIndex + i);
			}
		}

		inline void InsertRange(T* aRange, const uint aLength, const uint aIndex)
		{
			for (int i = 0; i < aLength; ++i)
			{
				Insert(aRange[i], aIndex + i);
			}
		}

#pragma endregion

#pragma region Iterators
		inline std::_Vector_iterator<T> begin() const
		{
			return myVector.begin();
		}

		inline std::_Vector_const_iterator<T> cbegin() const
		{
			return myVector.cbegin();
		}

		inline std::_Vector_iterator<T> end() const
		{
			return myVector.end();
		}

		inline std::_Vector_const_iterator<T> cend() const
		{
			return myVector.cend();
		}
#pragma endregion

#pragma region Contains
		inline const bool Contains(const T& anElement) const
		{
			return (std::find(myVector.begin(), myVector.end(), anElement) != myVector.end());
		}

		inline const bool ContainsRange(const DArray<T>& aRange)
		{
			for (uint i = 0; i < aRange.size(); ++i)
			{
				if (!Contains(aRange[i]))
				{
					return false;
				}
			}

			return true;
		}

		inline const bool ContainsRange(const std::vector<T>& aRange)
		{
			for (uint i = 0; i < aRange.size(); ++i)
			{
				if (!Contains(aRange[i]))
				{
					return false;
				}
			}

			return true;
		}

		template<uint Size>
		inline const bool ContainsRange(const std::array<T, Size>& aRange)
		{
			for (uint i = 0; i < Size; ++i)
			{
				if (!Contains(aRange[i]))
				{
					return false;
				}
			}

			return true;
		}

		inline const bool ContainsRange(T* aRange, const uint aLength)
		{
			for (uint i = 0; i < aLength; ++i)
			{
				if (!Contains(aRange[i]))
				{
					return false;
				}
			}

			return true;
		}



#pragma endregion

		inline const std::vector<T>& AsVector() const
		{
			return myVector;
		}

		inline std::vector<T>& AsVector()
		{
			return myVector;
		}

		T& back()
		{
			return myVector.back();
		}

		inline const T& back() const
		{
			return myVector.back();
		}

		inline T& front()
		{
			return myVector.front();
		}

		inline const T& front() const
		{
			return myVector.front();
		}

		inline const uint capacity() const
		{
			return myVector.capacity();
		}

		inline const bool empty() const
		{
			return myVector.empty();
		}

		inline const T* data() const
		{
			return myVector.data();
		}

		inline T* data()
		{
			return myVector.data();
		}

		inline void clear()
		{
			myVector.clear();
		}

		inline void pop_back()
		{
			myVector.pop_back();
		}

		inline void Resize(const uint aSize)
		{
			myVector.resize(aSize);
		}

		inline void Reserve(const uint aSize)
		{
			myVector.reserve(aSize);
		}

		inline void ShrinkToFit()
		{
			myVector.shrink_to_fit();
		}

	private:
		std::vector<T> myVector{};
	};
}