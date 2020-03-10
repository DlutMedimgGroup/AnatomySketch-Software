#pragma once

#include <QToolBar>
#include <QToolButton>
#include <QList>
#include <QString>

class AboutWidget;
class ASHelpWidget;

#define TOOLBAR_SIZE 60   // 工具栏高度;

//工具栏ID，修改ID可以调整按钮的显示顺序，ID必须连续且不重复
//#define TOOLBAR_OPEN				0		// 打开
//#define TOOLBAR_DICOM				1		// 打开DICOM
//#define TOOLBAR_CLOSEALL			2		// 关闭所有数据
//#define TOOLBAR_EXPORT				3		// 导出
#define ToolBar_ImageNavigator		0		// 调整窗位窗宽和焦点的工具
#define TOOLBAR_ROI					1		// 遮罩
#define TOOLBAR_PaintSeeds			2		// 绘制种子点
#define TOOLBAR_PaintContours		3		// 绘制轮廓线
#define TOOLBAR_PaintLandmarks		4		// 绘制标定点
#define TOOLBAR_PolyDeformation		5		// PolyDeformation
//#define TOOLBAR_GraphCuts			10		// GraphCuts
//#define TOOLBAR_Help				11		// Help
//#define TOOLBAR_About				12		// About

typedef struct structTools {
	int ID;
	bool isBelowWidget = false;
	QToolButton *toolButton;
}structTools;

class ASToolBar : public QToolBar
{
	Q_OBJECT

public:
	ASToolBar(QWidget *parent);
	~ASToolBar();

protected:
	// 绘制圆角
	void paintEvent(QPaintEvent* e);

private:

	//储存按钮和对应ID
	QList<structTools*>* m_Tools;

	//工具按钮初始化
	void ToolsInit();
	//工具按钮刷新显示
	void ToolsDisplayRefresh();
	//由ID得到工具
	structTools* gettool(int ID);
	// 当前按下的按钮
	void SetCrntTool(int ID, bool checked);

public slots:

	//在当前交互方式通过选中文件变化而需要改变时，发送信号让工具栏改变按钮状态，从datamanager发送
	//void slotRefreshSeedsCuntourPushButtonState();

private slots:

	// 关闭全部
	//void slotCloseAll();
	//// 导出
	//void slotExport();
	// 绘制种子点工具
	void slotPaintSeeds(bool checked);
	// 绘制轮廓线工具
	void slotPaintContours(bool checked);
	// 绘制标定点工具
	void slotPaintLandmarks(bool checked);
	// 调整窗位窗宽和焦点的工具
	void slotImageNavigator(bool checked);
	// ROI
	void slotROI(bool checked);
	// PolyDeformation
	void slotPolyDeformation(bool checked);
	// GraphCuts
	void slotGraphCuts(bool checked);
	// 切换当前工具按钮状态
	void slotSetSideBarBelowWidget(int c_WidgetType, bool c_isShown);

	void slotBtnRst();
};
