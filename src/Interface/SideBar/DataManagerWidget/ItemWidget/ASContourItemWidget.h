#pragma once

#include "ASMarkEleItemWidget.h"
#include <QString>
#include <QColor>
#include <QLabel>

class ASContourItemWidget : public ASMarkEleItemWidget
{
	Q_OBJECT

public:
	ASContourItemWidget(QWidget *parent);
	~ASContourItemWidget();

	// ID
	virtual int getMarkEleID();
	// Focus
	virtual void SetFocus(const bool c_f);
	// 参数
	void SetContourID(const int c_ID);
	int GetContourID();
	void SetLabel(const int c_Label);
	void SetColor(const QColor c_Color);

private:
	
	int m_ContourID;	
	// 界面元素	
	//QLabel* m_LabelLabel;
	//QLabel* m_LabelColor;

	void UIInit();

protected slots:

	virtual void slotCheckboxStateChanged(int state);
};
