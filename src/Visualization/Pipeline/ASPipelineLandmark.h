#pragma once
#include "ASPipelineBase.h"
#include <vtkSmartPointer.h>
#include <vtkSphereHandleRepresentation.h>
#include <vtkRenderer.h>
#include <QList>
#include <QColor>

#define	LANDMARKRADIUS				10		// 标定点半径

// 标定点显示管线结构体
typedef struct structLandmarkPipeline
{
	int LandmarkDataID;
	int LandmarkPointID;
	int ViewLabel;
	QString qsName;
	QColor qColor;
	vtkSmartPointer<vtkSphereHandleRepresentation> SphereHandleRep;
}structLandmarkPipeline;

class ASPipelineLandmark : public ASPipelineBase
{
public:
	ASPipelineLandmark(const int c_ID);
	~ASPipelineLandmark();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type);
	virtual bool isTypeAbsolute(const int c_Type);

	// 虚函数
	virtual void DeleteNode();	// 调用此函数来释放管线
	virtual void Update2DTransformMatrix(); // 切图焦点改变
	virtual void SetShown(const bool c_Shown); // 是否显示

	// 刷新样条轮廓线显示
	void RefreshLandmarkDisplay(const int c_ViewLabel);
	// 设置控制点直径
	void ReSetCtrlRadius(const int c_ViewLabel);
	// 根据actor地址查询控制点信息
	bool ReserchLanmark(vtkActor* pActor, int* LandmarkDataID, int* ViewLabel, int* LandmarkPointID);

private:

	bool m_isShown = false;
	vtkSmartPointer<vtkRenderer> m_pRenderer2D[3];
	QList<structLandmarkPipeline*>* m_plistLandmarkPipelines;
	double m_HandleRadius[3] = { 1, 1, 1 };
	// 控制点半径
	double GetCtrlPointRadiusSplineContour(const int c_viewLabel);
};
