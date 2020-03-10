/*
主界面 
*/

#pragma once

//#include "common.h"
#include "ASDropShadowWidget.h"
//#include <QWidget>

class ASTitleWidget;
class ASToolBar;
class ASSideBar;
class ASOperationBar;
class ASStatusBar;
//class ASSignalLinker;

#define VSPLITTERHANDLEWIDTH			3         // 分离器宽度，即侧边栏和操作栏之间空隙的宽度

class ASMainWidget : public ASDropShadowWidget
{
	Q_OBJECT

public:
	ASMainWidget(QWidget *parent = 0);
	~ASMainWidget();

	// 得到父窗口的指针，实例弹出窗口时用
	static QWidget* GetMainWidget();

protected:
	virtual void closeEvent(QCloseEvent * e);

public slots:

	void moveWindow(int ax, int ay);

private slots:

private:

	ASTitleWidget* m_TitleWidget;	//标题栏
	ASToolBar* m_ToolBar;			//工具栏
	ASSideBar* m_SideBar;			//侧边栏
	ASOperationBar* m_OperationBar; //工作区域
	ASStatusBar* m_StatusBar;		//状态栏

	void UIInit();

	// 父窗口指针
	static QWidget* m_FatherWidget;
};
