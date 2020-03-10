#pragma once

#include "ASImageData.h"
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <QColor>

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

class ASPolyImageData : public ASImageData
{
public:
	ASPolyImageData();
	~ASPolyImageData();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);

	// 得到图像数据
	QColor getColor();
	void setColor(const QColor c_Color); // 设置颜色
	int getOpacity();
	void setOpacity(const int opacity);
	vtkPolyData* getPolyImageData();
	void setPolyData(vtkPolyData* pPolyData);
	// 读入数据后的数据初始化
	void DataInitAfterLoad();

	//// 曲面形变模块
	//void setPolyDeformationChoosen(const bool isChoosen);
	//bool isPolyDeformationChoosen();

protected:

	QColor m_Color; 
	int	m_nOpacity; // 不透明度,0~100
	vtkSmartPointer<vtkPolyData> m_PolyData = nullptr; // 储存面数据

private:

	// 曲面形变功能成员
	bool m_isPolyDeformationChoosen = false;


};
