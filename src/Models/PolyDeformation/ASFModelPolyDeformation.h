#pragma once
#include "ASFunctionModelBase.h"
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <QList>

#define PolyDeformation_Panning		0
#define PolyDeformation_Rotation	1
#define PolyDeformation_Zooming		2
#define PolyDeformation_FFD			3
#define PolyDeformation_None		4

#define  MaximumWithdrawalSteps		7		//最大撤销次数

// --------------------- 缓存数据，用于撤销 ---------------------
typedef struct structTemporaryPolyDataSet
{
	int ID;
	vtkSmartPointer<vtkPolyData> pPolyData;
}structTemporaryPolyDataSet;

typedef struct structPolyDeformationTemporaryData
{
	int		DeformaitonType;
	QList<structTemporaryPolyDataSet*>* pTemporaryDataSet;
}structPolyDeformationTemporaryData;

class ASFModelPolyDeformation : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelPolyDeformation(QObject *parent = 0);
	~ASFModelPolyDeformation();

	// 选择
	void SetDataNodeChoosen(const int c_ID, const bool c_Choosen);
	void ResetDataNodeChoosen(QList<int> choosenID);
	static bool IsPolyChoosen(const int c_ID);
	// 当前变换
	int GetCurrentTransType();

	// 开始变换
	void StartPolyDeformation(double* Pos, const int c_Type, double ViewRange);
	// 继续变换
	int ContinuePolyDeformation(double* Pos, const int c_Viewlabel, const int c_Type);
	// 缩放
	void ContinueZoom(const double c_mX, const double c_mY, const double c_mZ);
	// 旋转
	void ContinueRotation(const double angle, const int viewlabel);
	// 改变FFD影响半径
	int FFDRadiusChange(int ZoominOrZoomout);
	// 撤销
	int UndoDef();


private:
	// 唯一对象
	static ASFModelPolyDeformation* ms_SelfPointer;
	// 当前选中曲面ID列表
	QList<int> m_listChoosenID;

	// 变换坐标信息
	double m_adPolyDeformationPos[6];
	// 当前视图范围
	double m_dViewRange = 0;
	// FFD当前作用半径
	double m_FFDRadius = 0;
	// 数据旋转或缩放的中心
	double m_PolyDeformationDataCenter[3];
	// 当前变换
	int m_CurrentTransType = PolyDeformation_None;
	// 缓存数据存储链表
	QList<structPolyDeformationTemporaryData*>* m_listPolyDeformationTemporaryData;

	// 进行平移变换
	int PanningDeformation();
	// 进行旋转变换，鼠标交互
	int RotationDeformation(int viewlabel);
	// 进行FFD变换
	int FFDDeformation();

private slots:
	

};
