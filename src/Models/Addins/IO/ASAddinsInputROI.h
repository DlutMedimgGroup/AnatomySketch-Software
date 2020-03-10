#pragma once

#include "ASAddinsInputNode.h"
#include <QComboBox>

class ASROIData;
class ASAddinsComboBox;

typedef struct structInputROI
{
	QString Name;
	int Label;
	QColor Color;
	bool autoROI; // 是否支持自动生成ROI
	ASAddinsComboBox* ComboBox;
}structInputROI;

class ASAddinsInputROI : public ASAddinsInputNode
{
public:
	ASAddinsInputROI(ASAddinsBase* parent);
	~ASAddinsInputROI();

	// Value
	virtual void SetValue(const QStringList c_value);
	// DataNode
	void SetROIData(ASROIData* datanode);
	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	// 保存输入到Python程序中的临时文件.1成功，-1失败
	virtual int PythonInputGenerate();
	// 运算后释放数据
	virtual void FreeMemory();
	// InputLandMark
	int GetNumOfInputROI();
	structInputROI* GetInputROIAt(const int c_index);
	virtual void RefreshCombo(int datatype);
	// ComboBox回调函数
	void ComboBoxCallBack(ASAddinsComboBox* sender);

private:
	
	ASROIData* m_ROIData = nullptr;
	float* m_Data = nullptr;
	int m_CubeSize = 0;

	QList<structInputROI*>* m_listInputROI;
};
