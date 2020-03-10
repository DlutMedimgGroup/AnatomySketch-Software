#pragma once

#include "ASWidgetInSideBarBase.h"
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSlider>
#include <QPushButton>

class ASSignalLinker;
class ASDataManager;

class ASImageNavigatorWidget : public ASWidgetInSideBarBase
{
	Q_OBJECT

public:
	ASImageNavigatorWidget(QWidget *parent);
	~ASImageNavigatorWidget();

private:
	ASSignalLinker* m_SignalLinker;	//软件中通用信号连接器
	ASDataManager* m_pDataManager;	//数据管理器的地址

	//界面元素
	QDoubleSpinBox*	m_LocationDoubleSpinBox[3];		// 焦点，世界坐标系
	QSpinBox*		m_FocusSpinBox[3];				// 焦点，像素坐标系
	QSlider*		m_FocusSlider[3];				// 焦点，滑动条
	QSpinBox*		m_TimeSpinBox;					// 时间
	QSlider*		m_TimeSlider;					// 时间，滑动条
	QDoubleSpinBox* m_LevelWindowDoubleSpinBox[2];	// 窗位窗宽
	QLabel*			m_Label[7];						// Label. 0:Location; 1:Axial; 2:Sagittal; 3:Coronal; 5:Level 6:Window

	//显示范围
	double m_ImageExtent[6];
	double m_ImageLocation[3];		//图像焦点，世界坐标系
	double m_ImageSpace[3];
	int m_NumOfFrames = 1;
	int m_crntFrame;

	bool isValueChanging = false;

	//界面初始化
	void GUIInit();
	//将新的焦点更新到控件显示
	void GeneratePosition();

private slots:

	//控件值改变
	// 焦点
	void slotLocationXValueChanged(double d);
	void slotLocationYValueChanged(double d);
	void slotLocationZValueChanged(double d);
	void slotFocusXValueChanged(int i);
	void slotFocusYValueChanged(int i);
	void slotFocusZValueChanged(int i);
	void slotSliderXValueChanged(int i);
	void slotSliderYValueChanged(int i);
	void slotSliderZValueChanged(int i);
	// 帧
	void slotTimeValueChanged(int i);
	// 窗位窗宽
	void slotLevelValueChanged(double d);
	void slotWindowValueChanged(double d);

public slots:

	// 被动更新焦点
	void slotSetDisplayPosition(double PosX, double PosY, double PosZ);
	// 被动更新帧
	void slotSetDisplayFrame(const int c_Frame);
	// 更新像素尺寸
	void slotSetImageSpacing(double* Spacing);
	// 更新图像范围和帧数
	void slotSetDisplayExtent(double* Extent, const int c_NumOfFrames);
	// 被动更新窗位窗宽
	void slotSetWindowLevel(double window, double level);
	void slotUpdateWindow();
	void slotUpdateLevel();

signals:
};
