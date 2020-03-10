#pragma once

#include "ASAddinsInputNode.h"
#include <QComboBox>
#include <vtkImageData.h>

class ASPolyImageData;

class ASAddinsInputPolyImage : public ASAddinsInputNode
{
public:
	ASAddinsInputPolyImage(ASAddinsBase* parent);
	~ASAddinsInputPolyImage();

	// DataNode
	void SetPolyData(ASPolyImageData* data);
	// Combobox
	QComboBox* GetCombobox();
	// ImageData
	ASPolyImageData* GetPolyData();
	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	// 保存输入到Python程序中的临时文件.1成功，-1失败
	virtual int PythonInputGenerate();
	// 运算后释放数据
	virtual void FreeMemory();

	virtual void RefreshCombo(int datatype);

private:
	
	QComboBox* m_Combobox = nullptr;
	ASPolyImageData* m_PolyData = nullptr;

	float* m_PointsArray = nullptr;
	int m_PointsSize;
	int* m_Edges = nullptr;
	int* m_EdgesGroup = nullptr;
	int m_EdgesGroupSize;


};
