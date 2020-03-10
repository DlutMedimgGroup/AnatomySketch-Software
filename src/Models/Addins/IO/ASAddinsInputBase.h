#pragma once
#include "ASAddinsIOBase.h"
#include <QStringList>

class ASAddinsInputBase : public ASAddinsIOBase
{
public:
	ASAddinsInputBase(ASAddinsBase* parent);
	~ASAddinsInputBase();

	virtual bool isInput();
	virtual void SetValue(const QStringList c_value);
	virtual void RefreshCombo(int datatype);


private:


};
