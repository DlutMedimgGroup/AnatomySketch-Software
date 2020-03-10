#pragma once

#include "ASMarkData.h"
#include <QColor>

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

typedef struct structLandmark
{
	double dPosition[3];
	QString qsName;
	int LandmarkID; // 每个landmark具有唯一ID
	QColor color;
}structLandmark;

class ASLandmarkData : public ASMarkData
{
public:
	ASLandmarkData();
	~ASLandmarkData();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);

	// 标定点数据数量
	static int GetNumOfLandmarkData();
	// 访问标定点
	int GetNumberOfLandmark();
	structLandmark* GetLandmarkAt(const int c_Landmark);
	structLandmark* GetLandmarkWithID(const int c_id);
	void AppandLandmark(structLandmark* NewLandmark);
	// 轮廓线操作
	void AddLandMark(double* pPosition, QString qsname);
	void MoveLandMark(double* pPos, const int c_PointID);
	void DeleteLandMark(const int c_PointID);

	// 属性
	virtual double* getDataExtent(); // 得到数据范围，double[6]
	
protected:

	QList<structLandmark*>* m_listLandmarks;

private:

	static int ms_nNumOfLandmarkData;				// 当前打开的标定点数据的数量
};
