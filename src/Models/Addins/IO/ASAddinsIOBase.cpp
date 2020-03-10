#include "ASAddinsIOBase.h"

ASAddinsIOBase::ASAddinsIOBase(ASAddinsBase* parent)
{
	m_Parent = parent;
}

ASAddinsIOBase::~ASAddinsIOBase()
{

}

// Type
int ASAddinsIOBase::GetType()
{
	return m_Type;
}
bool ASAddinsIOBase::isInput()
{
	return true;
}
// Name
void ASAddinsIOBase::SetName(const QString c_name)
{
	this->m_Name = c_name;
}
QString ASAddinsIOBase::GetName()
{
	return m_Name;
}
// 得到空指针，用于argc，s为描述
void* ASAddinsIOBase::GetVoidPoint(const QString s)
{
	return nullptr;
}
// 保存输入到Python程序中的临时文件
int ASAddinsIOBase::PythonInputGenerate()
{
	return -1;
}
// 运算后释放数据
void ASAddinsIOBase::FreeMemory()
{

}