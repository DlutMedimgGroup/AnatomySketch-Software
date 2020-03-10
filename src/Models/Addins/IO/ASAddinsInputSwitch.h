#pragma once

#include "ASAddinsInputBase.h"
#include <QComboBox>

class ASAddinsInputSwitch : public ASAddinsInputBase
{
public:
	ASAddinsInputSwitch(ASAddinsBase* parent);
	~ASAddinsInputSwitch();

	// Value
	virtual void SetValue(const QStringList c_value);
	// QComboBox
	QComboBox* GetComboBox();
	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);

private:

	QComboBox* m_ComboBox = nullptr;
	int m_dPara;


};
