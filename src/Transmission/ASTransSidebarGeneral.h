#pragma once
#include "ASTransmissionBase.h"
#include "ASAbstractData.h"
#include <QKeyEvent>
#include <QColor>

class ASFModelMask;

#define SIDEBAR_IMAGENAVIGATOR		0	// 侧边栏，ImageNavigator
#define SIDEBAR_Mask				1	// 侧边栏，Mask
#define SIDEBAR_PaintingSeeds		2	// 侧边栏，绘制种子点
#define SIDEBAR_PaintingContour		3	// 侧边栏，绘制轮廓线
#define SIDEBAR_PaintingLandmark	4	// 侧边栏，绘制标定点
#define SIDEBAR_PolyDeformation		5	// 侧边栏，PolyDeformation
#define SIDEBAR_GraphCuts			6	// 侧边栏，GraphCuts，图割算法
#define SIDEBAR_Operation			7	// 侧边栏，插件
#define SIDEBAR_ROI					8	// 侧边栏，ROI

class ASTransSidebarGeneral : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransSidebarGeneral(QObject *parent = 0);
	~ASTransSidebarGeneral();

	// 唯一对象
	static ASTransSidebarGeneral* GetSelfPointer();

	// 设置侧边栏下方widget是否显示
	static void SetSidebarBelowWidget(const int c_WidgetType, const bool c_isShown);

	// 刷新侧边栏文件下拉选款
	static void RefreshDataComboBox(const int c_AffectType);

	// 0 当前选中的数据节点
	static void SetCrntDataNode(ASAbstractData* datanode);
	static ASAbstractData* GetCrntDataNode();

	// 1 ImageNavigator
	// 设置当前焦点
	static void SetDisplayPosition(const double c_x, const double c_y, const double c_z);
	// 设置当前帧
	static void SetDisplayFrame(const int c_Frame);
	// 设置像素尺寸
	static void SetImageSpacing(double* Spacing);
	// 设置图像范围和帧范围
	static void SetDisplayExtent(double* Extent, const int c_NumOfFrames);
	// 设置窗位窗宽
	static void SetWindowLevel(double* WindowLevel);

	// 2 Mask
	// 设置遮罩使能状态
	static void SetMaskModleEnabled(const bool c_Enable);
	// 设置参照图像ID
	static void SetMaskWorkingImageID(const int c_ID);
	// 设置遮罩参数
	static void SetMaskParameter(double* Extent, double* ScaleRange);
	// 设置遮罩模式
	static void SetMaskModel(const int c_model); // 0: 普通模式, 1: 额外输入遮罩图像模式
	static int GetMaskModel(); // 0: 普通模式, 1: 额外输入遮罩图像模式
	// 交互设置遮罩位置
	static void SetMaskInteractorly(const double c_x, const double c_y, const double c_z);
	// 设置输入遮罩图像的ID
	static void SetMaskMData(const int c_ID);

	// 3 Addins
	static void NewFromAddin(const int c_Type, QString name, int label, QColor color);

	static void BtnRst();

private:
	// 唯一对象
	static ASTransSidebarGeneral* ms_SelfPointer;

	// 当前选中的数据节点，选择不同的数据节点更改交互方式，数据焦点内具体的标签由数据节点本身记录.
	static ASAbstractData* ms_crntDataNode;

	// 功能模块
	ASFModelMask* m_FModelMask;

signals:
	
	void signalSetSideBarBelowWidget(int IDOfWidget, bool isShown); // 设置侧边栏下方widget是否显示
	void signalRefreshDataComboBox(int c_AffectType);
	// 1 ImageNavigator
	void signalSetDisplayPosition(double PosX, double PosY, double PosZ);
	void signalSetDisplayFrame(int Frame);
	void signalSetImageSpacing(double* Spacing);
	void signalSetDisplayExtent(double* Extent, const int c_NumOfFrames);
	void signalSetWindowLevel(double window, double level);
	// 2 Mask
	void signalMaskEnabledChanged(); // 遮罩模块使能状态切换
	void signalSetMaskInteractorly(double x, double y, double z);

	void signalBtnRst();
};
