#pragma once

#include <vector>
#include "Hash.hpp"

enum class eState : uint16_t
{
	Empty = 1 << 0,
	InUse = 1 << 1,
	Removed = 1 << 2
};

template<class Key, class Value>
struct HashSet {
	eState myState = eState::Empty;
	Value myValue;
	Key myKey;
};

namespace CommonUtilities
{
	template <class Key, class Value>
	class HashMap
	{
	public:
		//Skapar en HashMap med plats för aCapacity element. Den behöver inte 
		//kunna växa. Blir den full så blir den full.
		HashMap(int aCapacity);

		//Stoppar in aValue under nyckeln aKey. Om det fanns ett värde innan med
		//aKey som nyckel så skrivs det värdet över. Om det lyckas returneras
		//true. Om map:en är full så görs ingenting, och false returneras.
		bool Insert(const Key& aKey, const Value& aValue);

		//Plockar bort elementet med nyckeln aKey, om det finns. Om det finns
		//returneras true. Om det inte finns görs ingenting, och false
		//returneras.
		bool Remove(const Key& aKey);

		//Ger en pekare till värdet med nyckeln aKey, om det finns. Om det inte
		//finns returneras nullptr.
		const Value* Get(const Key& aKey) const;

		//Som ovan, men returnerar en icke-const-pekare.
		Value* Get(const Key& aKey);

	private:
		std::vector<HashSet<Key, Value>> myArray;
		const int myCapacity = 0;
	};

	template <class Key, class Value>
	HashMap<Key, Value>::HashMap(int aCapacity)
		: myCapacity(aCapacity)
	{
		myArray.reserve(aCapacity);
		myArray.resize(aCapacity);
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::Insert(const Key& aKey, const Value& aValue)
	{
		if (myCapacity == 0)
			return false;

		int hashIndex = Hash(aKey) % myCapacity;

		HashSet<Key, Value> hashSet = HashSet<Key, Value>();
		hashSet.myKey = aKey;
		hashSet.myValue = aValue;
		hashSet.myState = eState::InUse;

		int count = 0;
		do 
		{
			if (++count >= myCapacity)
			{
				return false;
			}

			if (myArray[hashIndex].myState != eState::InUse)
			{
				myArray[hashIndex] = hashSet;
				return true;
			}

			if (myArray[hashIndex].myKey < aKey == false && aKey < myArray[hashIndex].myKey == false)
			{
				myArray[hashIndex] = hashSet;
				return true;
			}
			
			if (++hashIndex >= myCapacity)
			{
				hashIndex -= myCapacity;
			}
		}
		while (myArray[hashIndex].myState == eState::InUse);
		
		myArray[hashIndex] = hashSet;
		return true;
	}

	template <class Key, class Value>
	bool HashMap<Key, Value>::Remove(const Key& aKey)
	{
		if (myCapacity == 0)
			return false;

		int hashIndex = Hash(aKey) % myCapacity;

		if(myArray[hashIndex].myState == eState::Empty)
		{
			return false;
		}

		int count = 0;
		while(myArray[hashIndex].myState == eState::Removed ||myArray[hashIndex].myState == eState::InUse)
		{
			if (++count > myCapacity)
			{
				return false;
			}

			if(myArray[hashIndex].myState == eState::Removed)
			{
				hashIndex++;

				if (hashIndex >= myCapacity)
				{
					hashIndex -= myCapacity;
				}

				continue;
			}

			if(myArray[hashIndex].myState == eState::InUse)
			{
				if(!(myArray[hashIndex].myKey < aKey && aKey < myArray[hashIndex].myKey))
				{
					myArray[hashIndex].myState = eState::Removed;
					return true;
				}
			}

			hashIndex++;

			if (hashIndex >= myCapacity)
			{
				hashIndex -= myCapacity;
			}
		}
		return false;
	}

	template <class Key, class Value>
	const Value* HashMap<Key, Value>::Get(const Key& aKey) const
	{
		if (myCapacity == 0)
			return false;

		int hashIndex = Hash(aKey) % myCapacity;

		int count = 0;
		do
		{
			if (++count >= myCapacity)
			{
				return nullptr;
			}

			if (myArray[hashIndex].myKey < aKey == false && aKey < myArray[hashIndex].myKey == false)
			{
				return myArray[hashIndex].myValue;
			}

			if (++hashIndex >= myCapacity)
			{
				hashIndex -= myCapacity;
			}
		}
		while (myArray[hashIndex].myState == eState::InUse);
	}

	template <class Key, class Value>
	Value* HashMap<Key, Value>::Get(const Key& aKey)
	{
		if (myCapacity == 0)
			return false;

		int hashIndex = Hash(aKey) % myCapacity;

		int count = 0;
		do
		{
			if (++count >= myCapacity)
			{
				return nullptr;
			}

			if (myArray[hashIndex].myKey < aKey == false && aKey < myArray[hashIndex].myKey == false && myArray[hashIndex].myState == eState::InUse)
			{
				return &myArray[hashIndex].myValue;
			}

			if (++hashIndex >= myCapacity)
			{
				hashIndex -= myCapacity;
			}
		}
		while (myArray[hashIndex].myState == eState::InUse);

		return nullptr;
	}
}

namespace CU = CommonUtilities;