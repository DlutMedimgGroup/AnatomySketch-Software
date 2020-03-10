#pragma once

#include "ASAddinsOutputNode.h"

class ASAddinsOutputPolyImage : public ASAddinsOutputNode
{
public:
	ASAddinsOutputPolyImage(ASAddinsBase* parent);
	~ASAddinsOutputPolyImage();

	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	virtual void GenerateFinished(const int InterfaceType = 0);
	// 运算后释放数据
	virtual void FreeMemory();

private:
	
	float** m_pPoints = nullptr;
	int** m_pEdges = nullptr;
	int** m_pGroupEdges = nullptr;
	int m_NumOfPoints = -1;
	int m_NumOfEdges = -1;
	int m_NumOfGroup = -1;
};
