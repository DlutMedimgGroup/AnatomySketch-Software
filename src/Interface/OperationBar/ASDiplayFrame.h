#pragma once

//#include "common.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QKeyEvent>

class ASQVTKWidget;
class ASHoverWidgetInQVTKWidget;

class ASDiplayFrame : public QFrame
{
	Q_OBJECT

public:
	ASDiplayFrame(QWidget *parent);
	~ASDiplayFrame();

	void setViewSelection(int viewselection);
	ASQVTKWidget* getASQVTKWidget();
	void setExpanded(const bool c_expanded);

private:

	ASQVTKWidget*	m_QVTKWidget;
	QHBoxLayout*	m_HBoxLayout;

	int m_ViewSelection = 0;
	bool m_Expanded = false;

	ASHoverWidgetInQVTKWidget* m_HoverWidget;

protected:

	void resizeEvent(QResizeEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

signals:

	//向需要VTKWidget指针的类发送地址，在软件初始化阶段
	void signalSetQVTKWidgerAddress(void* pAddr, int ViewLabel);
	//更改窗口布局
	void signalButtonLayoutClicked(int viewlabel);
	// 十字线是否显示
	void signalButtonCrossShairClicked(int viewlabel);

private slots:

	void slotButtonLayoutClicked();
	void slotButtonCrossShairClicked();
};
