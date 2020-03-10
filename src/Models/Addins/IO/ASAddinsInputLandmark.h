#pragma once

#include "ASAddinsInputNode.h"
#include <QComboBox>

class ASLandmarkData;
class ASAddinsComboBox;

typedef struct structInputLandMark
{
	QString Name;
	int Label;
	QColor Color;
	ASAddinsComboBox* ComboBox;
}structInputLandMark;

class ASAddinsInputLandmark : public ASAddinsInputNode
{
public:
	ASAddinsInputLandmark(ASAddinsBase* parent);
	~ASAddinsInputLandmark();

	// Value
	virtual void SetValue(const QStringList c_value);
	// DataNode
	void SetLandmarkData(ASLandmarkData* datanode);
	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	// 保存输入到Python程序中的临时文件.1成功，-1失败
	virtual int PythonInputGenerate();
	// 运算后释放数据
	virtual void FreeMemory();
	// InputContours
	int GetNumOfInputLandMark();
	structInputLandMark* GetInputLandMarkAt(const int c_index);
	virtual void RefreshCombo(int datatype);
	// ComboBox回调函数
	void ComboBoxCallBack(ASAddinsComboBox* sender);

private:
	
	ASLandmarkData* m_LandmarkData = nullptr;	
	QList<structInputLandMark*>* m_listInputLandMark;
	// 用于DLL的数据
	float* m_Data = nullptr;
	int m_Size = 0;

};
