#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <assert.h>

struct Value;


class OrderedMap
{

private:

	typedef std::map<std::string, Value *> DictionaryMap;
	DictionaryMap dictionary;

	typedef std::pair<std::string, Value*> PairType;
	std::vector<PairType> dictPtrs;
	
public:

	OrderedMap(void);

	void Insert(std::string key, Value * value);

	Value * GetValueByKey(std::string key);

	Value * GetValueByIndex(int index);

	std::string GetKeyByIndex(int index);

	int GetSize();

	void RemoveFromVectorByIndex(int index);
	



};
