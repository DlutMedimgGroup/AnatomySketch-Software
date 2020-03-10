#pragma once

#include "ASWidgetInSideBarBase.h"
#include <QLabel>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QTabWidget>

class ASSignalLinker;
class ASDataManager;

class ASMaskWidget : public ASWidgetInSideBarBase
{
	Q_OBJECT

public:
	ASMaskWidget(QWidget *parent);
	~ASMaskWidget();

	virtual void SetEnabled(const bool c_bEnabled);

private:

	ASSignalLinker* m_SignalLinker;	//软件中通用信号连接器
	ASDataManager* m_pDataManager;	//数据管理器的地址

	//储存下拉框中index对应的ID,文件操作时order可能会发生变化
	QList<int> m_listIDForIndex;
	QList<int> m_listIDForIndexMData;

	int m_crntIDOfComboBox = -1;
	int m_crntIDOfMdata = -1;
	//更改条目的槽生效的标志位，当刷新条目内容时，使槽失效
	bool isEnabelforslotComboBoxCurrentIndexChanged = true;
	//更改输入参数时，使刷新显示的信号失效
	bool isEnabelforslotSettingValueChanged = true;


	//界面元素
	QDoubleSpinBox*		m_DSBoxCoRange[6]; // 0: XMin 1: XMax 2: YMin 3: YMax 4: ZMin 5: ZMax
	QPushButton*		m_PushButtonCoRange[6]; // 0: XMin 1: XMax 2: YMin 3: YMax 4: ZMin 5: ZMax
	QComboBox*			m_ComboBoxInput;
	QTabWidget*			m_TabWidget;
	// 直接灰度值范围
	QDoubleSpinBox*		m_DSBoxGrayRange[2]; //0: min 1: max
	// 选择其他图像计算灰度值范围
	QComboBox*			m_ComboBoxMDataInput;
	QDoubleSpinBox*		m_DSBoxMDataGrayRange[2]; //0: min 1: max
	QLabel*				m_LabelMDataStatus;

	//界面初始化
	void UIInit();
	// 更新控件，范围、当前值
	void UpdataSpinBox();
	// 判断遮罩图像是否符合要求
	void MDataCheck();

private slots:

	//文件下拉选框选项改变
	void slotCrntIndexChanged(int index);
	void slotCrntIndexChangedMData(int index);
	//界面设置参数改变，更新遮罩数组
	void slotSettingValueChanged();
	//刷新所有文件下拉选框
	void slotRefreshDataComboBox(int c_AffectType);
	// 在视图中选择遮罩
	void slotSelectFromView(bool b);
	// 软件当前交互方式改变
	void slotInteractorStyle2DChanged(int interactor);
	// 交互设置遮罩
	void slotSetMaskInteractor(double x, double y, double z);
	// 子页切换
	void slotTabCurrentChanged(int index);

signals:

	//遮罩更新后刷新显示
	void signalRefreshMaskDisplay();
	//遮罩使能状态改变后更新显示
	void signalMaskStateChanged();

};
