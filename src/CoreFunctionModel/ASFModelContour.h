#pragma once
#include "ASFunctionModelBase.h"
#include <vtkActor.h>

class ASPipelineContour;

class ASFModelContour : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelContour(QObject *parent = 0);
	~ASFModelContour();

	// 当前绘制的种子点的ID
	void SetIDOfPaintingData(const int c_ID);
	static int GetIDOfPaintingData();
	// 当前绘制数据节点中轮廓线的序号
	void SetCurrentContourID(const int c_ID);
	static int GetCurrentContourID();
	// 模块使能状态
	void SetContourModleEnabled(const bool c_enabled);
	static bool GetContourModleEnabled();
	// 没建立管线就建立管线
	void InitContourPipeline();
	// 轮廓线管线是否显示，没有2D内容时不显示种子点
	void SetContourShown(const bool c_Shown);
	// 交互模式。0, 普通; 1, 触控
	void SetInterfaceModel(const int c_model);
	static int GetInterfaceModel();

	// 轮廓线操作
	// 添加临时控制点
	void AddContrlPointsTemp(double* pPosition, const int ViewLabel, double* pSpacing);
	// 添加控制点
	void AddContrlPoints(double* pPosition, const int ViewLabel, double* pSpacing);
	// 移动控制点
	void MoveContrlPoints(double* pPosition, const int c_ViewLabel, double* pSpacing, const int c_ID = -1, 
		const int c_ContourID = -1, const int c_IndexOfContour = -1, const int c_IndexOfHandle = -1);
	// 闭合轮廓线
	void SetContourClosed(const int c_id, const int c_viewlabel, const int c_indexOfContour, const int c_indexOfHandle, const bool c_Closed);
	// 删除控制点
	void DeleteContrlPoints(const int c_ViewLabel, double* pSpacing, const int c_ID, const int c_ContourID,
		const int c_IndexOfContour, const int c_IndexOfHandle);
	// 删除最后一个控制点
	void DeleteContrlPointsTemp(double* pPosition, const int c_ViewLabel, double* pSpacing);	
	// 插入控制点
	void InsertContrlPoints(double* pPosition, const int c_ViewLabel, double* pSpacing, const int c_ID, const int c_IndexOfContour, const int c_IndexOfHandle);
	// 删除轮廓
	void DeleteContour(const int c_id, const int c_ContourID, const int c_viewlabel, 
		const int c_indexOfContour, const int c_indexOfHandle);
	// 删除整个轮廓数据
	void DeleteWholeContour(const int c_DataID, const int c_ContourID);
	// 设置控制点直径
	void ReSetCtrlRadius(const int c_ViewLabel);
	// 显示
	void RefreshSplineContourDisplay(const int c_ViewLabel);
	// 根据actor地址查询控制点信息
	bool ReserchHandleSplineContour(vtkActor* pActor, int* ID, int* ContourID, int* ViewLabel, 
		int* ItemOfContour, int* ItemOfHandle);
	// 插入控制点时根据坐标判断控制点位置
	bool GetInsertHandleIndex(double* pos, const int c_viewLabel, int* ID, int* ItemOfContour, int* ItemOfHandle);

	// 读取轮廓线数据
	void LoadContourData(const QString c_FileName);

private:
	// 唯一对象
	static ASFModelContour* ms_SelfPointer;

	// 当前绘制轮廓线数据节点的ID
	int m_IDOfPaintingData = -1;
	// 当前绘制数据节点中轮廓线的序号
	int m_CurrentContourID = -1;
	// 模块使能状态
	bool m_ContourModelEnabled = false;
	// 轮廓线模块是否显示
	bool m_ContourShown = false;
	// 交互模式。0, 普通; 1, 触控
	int m_InterfaceModel = 0;

	// 管线
	ASPipelineContour* m_PipelineContour = nullptr;

private slots:
	

};
