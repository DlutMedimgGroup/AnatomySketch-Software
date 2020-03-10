#pragma once

#include "ASWidgetInSideBarBase.h"
#include <QPushButton>
#include <QListWidget>
#include <QMenu>
#include <QAction>

class ASLandMarkItemWidget;

class ASPaintingLandmarkWidget : public ASWidgetInSideBarBase
{
	Q_OBJECT

public:
	ASPaintingLandmarkWidget(QWidget *parent);
	~ASPaintingLandmarkWidget();

	virtual void SetEnabled(const bool c_bEnabled);

private:

	// 界面初始化
	void UIInit();

signals:

private slots :

	// 软件当前交互方式改变
	void slotInteractorStyle2DChanged(int interactor);
};
