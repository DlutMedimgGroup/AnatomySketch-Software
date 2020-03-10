#pragma once

#include "ASAddinsInputNode.h"
#include <QComboBox>
#include <QString>
#include <QColor>

class ASSeedData;
class ASAddinsComboBox;

typedef struct structInputSeed
{
	QString Name;
	int Label;
	QColor Color;
	ASAddinsComboBox* ComboBox;
}structInputSeed;

class ASAddinsInputSeed : public ASAddinsInputNode
{
public:
	ASAddinsInputSeed(ASAddinsBase* parent);
	~ASAddinsInputSeed();

	// Value
	virtual void SetValue(const QStringList c_value);
	// DataNode
	void SetSeedData(ASSeedData* datanode);
	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	// 保存输入到Python程序中的临时文件.1成功，-1失败
	virtual int PythonInputGenerate();
	// 运算后释放数据
	virtual void FreeMemory();
	// InputSeeds
	int GetNumOfInputSeed();
	structInputSeed* GetInputSeedAt(const int c_index);
	virtual void RefreshCombo(int datatype);
	// ComboBox回调函数
	void ComboBoxCallBack(ASAddinsComboBox* sender);

private:
	
	ASSeedData* m_SeedData = nullptr;
	int m_Size = 0;
	float* m_Data = nullptr;

	QList<structInputSeed*>* m_listInputSeed;

	int UpdatemData();
};
