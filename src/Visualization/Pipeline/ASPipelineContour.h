#pragma once
#include "ASPipelineBase.h"
#include <QList>
#include <QColor>
#include <vtkSphereHandleRepresentation.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkProperty.h>
#include <vtkActor.h>

#define	CTRLPOINTRADIUS				10		// 样条轮廓线控制点半径

//轮廓线显示管线结构体，CTRS：ContourSplin
typedef struct structCTRSPipe {
	int											ID;
	int											ContourID;
	int											ViewLabel;
	int											ItemOfContour; // 该轮廓在链表中的序号
	QList<vtkSphereHandleRepresentation*>*		plistContourSplinSphereHandle;
	vtkSmartPointer<vtkPoints>					pCTRSPoints;
	vtkSmartPointer<vtkParametricSpline>		pCTRSParaSpline;
	vtkSmartPointer<vtkParametricFunctionSource> pCTRSParaFuncSource;
	vtkSmartPointer<vtkPolyData>				pCTRSPolyData;
	vtkSmartPointer<vtkTransform>				pCTRSTransform;
	vtkSmartPointer<vtkTransformPolyDataFilter>	pCTRSTransPolyDataFilter;
	vtkSmartPointer<vtkPolyDataMapper>			pCTRSPolyDataMapper;
	vtkSmartPointer<vtkProperty>				pCTRSProperty;
	vtkSmartPointer<vtkActor>					pCTRSActor;
}structCTRSPipe;

class ASPipelineContour : public ASPipelineBase
{
public:
	ASPipelineContour(const int c_ID);
	~ASPipelineContour();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type);
	virtual bool isTypeAbsolute(const int c_Type);

	// 虚函数
	virtual void DeleteNode();	// 调用此函数来释放管线
	virtual void Update2DTransformMatrix(); // 切图焦点改变
	virtual void SetShown(const bool c_Shown); // 是否显示

	// 是否可以拾取
	void SetPickable(const bool c_isPickable);
	// 刷新样条轮廓线显示
	void RefreshSplineContourDisplay(const int c_ViewLabel);
	// 设置控制点直径
	void ReSetCtrlRadius(const int c_ViewLabel);
	// 根据actor地址查询控制点信息
	bool reserchHandleSplineContour(vtkActor* pActor, int* ID, int* ContourID, int* ViewLabel, 
		int* ItemOfContour, int* ItemOfHandle);
	// 插入控制点时根据坐标判断控制点位置
	bool getInsertHandleIndex(double* pos, const int c_viewLabel, int* ID, int* ItemOfContour, int* ItemOfHandle);

private:

	bool m_isShown = false;
	bool m_Pickable = true;
	vtkSmartPointer<vtkRenderer>				m_pRenderer2D[3];
	QList<structCTRSPipe*>* m_plistContourSplinePipeline;
	double m_HandleRadius[3] = { 1, 1, 1 };
	
	// 控制点半径
	double GetCtrlPointRadiusSplineContour(const int c_viewLabel);
};
