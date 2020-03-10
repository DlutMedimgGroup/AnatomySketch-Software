#pragma once

#include <QObject>
#include <QWidgetAction>
#include <QPushButton>
#include <QSlider>
#include <QPoint>
#include "ASDataManagerAction.h"

class ASAbstractData;

class ASDataNodeRClickMenu : public QObject
{
	Q_OBJECT

public:
	ASDataNodeRClickMenu(QObject *parent = nullptr);
	~ASDataNodeRClickMenu();

	void Generate(const QPoint c_pos, ASAbstractData* DataNode);

private:

	// 右键菜单
	ASDataManagerAction* m_RASave;
	ASDataManagerAction* m_RARemove;
	ASDataManagerAction* m_RARemoveAll;
	ASDataManagerAction* m_RAReinit;
	ASDataManagerAction* m_RAMoveto;
	ASDataManagerAction* m_RACuttentData;
	QMenu* m_RMDataType;
	ASDataManagerAction* m_RADataType[2];
	QMenu* m_RMColorMap;
	ASDataManagerAction* m_RAColorMap[14];
	ASDataManagerAction* m_RANewAnnotation;
	ASDataManagerAction* m_RANewSeg;
	ASDataManagerAction* m_RANewImgProcess;
	ASDataManagerAction* m_RANewMeshProcess;
	QMenu* m_RMContourInterStyle;
	ASDataManagerAction* m_RAContourIS_Mouse;
	ASDataManagerAction* m_RAContourIS_MouseSerial;
	ASDataManagerAction* m_RAContourIS_Stylus;
	// 新建数据
	ASDataManagerAction* m_RANewContour;
	ASDataManagerAction* m_RANewSeeds;
	ASDataManagerAction* m_RANewROI;
	ASDataManagerAction* m_RANewLandmark;
	// 不透明度滑动条
	QWidgetAction* m_RWAOpacity;
	QSlider* m_RSliderOpacity;
	// 颜色选择
	QWidgetAction* m_RWAColor;
	QPushButton* m_RButtonColor;

	ASAbstractData* m_pGeneratingDataNode = nullptr; // 发生右键菜单的节点

	// 右键菜单初始化
	void RightClickInit();
	// 右键菜单动作
	void RightClickMenuAction(ASAbstractData* DataNode, enumActionInContextMenu action);
	// 由右键菜单动作得到颜色表序号
	int getColorMapFromAction(enumActionInContextMenu actor);

signals:


private slots:

	// 右键菜单面数据颜色和不透明度
	void slotOpacityPolyContextMenu(int value);
	void slotColorPolyContextMenu();


};
