#pragma once

#include "ASAbstructItemWidget.h"
#include <QCheckBox>
#include <QLabel>
#include <QString>

class ASDataNodeItemWidget : public ASAbstructItemWidget
{
	Q_OBJECT

public:
	ASDataNodeItemWidget(QWidget *parent);
	~ASDataNodeItemWidget();
	// ItemType
	virtual bool IsMarkEle();
	virtual void SetChecked(bool checked);

private:

	void UpdateIcon();

protected slots:

	//virtual void slotCheckboxStateChanged(int state);
	virtual void slotIconClicked();
};
