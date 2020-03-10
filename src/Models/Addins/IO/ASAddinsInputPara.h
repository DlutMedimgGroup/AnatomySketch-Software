#pragma once

#include "ASAddinsInputBase.h"
#include <QDoubleSpinBox>

class ASAddinsInputPara : public ASAddinsInputBase
{
public:
	ASAddinsInputPara(ASAddinsBase* parent);
	~ASAddinsInputPara();

	// Value
	virtual void SetValue(const QStringList c_value);
	// QDoubleSpinBox
	QDoubleSpinBox* GetDouSpinBox();
	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	// 保存输入到Python程序中的临时文件.1成功，-1失败
	virtual int PythonInputGenerate();
	// Para
	void SetParaValue(const double c_value);

private:
	
	QDoubleSpinBox* m_DouSpinBox = nullptr;
	double m_dPara;

};
