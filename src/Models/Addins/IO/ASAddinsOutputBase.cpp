#include "ASAddinsOutputBase.h"

ASAddinsOutputBase::ASAddinsOutputBase(ASAddinsBase* parent)
	: ASAddinsIOBase(parent)
{

}

ASAddinsOutputBase::~ASAddinsOutputBase()
{

}

bool ASAddinsOutputBase::isInput()
{
	return false;
}
void ASAddinsOutputBase::GenerateFinished(const int InterfaceType)
{

}