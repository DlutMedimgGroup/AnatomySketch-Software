#pragma once

#include "ASAddinsOutputNode.h"

class ASAddinsOutputArrayImage : public ASAddinsOutputNode
{
public:
	ASAddinsOutputArrayImage(ASAddinsBase* parent);
	~ASAddinsOutputArrayImage();

	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	virtual void GenerateFinished(const int InterfaceType = 0);

	// 运算后释放数据
	virtual void FreeMemory();

	float* GetArray();
	int* GetDim();
	double* GetSpacing();
	double* GetOrigin();

private:
	
	float** m_pArray = nullptr;
	int* m_Dim = nullptr;
	double* m_Spacing = nullptr;
	double* m_Origin = nullptr;

};
