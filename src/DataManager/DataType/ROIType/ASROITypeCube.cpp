#include "ASROITypeCube.h"

ASROITypeCube::ASROITypeCube()
{
	m_ROIType = ROIType_Cube;

}

ASROITypeCube::~ASROITypeCube()
{

}

// 调用此函数来释放节点
void ASROITypeCube::DeleteNode()
{
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASROITypeCube::isTypeInherited(const int c_Type)
{
	if (c_Type == ROIType_Cube || ASROITypeBase::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASROITypeCube::isTypeAbsolute(const int c_Type)
{
	if (c_Type == ROIType_Cube)
	{
		return true;
	}
	return false;
}
bool ASROITypeCube::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == ROIType_Cube || ASROITypeBase::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}
void ASROITypeCube::GetCenter(double* center)
{
	for (int i = 0; i < 3; i++)
	{
		center[i] = (m_Range[2 * i] + m_Range[2 * i + 1]) / 2;
	}
}
// 范围
void ASROITypeCube::SetRange(double xmin, double xmax, double ymin,
	double ymax, double zmin, double zmax)
{
	m_Range[0] = xmin;
	m_Range[1] = xmax;
	m_Range[2] = ymin;
	m_Range[3] = ymax;
	m_Range[4] = zmin;
	m_Range[5] = zmax;
}
void ASROITypeCube::SetRange(double* range)
{
	for (size_t i = 0; i < 6; i++)
	{
		m_Range[i] = range[i];
	}
}
void ASROITypeCube::SetRange(const int c_Index, const double c_Value)
{
	m_Range[c_Index] = c_Value;
}
double* ASROITypeCube::GetRange()
{
	return m_Range;
}
void ASROITypeCube::GetRange(double* range)
{
	for (size_t i = 0; i < 6; i++)
	{
		range[i] = m_Range[i];
	}
}
