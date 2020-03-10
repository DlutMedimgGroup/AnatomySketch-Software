#pragma once

#include "ASMarkEleItemWidget.h"
#include <QString>
#include <QColor>
#include <QCheckBox>
#include <QLabel>
#include <QListWidgetItem>

class ASLandMarkItemWidget : public ASMarkEleItemWidget
{
	Q_OBJECT

public:
	ASLandMarkItemWidget(QWidget *parent);
	~ASLandMarkItemWidget();

	// ID
	virtual int getMarkEleID();
	// Focus
	virtual void SetFocus(const bool c_f);
	// 参数
	void SetLandmarkID(const int c_ID);
	int GetLandmarkID();
	void SetColor(const QColor c_Color);

private:

	int m_LandmarkID;
	// 界面元素
	//QLabel* m_LabelLabel; // 占位
	//QLabel* m_LabelColor;

	void UIInit();

protected slots:

	virtual void slotCheckboxStateChanged(int state);
};
