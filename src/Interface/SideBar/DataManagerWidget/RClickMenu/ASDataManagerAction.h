#pragma once

#include <QAction>

// 右键菜单中出现的项目

enum enumActionInContextMenu
{
	EMActSave,					// save
	EMActRemove,				// Remove
	EMActRemoveAll,				// RemoveAll
	EMActReinit,				// Reinit
	EMActMoveto,				// Moveto
	EMActNewAnnotation,			// NewAnnotation
	EMActNewSeg,				// NewSeg
	EMActNewImgProcess,			// NewImgProcess
	EMActNewMeshProcess,		// NewMeshProcess
	EMActCurrentData,			// CurrentData
	EMActProperty,				// Property
	EMActFocusOn,				// FocusOn
	EMActNewContour,			// 新建轮廓线
	EMActNewSeed,				// 新建种子点
	EMActNewROI,				// 新建ROI
	EMActNewLandmark,			// 新建标定点
	EMActDataType_GrayScale,	// 数据类型，灰度图
	EMActDataType_LabelMap,		// 数据类型，标签图
	EMActColorMap_Gray,			// ColorMap, Default
	EMActColorMap_MultiLabel,	// ColorMap, MapMultiLabel
	EMActColorMap_Mask,			// ColorMap, Mask
	EMActColorMap_Cold,			// ColorMap, Cold
	EMActColorMap_Hot,			// ColorMap, Hot
	EMActColorMap_Cool,			// ColorMap, Cool
	EMActColorMap_Bronson,		// ColorMap, Bronson
	EMActColorMap_SplitBBR,		// ColorMap, Split_BBR
	EMActColorMap_UCLA,			// ColorMap, UCLA
	EMActColorMap_Red,			// ColorMap, Red
	EMActColorMap_Green,		// ColorMap, Green
	EMActColorMap_Blue,			// ColorMap, Blue
	EMActColorMap_HeartVR,		// ColorMap, HeartVR
	EMActColorMap_HeadVR,		// ColorMap, HeadVR
	EMActCntrIS_Mouse,			// 轮廓线交互方式，鼠标
	EMActCntrIS_MouseSerial,	// 轮廓线交互方式，鼠标连续
	EMActCntrIS_Stylus			// 轮廓线交互方式，触控笔
};

class ASDataManagerAction : public QAction
{
	Q_OBJECT

public:
	ASDataManagerAction(QObject * parent, enumActionInContextMenu action);
	ASDataManagerAction(const QString & text, QObject * parent, enumActionInContextMenu action);
	ASDataManagerAction(const QIcon & icon, const QString & text, QObject * parent, enumActionInContextMenu action);
	~ASDataManagerAction();

	enumActionInContextMenu getAction();

private:

	enumActionInContextMenu m_Action;

};
