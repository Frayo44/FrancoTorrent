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

	int lastIndex;
	int lastOffset;
	int lastFileSize;

private:
	bool begin;
	bool finishedDownload;
	bool inDownload;

public:
	PieceItem(int startIndex, int endIndex, int beginOffSet, int endOffSet, int fileSize, std::string fileName, int fileIndex)
		: startIndex(startIndex), endIndex(endIndex), beginOffSet(beginOffSet), endOffSet(endOffSet), fileSize(fileSize), fileName(fileName), fileIndex(fileIndex),
		begin(false), finishedDownload(false), lastIndex(startIndex), lastOffset(beginOffSet), lastFileSize(fileSize), inDownload(false)
	{
	
	};

	bool isBegin()
	{
		return begin;
	}

	void setBegin(bool begin)
	{
		this->begin = begin;
	}

	bool isFinished()
	{
		return finishedDownload;
	}

	void setFinish(bool finished)
	{
		this->finishedDownload = finished;
	}

	bool isInDownload()
	{
		return inDownload;
	}

	void setInDownload(bool inDownload)
	{
		this->inDownload = inDownload;
	}

	
};