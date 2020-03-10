#pragma once

#include "ASAbstructItemWidget.h"
#include <QLabel>

class ASMarkEleItemWidget : public ASAbstructItemWidget
{
	Q_OBJECT

public:
	ASMarkEleItemWidget(QWidget *parent);
	~ASMarkEleItemWidget();	

	// ItemType
	virtual bool IsMarkEle();
	// No.
	void SetNo(const int c_No);
	int GetNo();
	// Focus
	virtual void SetFocus(const bool c_f);
	bool GetFocus();

protected:

	int m_No;

	//QLabel* m_LabelNo;

signals:

private slots:

};
