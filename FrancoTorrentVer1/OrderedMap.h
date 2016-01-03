#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>

//struct Value;

template <class Z, class T>
class OrderedMap
{

private:

	typedef std::map<Z, T> DictionaryMap;
	DictionaryMap dictionary;

	typedef std::pair<Z, T> PairType;
	std::vector<PairType> dictPtrs;
	
public:

	void Insert(Z key, T value)
	{
		auto val = make_pair(key, value);
		// insert in map
		auto pr = dictionary.insert(val);

		T valuePtr = dictionary[key];


		auto find_it = dictionary.find(key);

		dictPtrs.push_back(val);
	}


	T GetValueByKey(Z key)
	{
		T v = dictionary[key];

		return v;
	}


	T GetValueByIndex(int index)
	{
		return  dictPtrs[index].second;
	}

	Z GetKeyByIndex(int index)
	{
		return dictPtrs[index].first;
	}

	int GetSize()
	{
		return dictPtrs.size();
	}

	void Erase()
	{
		dictionary.clear();
		dictPtrs.clear();
	}

	void EraseByKey(Z key)
	{
		dictionary.erase(key);

		for (std::size_t i = 0; i < dictPtrs.size(); i++)
		{
			if (dictPtrs.at(i).first == key)
			{
				RemoveFromVectorByIndex(i);
				break;
			}

		}

	}

	void RemoveFromVectorByIndex(int index)
	{
		assert(index < 0 || index > GetSize());

		dictPtrs.erase(dictPtrs.begin() + index);
	}




};
