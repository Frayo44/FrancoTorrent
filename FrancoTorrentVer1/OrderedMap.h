#pragma once

#include <string>

struct Value;


class OrderedMap
{
	
public:

	OrderedMap(void) 
	{}

	void Insert(std::string key, Value * value) 
	{
		auto val = make_pair(key, value);
		// insert in map
		auto pr = dictionary.insert(val);

		Value * valuePtr = dictionary[key];


		auto find_it = dictionary.find(key);
	
		dictPtrs.push_back(val);
	};

	Value * GetValueByKey(std::string key)
	{
		Value * v = dictionary[key];

		return v;
	};

	Value * GetValueByIndex(int index)
	{
		return  dictPtrs[index].second;
	};

	std::string GetKeyByIndex(int index)
	{
		return dictPtrs[index].first;
	};

	int GetSize()
	{
		return dictPtrs.size();
	};

	void RemoveFromVectorByIndex(int index)
	{
		assert(index < 0 || index > GetSize());

		dictPtrs.erase(dictPtrs.begin() + index);
	}
	

private:


	typedef std::map<std::string, Value *> DictionaryMap;
	DictionaryMap dictionary;

	typedef std::pair<std::string, Value*> PairType;
	std::vector<PairType> dictPtrs;

};
