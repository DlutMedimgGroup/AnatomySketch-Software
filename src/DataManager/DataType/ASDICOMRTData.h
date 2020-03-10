#pragma once

#include "ASMarkData.h"
#include <QList>

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

typedef struct structDICOMRTDataSlice {
	double layer;
	QList<double> points;
	double LayerDataBounds[4]; //因为数据都是横截面上的，所以为横截面上一层的边界。xmin,xmax,ymin,ymax。
}structDICOMRTDataSlice;

class ASDICOMRTData : public ASMarkData
{
public:
	ASDICOMRTData();
	~ASDICOMRTData();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);

	// 添加一个闭合曲线
	void appendSlice(const double* c_pdPoints, const int c_nNumOfPoints);
	// slice数量
	int getNumOfSlice();
	// 获取Slice
	structDICOMRTDataSlice* getSliceAt(int index);

	// 静态函数
	static int getNumOfDICOMRTData();

	// 静态函数
	// 得到所有DICOM-RT数据的范围，返回0成功，返回-1表示没有RT数据或只有空数据
	static int getDICOMRTDataBounds(double* pdBounds);

private:
	// 储存各闭合曲线
	QList<structDICOMRTDataSlice*>* m_ListRTDataSlice;

	static int ms_nNumOfDICOMRTData;				// 当前打开的RT数据的数量

};
