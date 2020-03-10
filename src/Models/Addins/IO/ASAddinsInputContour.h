#pragma once

#include "ASAddinsInputNode.h"
#include <QComboBox>

class ASContourData;
class ASAddinsComboBox;

typedef struct structInputContour
{
	QString Name;
	int Label;
	QColor Color;
	ASAddinsComboBox* ComboBox;
}structInputContour;

class ASAddinsInputContour : public ASAddinsInputNode
{
public:
	ASAddinsInputContour(ASAddinsBase* parent);
	~ASAddinsInputContour();

	// Value
	virtual void SetValue(const QStringList c_value);
	// DataNode
	void SetContourData(ASContourData* datanode);
	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	// 保存输入到Python程序中的临时文件.1成功，-1失败
	virtual int PythonInputGenerate();
	// 运算后释放数据
	virtual void FreeMemory();
	// InputContours
	int GetNumOfInputContour();
	structInputContour* GetInputContourAt(const int c_index);
	virtual void RefreshCombo(int datatype);
	// ComboBox回调函数
	void ComboBoxCallBack(ASAddinsComboBox* sender);

private:
	
	ASContourData* m_ContourData = nullptr;
	// 用于DLL的数据
	float* m_Data = nullptr;
	int m_Size = 0;
	QList<structInputContour*>* m_listInputContour;

};
