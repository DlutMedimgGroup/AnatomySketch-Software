#pragma once
#include "ASTransmissionBase.h"
#include <vtkActor.h>

class ASAbstractData;
class ASArrayImageData;
class ASPolyImageData;
class ASFModelGeneralInteractor;

class ASTransGeneralInteractor : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransGeneralInteractor(QObject *parent = 0);
	~ASTransGeneralInteractor();

	// 唯一对象
	static ASTransGeneralInteractor* GetSelfPointer();
	// 切换3D视图2D截面是否显示，返回切换后是否显示
	static bool Switch3DImageStackShown();
	// 设置显示焦点，渲染
	static void SetVisualizationPosition(const double c_x, const double c_y, const double c_z);
	// 设置当前帧
	static void SetVisualizationFrame(const int c_frame);
	// 设置当前图像窗位窗宽
	static void SetVisualizationLevelWindow(const double c_level, const double c_window);
	// 设置某数据是否显示
	static void SetDateShown(const int c_ID, const bool c_isShown);
	// 重置数据
	static void ReinitDataNode(ASAbstractData* DataNode);
	// 设置体数据的颜色表
	static void SetArrayDataColorMap(ASArrayImageData* DataNode, const int c_ColorMap);
	// 设置体数据窗位窗宽
	static void SetArrayDataWindowLevel(ASArrayImageData* DataNode, double* WindowLevel);
	// 设置面数据不透明度
	static void SetPolyDataOpacity(ASAbstractData* DataNode, const int c_Opacity);
	// 设置面数据颜色
	static void SetPolyDataColor(ASPolyImageData* DataNode, const QColor c_Color);
	// 视图缩放
	static void GenerateViewZoom(const int c_view);

	// 设置显示焦点，不渲染
	static void fSetVisualizationPosition(const double c_x, const double c_y, const double c_z);

	// 悬停信息
	static void HoverActorPicked(vtkActor* actor, const int c_x, const int c_y, const int c_ViewLabel);
	static void AddHoverInfor(vtkActor* actor, QString infor);
	static void DeleteHoverInfor(vtkActor* actor);
	static int ShowHoverText(const int c_ViewLabel, const QString c_text, double* pos1, double* pos2);
	static void DeleteHoverText(const int c_TextID, const int c_ViewLabel);

	static void SetWindow(double m_Window);
	static void SetLevel(double m_Level);

	static double GetWindow();
	static double GetLevel();

private:
	// 窗位窗宽
	double WindowLevel[2];

	// 唯一对象
	static ASTransGeneralInteractor* ms_SelfPointer;

	// 功能模块
	ASFModelGeneralInteractor* m_FModelGenInter;

	// 设置是否显示
	void fSetDataShown(ASAbstractData* DataNode, const bool c_isShown);

signals:

	// 数据是否显示改变
	void signalDataShownChanged(ASAbstractData* DataNode);
};
