#pragma once
#include <QString>
#include <QColor>

// ROI类型
#define ROIType_Base		0
#define ROIType_Cube		1

class ASROITypeBase
{
public:
	ASROITypeBase();
	~ASROITypeBase();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);
	int GetROIType();
	// 得到数据中心
	virtual void GetCenter(double* center);

	// ROIID
	void SetROIID(const int c_nID);
	int GetROIID();
	// Name
	void SetName(const QString c_Name);
	QString GetName();
	// Shown
	void SetShown(const bool c_shown);
	bool isShown();
	// Color
	void SetColor(QColor color);
	QColor GetColor();

protected:

	int	m_ROIType;	// 数据类型
	int m_ROIID;	// 同一个ROI文件中的ID各不相同
	QString m_Name; // 名称
	bool m_isShown = true;
	QColor m_Color;
};
