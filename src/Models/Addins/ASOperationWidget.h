#pragma once

#include "ASWidgetInSideBarBase.h"
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class ASAddinsBase;

class ASOperationWidget : public ASWidgetInSideBarBase
{
	Q_OBJECT

public:
	ASOperationWidget(QWidget *parent);
	~ASOperationWidget();

	virtual void SetEnabled(const bool c_bEnabled);

private:

	// 界面元素
	QComboBox* m_Combox;
	QPushButton* m_GeneButn;
	QLabel* m_Description;
	QPushButton* m_BtnFinish;
	QVBoxLayout* m_mainlayout;
	void ComboInit(const int c_AddinType);
	void RemoveLayout(QLayout* layout);

	ASAddinsBase* m_crntOperation= nullptr;
	bool m_FlagFromAddin = false;

	// 根据当前操作刷新界面
	void RefreshCrntOperationUI();

protected:

	bool m_GeneratingCombo = false; // 正在重心生成方法下拉菜单的标志位
	

private slots :

	// combobox 
	void slotComboxChanged();
	// PushButton
	void slotGenerate();
	// 刷新，如删除数据后
	void slotRefreshWidget();
	// 更换当前操作
	void slotCrntOperationChanged();
	// 数据列表变化，刷新combobox
	void slotUpdateDataNodeLists(int Type);
	// 设置当前方法
	void slotSetCurrentMethod(QString s);
	// Finish
	void slotFinish();

signals:

};
