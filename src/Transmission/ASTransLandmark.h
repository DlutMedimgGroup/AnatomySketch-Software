#pragma once
#include "ASTransmissionBase.h"
#include "ASFModelLandmark.h"
#include <vtkActor.h>

class ASLandmarkData;
class ASAbstractData;

#define CtrOpe_AddLandM			0
#define CtrOpe_MoveChooseLandM	1
#define CtrOpe_MovingLandM		2
#define CtrOpe_DeleteLandM		3
#define CtrOpe_ZoomLandM		4

class ASTransLandmark : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransLandmark(QObject *parent = 0);
	~ASTransLandmark();

	// 唯一对象
	static ASTransLandmark* GetSelfPointer();

	// 得到当前标定点
	static ASLandmarkData* GetCurrentLMData();
	// 新建轮廓线文件
	static void NewLandmarkData(ASAbstractData* pParent);
	// 初始化轮廓线管线
	static void InitLandmarkPipeline();
	// 设置模块是否使能
	static void SetModelEnabled(const bool c_enable);
	// 标定点是否显示
	static void SetLandmarksShown(const bool c_Shown);
	// 标定点操作
	static void LandmarkOperation(double* pos, int viewlabel, int operate, vtkActor* actor);
	// 刷新
	static void RefreshLandMDisplay();
	// 从硬盘读文件
	static void LoadLandMDataFromDisk(const QStringList c_FileNameList);
	// 移除标定点
	static void RemoveLandMark(const int c_LMDataNodeID, const int c_LMID);

private:

	// 唯一对象
	static ASTransLandmark* ms_SelfPointer;
	// 标定点模块
	ASFModelLandmark* m_FModelLandmark;

signals :

	// 设置界面的模块使能状态
	void signalSetModelEnabled(bool enabled);

};
