#pragma once

#include "ASAddinsInputNode.h"
#include <vtkImageData.h>
#include <QComboBox>

class ASAddinsComboBox;
class ASArrayImageData;

class ASAddinsInputMask : public ASAddinsInputNode
{
public:
	ASAddinsInputMask(ASAddinsBase* parent);
	~ASAddinsInputMask();

	// DataNode
	void SetImageData(ASArrayImageData* data);
	// Combobox
	QComboBox* GetCombobox();
	// ImageData
	ASArrayImageData* GetImageData();
	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	// 保存输入到Python程序中的临时文件.1成功，-1失败
	virtual int PythonInputGenerate();
	// 运算后释放数据
	virtual void FreeMemory();
	// Combo
	virtual void RefreshCombo(int datatype);
	// ComboBox回调函数
	void ComboBoxCallBack(ASAddinsComboBox* sender);
	// 新建遮罩完成回调函数
	void NewMaskFinishCallBack();

private:
	
	ASAddinsComboBox* m_Combobox = nullptr;
	ASArrayImageData* m_ImageData = nullptr;
	int m_Dim[3];
	double m_Origin[3];
	double m_Spacing[3];
	double m_WinLevel[2];

	float* m_Data = nullptr;
	QString m_CallbackName;

};
