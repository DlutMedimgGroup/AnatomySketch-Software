#pragma once

#include "ASWidgetInSideBarBase.h"
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QListWidget>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QTimer>

class ASAbstractData;

typedef struct structPolyDeformationInputDataItem
{
	QListWidgetItem* listWedgetItem;
	QCheckBox *checkboxInItem;	//qListWidget中每一行的checkbox
	int ID;						//qListWidget中item对应的ID
}structPolyDeformationInputDataItem;


class ASPolyDeformationWidget : public ASWidgetInSideBarBase
{
	Q_OBJECT

public:
	ASPolyDeformationWidget(QWidget *parent);
	~ASPolyDeformationWidget();

	virtual void SetEnabled(const bool c_bEnabled);

	// 接收快捷键操作信号，用于曲面调整
	//void HotKeyPolyAdjustment(enumHotKeyPolyAdj hotK);

private:

	QList<structPolyDeformationInputDataItem*>* m_listItemOfListWidget;

	//更改条目的槽生效的标志位，当刷新条目内容时，是槽失效
	bool isEnabelforslotComboBoxCurrentIndexChanged = true;
	
	bool m_isZoomSameOperating = false;
	bool m_isZoomValueChangedDisabled = false;
	//QTimer* m_timerForZoom;

	//-------------------- 界面元素 --------------------
	//公共
	QLabel*				m_LabelInput;
	QListWidget*		m_pListWidget;
	QPushButton*		m_PushButtonUndo;

	//界面初始化
	void UIInit();
	//由列表中的QListWidgetItem地址得到InputDataItem地址
	structPolyDeformationInputDataItem* getInputDataItemFromQListWidgetItem(QListWidgetItem* pItem);

protected:
	//void keyPressEvent(QKeyEvent * event);

private slots:

	// 刷新文件下拉选框
	void slotRefreshDataComboBox(int c_AffectType);
	//撤销键按下
	void slotUndoClicked();
	//checkbox状态改变
	void slotFFDCheckBoxChecked(int s);
	// 软件当前交互方式改变
	void slotInteractorStyle2DChanged(int interactor);
	// 数据选择状态改变
	void slotChoosnChanged(int ID);
	// 接收全局键盘信号
	void slotGlobalKeyBoard(QKeyEvent* pKeyEvent);
	// 使用Alt+左键选择polyDeformation作用曲面，接收来自ASDisplay2D的ID
	void slotPolyDeformationChoosePolyData(int nID);
	// 数据是否显示状态改变
	void slotDataShownChanged(ASAbstractData* DataNode);

signals:

	// 选择PolyDeformation作用数据时修改轮廓线宽度刷新轮廓线显示
	// 刷新文件显示，如设置颜色表，是否显示等，均通过此方法刷新
	void signalRefreshDataDisplay(ASAbstractData* datanode);
};
