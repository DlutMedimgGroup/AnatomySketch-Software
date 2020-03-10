#pragma once

#include "ASMarkData.h"
#include "GlobelDeclaration.h"

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

typedef struct structCurve {
	QList<structPoint*> pvectorPoint;
	int ViewLabel; //0:sagittal  1 : coronal   2 : axial DimensionsOfImage
	double dLayer;
	bool isClosure; // 是否闭合
	bool haveTemporary = false;
}structCurve;
typedef struct structContour {
	int contourID;
	bool isShown;
	QColor color;
	QString name;
	int label;
	QList<structCurve*>* pListCurves;
}structContour;

class ASContourData : public ASMarkData
{
public:
	ASContourData();
	~ASContourData();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);
	// 新建轮廓线
	int NewContour(const QString c_name, const int c_lable, const QColor c_color);
	// 轮廓线数据数量
	static int GetNumOfContourData();
	// 标签值
	int getLabel();
	void setLabel(const int label);
	// 访问轮廓线
	int GetNumberOfContour();
	structContour* GetContourAt(const int c_Contour);
	structContour* GetContourFromID(const int c_ContourID);
	void AppandContour(structContour* NewContour);
	// 属性
	virtual double* getDataExtent(); // 得到数据范围，double[6]

	// 轮廓线操作
	void AddContrlPointsTemp(double* pPosition, const int ViewLabel, 
		double* pSpacing, const int c_ContourID);
	void AddContrlPoints(double* pPosition, const int ViewLabel, double* pSpacing, const int c_ContourID);
	void MoveContrlPoints(double* pPosition, const int c_ViewLabel, double* pSpacing, const int c_ContourID,
		const int c_IndexOfContour = -1, const int c_IndexOfHandle = -1);
	void DeleteContrlPointsTemp(double* pPosition, const int c_ViewLabel, double* pSpacing, 
		const int c_ContourID);
	void SetContourClosed(const int c_viewlabel, const int c_indexOfContour, const int c_indexOfHandle, 
		const bool c_Closed, const int c_ContourID);
	void DeleteContrlPoints(const int c_ViewLabel, double* pSpacing, const int c_ContourID, 
		const int c_IndexOfContour, const int c_IndexOfHandle);
	void InsertContrlPoints(double* pPosition, const int c_ViewLabel, double* pSpacing, 
		const int c_ContourID, const int c_IndexOfContour, const int c_IndexOfHandle);
	void DeleteContour(const int c_viewlabel, const int c_indexOfContour, const int c_indexOfHandle,
		const int c_ContourID);
	void DeleteWholeContour(const int c_ContourID);

protected:

	int m_nLabel; // 标签值

private:

	QList<structContour*>* m_listContours;

	static int ms_nNumOfContourData;				// 当前打开的轮廓线数据的数量

	// 得到一个新的不重复ID
	int GetNewContourID();
};
