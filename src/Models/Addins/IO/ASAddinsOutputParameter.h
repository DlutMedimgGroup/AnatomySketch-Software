#pragma once

#include "ASAddinsOutputNode.h"
#include <QLineEdit>

class ASAddinsOutputParameter : public ASAddinsOutputNode
{
public:
	ASAddinsOutputParameter(ASAddinsBase* parent);
	~ASAddinsOutputParameter();

	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	virtual void GenerateFinished(const int InterfaceType = 0);
	// QLineEdit
	QLineEdit* GetLineEdit();

private:

	QLineEdit* m_LineEdit = nullptr;
	double m_dPara;

};
