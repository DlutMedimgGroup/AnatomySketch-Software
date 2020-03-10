#pragma once

#include "ASAbstractData.h"
#include <QColor>

class ASMarkData : public ASAbstractData
{
public:
	ASMarkData();
	~ASMarkData();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);

	// 颜色
	void setColor(const QColor c_Color);
	QColor getColor();

protected:
	QColor m_Color;

};
