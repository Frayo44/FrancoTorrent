#include "OrderedMap.h"

OrderedMap::OrderedMap(void)
{

}

void OrderedMap::Insert(std::string key, Value * value)
{
	auto val = make_pair(key, value);
	// insert in map
	auto pr = dictionary.insert(val);

	Value * valuePtr = dictionary[key];


	auto find_it = dictionary.find(key);

	dictPtrs.push_back(val);
};

Value * OrderedMap::GetValueByKey(std::string key)
{
	Value * v = dictionary[key];

	return v;
};

Value * OrderedMap::GetValueByIndex(int index)
{
	return  dictPtrs[index].second;
};

std::string OrderedMap::GetKeyByIndex(int index)
{
	return dictPtrs[index].first;
};

int OrderedMap::GetSize()
{
	return dictPtrs.size();
};

void OrderedMap::RemoveFromVectorByIndex(int index)
{
	assert(index < 0 || index > GetSize());

	dictPtrs.erase(dictPtrs.begin() + index);
}
