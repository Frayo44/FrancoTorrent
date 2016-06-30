#pragma once

struct PieceItem
{
	int startIndex;
	int beginOffSet;
	int endOffSet;
	int endIndex;
	int fileSize;
	std::string fileName;
	int fileIndex;

public:
	PieceItem(int startIndex, int endIndex, int beginOffSet, int endOffSet, int fileSize, std::string fileName, int fileIndex)
		: startIndex(startIndex), endIndex(endIndex), beginOffSet(beginOffSet), endOffSet(endOffSet), fileSize(fileSize), fileName(fileName), fileIndex(fileIndex)
	{

	};
};