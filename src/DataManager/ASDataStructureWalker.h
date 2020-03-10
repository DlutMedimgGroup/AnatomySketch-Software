#pragma once

//#include "common.h"
#include <queue>
#include "ASAbstractData.h"

// 用于数据存储结构的遍历
// 例如 for (ASAbstractData* item = ASDataStructureWalker::bigen_single(DataType_ArrayImageData); item != nullptr; item = ASDataStructureWalker::next_single(DataType_ArrayImageData)){}
//      for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_ArrayImageData); item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_ArrayImageData)){}

class ASDataStructureWalker
{
public:
	ASDataStructureWalker(ASAbstractData* rootNode);
	~ASDataStructureWalker();

	static ASAbstractData* bigen_inherit(int type);
	static ASAbstractData* bigen_single(int type);
	static ASAbstractData* next_inherit(int type);
	static ASAbstractData* next_single(int type);
private:

	//对树先根遍历所使用的队列FIFO
	static std::queue<ASAbstractData*>* ms_pQueue;
	static ASAbstractData* ms_pRootNode;

};
