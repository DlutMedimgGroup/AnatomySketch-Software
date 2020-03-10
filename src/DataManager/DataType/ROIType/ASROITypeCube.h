#pragma once

#include "ASROITypeBase.h"

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

class ASROITypeCube : public ASROITypeBase
{
public:
	ASROITypeCube();
	~ASROITypeCube();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);

	virtual void GetCenter(double* center);
	// 范围
	void SetRange(double xmin, double xmax, double ymin, 
		double ymax, double zmin, double zmax);
	void SetRange(double* range);
	void SetRange(const int c_Index, const double c_Value);
	double* GetRange();
	void GetRange(double* range);

protected:

	double m_Range[6];

private:

};
