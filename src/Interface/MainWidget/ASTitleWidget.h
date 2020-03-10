/*
标题  包括：最小化、关闭等
*/
#pragma once
#ifdef WIN32
#pragma  execution_character_set("utf-8")
#endif

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>

class ASPushButtonInTitleWidget;
class AboutWidget;
class ASHelpWidget;

#define BUTTON_WIDTH 32         // 按钮宽度;
#define BUTTON_HEIGHT 26        // 按钮高度;
#define TITLE_HEIGHT 30         // 标题栏高度;

class ASTitleWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ASTitleWidget(QWidget *parent = 0);
	~ASTitleWidget();
	
protected:

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

private:

	QPoint m_press_point;//鼠标按下去的点

	bool is_move;
	bool is_Maximized = true;

	QPoint move_point; //移动的距离
	bool mouse_press; //按下鼠标左键

	ASPushButtonInTitleWidget* m_min_button; //最小化
	ASPushButtonInTitleWidget* m_max_button; //最大化
	ASPushButtonInTitleWidget* m_close_button; //关闭

	QPushButton* m_BtnOpen;
	QPushButton* m_BtnDICOM;
	QPushButton* m_BtnHelp;
	QPushButton* m_BtnAbout; 
	QPushButton* m_BtnTheme;
	QPushButton* m_BtnLang;
	
	void UIInit();

	// 子界面
	AboutWidget* m_AboutWidget;
	ASHelpWidget* m_HelpWidget;

private slots:

	void maxClicked();//最大化按钮按下，需要判断最大化还是取消最大化

	// 打开
	void slotOpen();
	// DICOM
	void slotDICOM();
	// Help
	void slotHelp();
	// About
	void slotAbout();
	// Theme
	void slotQSSChange(bool checked);
	void slotOperationbarThemeChange(bool checked);
	void slotBackgroundChange(bool checked);
	void slotSidebarChange(bool checked);

	// Lang
	void slotLanguageChange(bool checked);

signals:

	void showMin();
	void showMax();
	void ShowNormal();
	void programexit();
	void movewindow(int ax, int ay);

};
