#include "ASMainWidget.h"
#include "ASTitleWidget.h"
#include "ASToolBar.h"
#include "ASSideBar.h"
#include "ASOperationBar.h"
#include "ASStatusBar.h"
#include "ASTransSetting.h"
#include <QIcon>
#include <QSplitter>
#include <QSettings>
#include <QColor>
#include <QVBoxLayout>
#include <QCoreApplication>

QWidget* ASMainWidget::m_FatherWidget = nullptr;

ASMainWidget::ASMainWidget(QWidget *parent)
	: ASDropShadowWidget(parent)
{
	m_FatherWidget = this;

	// 读取窗口位置大小信息
	this->resize(ASTransSetting::GetSetting()->value("mainWindowSize").toSize());
	this->move(ASTransSetting::GetSetting()->value("mainWindowPosition").toPoint());
	
	setMinimumSize(900, 600);
	setWindowIcon(QIcon(":/Icon/AnatomySketch"));
	showMaximized();

	//实例化各模块
	m_TitleWidget = new ASTitleWidget(this);
	m_ToolBar = new ASToolBar(this);
	m_SideBar = new ASSideBar(this);
	m_OperationBar = new ASOperationBar(0);
	m_StatusBar = new ASStatusBar(this);

	this->UIInit();

	//-----------------------信号槽----------------------
	connect(m_TitleWidget, SIGNAL(showMin()), this, SLOT(showMinimized()));
	connect(m_TitleWidget, SIGNAL(showMax()), this, SLOT(showMaximized()));	
	connect(m_TitleWidget, SIGNAL(ShowNormal()), this, SLOT(showNormal()));
	connect(m_TitleWidget, SIGNAL(programexit()), this, SLOT(close()));
	connect(m_TitleWidget, SIGNAL(movewindow(int, int)), this, SLOT(moveWindow(int, int)));
}

ASMainWidget::~ASMainWidget()
{
}
// 得到父窗口的指针，实例弹出窗口时用
QWidget* ASMainWidget::GetMainWidget()
{
	return m_FatherWidget;
}

void ASMainWidget::closeEvent(QCloseEvent * e)
{
	// 写入窗口大小位置信息
	if (!isMaximized())
	{
		ASTransSetting::GetSetting()->setValue("mainWindowPosition", pos());
		ASTransSetting::GetSetting()->setValue("mainWindowSize", size());
		ASTransSetting::GetSetting()->sync();
	}
	QCoreApplication::quit(); // 退出程序
}

void ASMainWidget::UIInit()
{
	//-----------------窗口布局---------------------

	QVBoxLayout* mainLayout = new QVBoxLayout();

	//标题栏
	mainLayout->addWidget(m_TitleWidget);
	//工具栏
	//QHBoxLayout* layout1 = new QHBoxLayout();
	//layout1->addWidget(m_ToolBar);

	//侧边栏&显示交互窗口
	QSplitter*  pSplitter = new QSplitter(Qt::Horizontal, this);	// 设置分离器，分离器可由鼠标拖动调整子控件大小	
	pSplitter->setObjectName("pSplitter");
	pSplitter->setHandleWidth(VSPLITTERHANDLEWIDTH);					// 设置分离器宽度
	pSplitter->setMouseTracking(true);								//在鼠标没有点击是跟踪鼠标移动，用于窗口边界缩放	
	pSplitter->setAutoFillBackground(true);							// 设置分离器透明
	QPalette PaletteOfSplitter = pSplitter->palette();
	PaletteOfSplitter.setColor(pSplitter->backgroundRole(), QColor(0, 0, 0, 0));
	pSplitter->setPalette(PaletteOfSplitter);
	pSplitter->setChildrenCollapsible(false);						//子部件不可折叠

	pSplitter->addWidget(m_SideBar);			// 添加分离器子控件，侧边栏	
	pSplitter->addWidget(m_OperationBar);		// 添加分离器子控件，操作栏
	pSplitter->setStretchFactor(0, 1);
	pSplitter->setStretchFactor(1, 3);
	
	//layout1->addWidget(pSplitter);
	//mainLayout->addLayout(layout1);
	mainLayout->addWidget(pSplitter);
	//状态栏
	mainLayout->addWidget(m_StatusBar);

	mainLayout->setContentsMargins(0, SHADOW_WIDTH, 0, 0);	//设置外边框
	mainLayout->setSpacing(0);	//设置两个控件之间的距离
	setLayout(mainLayout);
}

void ASMainWidget::moveWindow(int ax, int ay)
{
	this->move(ax, ay);
}