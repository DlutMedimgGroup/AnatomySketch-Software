#pragma once

#include "ASMarkEleItemWidget.h"
#include <QString>
#include <QColor>
#include <QCheckBox>
#include <QLabel>
#include <QListWidgetItem>

class ASROIItemWidget : public ASMarkEleItemWidget
{
	Q_OBJECT

public:
	ASROIItemWidget(QWidget *parent);
	~ASROIItemWidget();	

	// ID
	virtual int getMarkEleID();
	// Focus
	virtual void SetFocus(const bool c_f);
	// 参数
	void SetROIID(const int c_ID);
	int GetROIID();
	void SetColor(const QColor c_Color);

private:

	int m_ROIID;
	// 界面元素
	//QLabel* m_LabelLabel;
	//QLabel* m_LabelColor;

	void UIInit();

protected slots:

	virtual void slotCheckboxStateChanged(int state);
};
