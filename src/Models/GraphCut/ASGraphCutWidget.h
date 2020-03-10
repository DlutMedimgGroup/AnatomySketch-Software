#pragma once

#include "ASWidgetInSideBarBase.h"
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QListWidget>

class ASGraphCutWidget : public ASWidgetInSideBarBase
{
	Q_OBJECT

public:
	ASGraphCutWidget(QWidget *parent);
	~ASGraphCutWidget();

private:

	// 界面元素
	QComboBox* m_ComboBoxImageFile;
	QListWidget* m_ListWidgetSeeds;
	QLineEdit* m_LineEditOutput;	
	QDoubleSpinBox* m_DSpinBoxLambda;
	QDoubleSpinBox* m_DSpinBoxDelta;
	QComboBox* m_ComboBoxNeighbor;
	QLineEdit* m_LineEditPostProcessing;
	QCheckBox* m_CheckboxMarchingCub;
	QSpinBox* m_SmoothIterations;
	QCheckBox* m_CheckboxQuery;
	QPushButton* m_PushButtonGenerate;
	// 界面初始化
	void UIInit();
	//储存下拉框中index对应的ID,文件操作时order可能会发生变化
	QList<int> m_listIDImageFileIndex;
	QList<int> m_listIDSeedsListIndex;
	int m_ncrntIDOfImageFile = -1;
	//更改条目的槽生效的标志位，当刷新条目内容时，使槽失效
	bool isEnabelforslotComboBoxCurrentIndexChanged = true;
	// 刷新下拉选框
	void UpdateComboBoxImage();
	// 刷新种子点列表
	void UpdateSeedsList();

private slots:

	// 生成结果曲面checkbox
	void slotMarchingCubChecked(int state);
	// 生成按钮按下
	void slotGenerateClicked();
	// 刷新文件下拉选框
	void slotRefreshDataComboBox(int c_AffectType);
	// 下拉选框变化
	void slotComboBoxChanged(int Index);
	// 遮罩模块使能状态切换
	void slotMaskEnabledChanged();
};
