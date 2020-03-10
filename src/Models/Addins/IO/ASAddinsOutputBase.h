#pragma once
#include "ASAddinsIOBase.h"
#include <QString>

class ASAddinsOutputBase : public ASAddinsIOBase
{
public:
	ASAddinsOutputBase(ASAddinsBase* parent);
	~ASAddinsOutputBase();

	virtual bool isInput();
	virtual void GenerateFinished(const int InterfaceType = 0);

protected:


private:


};
