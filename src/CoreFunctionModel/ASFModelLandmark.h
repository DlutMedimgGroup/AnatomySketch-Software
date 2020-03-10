#pragma once
#include "ASFunctionModelBase.h"
#include <vtkActor.h>

class ASPipelineLandmark;

class ASFModelLandmark : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelLandmark(QObject *parent = 0);
	~ASFModelLandmark();

	//// 当前绘制的标定点的ID
	//void SetIDOfPaintingData(const int c_ID);
	//static int GetIDOfPaintingData();
	// 模块使能状态
	void SetLandmarkModleEnabled(const bool c_enabled);
	static bool GetLandmarkModleEnabled();
	// 没建立管线就建立管线
	void InitLandmarkPipeline();
	// 轮廓线管线是否显示，没有2D内容时不显示种子点
	void SetLandmarkShown(const bool c_Shown);

	// 标定点操作
	// 添加标定点
	void AddLandmark(double* pPosition, QString qsname);
	// 移动标定点
	void MoveLandmark(double* pPos, const int c_DataID, const int c_ViewLabel, const int c_PointID);
	// 删除标定点
	void DeleteLandmark(const int c_DataID, const int c_PointID);

	// 设置控制点直径
	void ReSetCtrlRadius(const int c_ViewLabel);
	// 显示
	void RefreshLMDisplay(const int c_ViewLabel);
	// 根据actor地址查询控制点信息
	bool ReserchLanmark(vtkActor* pActor, int* LandmarkDataID, int* ViewLabel, int* LandmarkPointID);

	// 读取种子点数据
	void LoadLandmarkData(const QString c_FileName);

private:
	// 唯一对象
	static ASFModelLandmark* ms_SelfPointer;

	// 模块使能状态
	bool m_LandmarkModelEnabled = false;
	// 标定点模块是否显示
	bool m_LandmarkShown = false;

	// 管线
	ASPipelineLandmark* m_PipelineLandmark = nullptr;

private slots:
	

};
