#pragma once

#include <QWidget>

class ASWidgetInSideBarBase : public QWidget
{
	Q_OBJECT

public:
	ASWidgetInSideBarBase(QWidget *parent);
	~ASWidgetInSideBarBase();

	virtual void SetEnabled(const bool c_bEnabled);
};
