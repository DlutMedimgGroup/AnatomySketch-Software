#pragma once

#include <QWidget>
#include <QList>
#include <QStackedWidget>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QSplitter>
#include <QScrollArea>

class ASSignalLinker;

#define SIDEBAR_MINWIDTH			3         // 侧边栏最小宽度
#define SIDEBAR_MAXWIDTH			500         // 侧边栏最大宽度

#define HSPLITTERHANDLEWIDTH			10			// 分离器宽度，即数据管理器和下方工具的距离

class ASDataManagerWidget;
class ASImageNavigatorWidget;
class ASMaskWidget;
class ASPaintingSeedsWidget;
class ASPaintingContourWidget;
class ASPaintingLandmarkWidget;
class ASPolyDeformationWidget;
class ASGraphCutWidget;
class ASOperationWidget;
class ASROIWidget;
class ASWidgetInSideBarBase;

typedef struct structWidgetInSideBarBelow
{
	int		ID;			
	ASWidgetInSideBarBase*	pWidget;
	QGroupBox* pGroupbox;
	QHBoxLayout* pLayout;
}structWidgetInSideBarBelow;

class ASSideBar : public QWidget
{
	Q_OBJECT

public:
	ASSideBar(QWidget *parent);
	~ASSideBar();

private:

	ASSignalLinker* m_SignalLinker;	//软件中通用信号连接器

	//模块
	ASDataManagerWidget*		m_DataManagerWidget;		// 数据管理列表
	ASImageNavigatorWidget*		m_ImageNavigatorWidget;		// 图像导航，包括图像焦点、窗位窗宽。
	//ASMaskWidget*				m_MaskWidget;				// 遮罩
	ASPaintingSeedsWidget*		m_PaintingSeedsWidgets;		// 种子点
	ASPaintingContourWidget*	m_PaintingContourWidget;	// 轮廓线
	ASPaintingLandmarkWidget*	m_PaintingLandmarkWidget;	// 标定点
	ASGraphCutWidget*			m_GraphCut;					// 图割算法	
	ASPolyDeformationWidget*	m_PolyDeformation;			// 面数据变形
	ASOperationWidget*			m_Operation;				// 插件界面
	ASROIWidget*				m_ROI;						// ROI

	//存储侧边栏结构体的链表
	QList<structWidgetInSideBarBelow*>* m_listWidgetInSideBarBelow;
	//界面元素
	QScrollArea* m_ScrollArea;
	QWidget* m_BaseWidget;
	QGroupBox* m_GroupboxDatamanager;
	QHBoxLayout* m_SidebarLayout;
	QSplitter*  m_Splitter;

	//界面初始化
	void GUIInit();
	//子模块初始化
	void SubModuleInit();

public slots:

	//通过工具栏选择侧边栏下方的Widget
	void slotSetWidgetBelow(int IDOfWidget, bool isShown);
	// 主题改变
	void slotSidebarChange(bool checked);

signals:

	// 设置曲面轮廓是否可以被拾取，配合曲面调整功能
	void signalSetPolyDataContourPickable(bool pickable);
};
