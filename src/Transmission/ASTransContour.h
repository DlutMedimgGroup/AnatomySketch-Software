#pragma once
#include "ASTransmissionBase.h"
#include <vtkActor.h>

#define CtrOpe_Addtemp			0
#define CtrOpe_Movetemp			1
#define CtrOpe_Deletetemp		2
#define CtrOpe_Closetemp		3
#define CtrOpe_ClosetempStop	4
#define CtrOpe_Add				5
#define CtrOpe_Close			6

#define CtrOpe_MoveChoose		7 
#define CtrOpe_Moving			8 
#define CtrOpe_Delete			9 
#define CtrOpe_InsertStart		10 
#define CtrOpe_InsertContinue	11
#define CtrOpe_Refresh			12
#define CtrOpe_DeleteWhole		13

class ASAbstractData;
class ASFModelContour;
class ASContourData;

class ASTransContour : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransContour(QObject *parent = 0);
	~ASTransContour();

	// 唯一对象
	static ASTransContour* GetSelfPointer();
	//当前ContourID
	static void SetCrntContourID(const int c_id);
	static int GetCrntContourID();
	// 得到当前Contour数据
	static ASContourData* GetCurrentContourData();
	// 新建轮廓线文件
	static void NewContourData(ASAbstractData* pParent);
	static void NewContourData(ASAbstractData* pParent, QString name, int label, QColor color);
	// 刷新轮廓线相关列表
	static void UpdateListsOfContourData();
	// 初始化轮廓线管线
	static void InitContourPipeline();
	// 设置模块是否使能
	static void SetModelEnabled(const bool c_enable);
	// 轮廓线是否显示
	static void SetContourShown(const bool c_Shown);
	// 设置交互方式。0, 普通; 1, 触控
	static void SetInterfaceModel(const int c_model);
	static int GetInterfaceModel();
	// 刷新轮廓线列表
	static void RefreshContourList();
	// 设置当前正在绘制的轮廓线数据的ID
	static void SetPaintingContourID(const int c_ID);
	// 样条操作。
	static void SplineContourOperation(double* pos, int viewlabel, int operate, vtkActor* actor = nullptr);
	// 判断是否为第一个控制点
	static bool CheckFirstCtrlPoint(vtkActor* pActor, int viewlabel);
	// 刷新
	static void RefreshContourDisplay();
	// 从硬盘读文件
	static void LoadContourDataFromDisk(const QStringList c_FileNameList);
	// 视图缩放
	static void ViewZoom(const int c_view);
	// 移除轮廓线
	static void RemoveContour(const int c_ContourDataNodeID, const int c_ContourID);

private:

	// 唯一对象
	static ASTransContour* ms_SelfPointer;
	// 种子点模块
	ASFModelContour* m_FModelContour;

	int ID_Closetemp = -1;
	int ContourID_Closetemp = -1;
	int viewLabel_Closetemp = -1;
	int indexOfContour_Closetemp = -1;
	int indexOfHandle_Closetemp = -1;
	bool Closetemp = false;

signals :

	// 设置界面的模块使能状态
	void signalSetModelEnabled(bool enabled);

};
