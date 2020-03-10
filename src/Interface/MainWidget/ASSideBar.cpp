#include "ASSideBar.h"
#include "ASDataManagerWidget.h"
#include "ASTransSidebarGeneral.h"
#include "ASImageNavigatorWidget.h"
#include "ASMaskWidget.h"
#include "ASPaintingSeedsWidget.h"
#include "ASPaintingContourWidget.h"
#include "ASPaintingLandmarkWidget.h"
#include "ASGraphCutWidget.h"
#include "ASPolyDeformationWidget.h"
#include "ASOperationWidget.h"
#include "ASROIWidget.h"
#include "ASTransTheme.h"
#include "ASTransAddins.h"
#include "ASOperation.h"
#include "ASAddinsBase.h"
#include <QVBoxLayout>

ASSideBar::ASSideBar(QWidget *parent)
	: QWidget(parent)
{	
	connect(ASTransTheme::GetSelfPointer(), SIGNAL(signalSidebarChange(bool)), this, SLOT(slotSidebarChange(bool)));

	GUIInit();			//界面初始化
	SubModuleInit();	//子模块初始化	

	//在鼠标没有点击是跟踪鼠标移动，用于窗口边界缩放
	setMouseTracking(true);

	// 接收Transmission信号
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalSetSideBarBelowWidget(int, bool)), 
		this, SLOT(slotSetWidgetBelow(int, bool)));
}

ASSideBar::~ASSideBar()
{

}

//界面初始化
void ASSideBar::GUIInit()
{
	bool default_theme = ASTransTheme::GetTheme();

	m_ScrollArea = new QScrollArea(this);
	m_ScrollArea->setWidgetResizable(true);
	m_BaseWidget = new QWidget(m_ScrollArea);
	//设置控件背景颜色
	m_BaseWidget->setAutoFillBackground(true);
	slotSidebarChange(default_theme);
	m_ScrollArea->setWidget(m_BaseWidget);
	m_ScrollArea->setObjectName("m_ScrollArea");
	QHBoxLayout* MainLayout = new QHBoxLayout();
	MainLayout->setMargin(0);
	MainLayout->addWidget(m_ScrollArea);
	this->setLayout(MainLayout);
	
	m_GroupboxDatamanager = new	QGroupBox(m_BaseWidget);
	m_SidebarLayout = new QHBoxLayout(m_BaseWidget);
	m_DataManagerWidget = new ASDataManagerWidget(m_BaseWidget);

	// 设置分离器，分离器可由鼠标拖动调整子控件大小	
	m_Splitter = new QSplitter(Qt::Vertical, this);
	m_Splitter->setObjectName("Splitter");
	m_Splitter->setHandleWidth(HSPLITTERHANDLEWIDTH);					// 设置分离器宽度
	m_Splitter->setMouseTracking(true);								//在鼠标没有点击时跟踪鼠标移动，用于窗口边界缩放	
	m_Splitter->setAutoFillBackground(true);							// 设置分离器透明
	QPalette PaletteOfSplitter = m_Splitter->palette();
	PaletteOfSplitter.setColor(m_Splitter->backgroundRole(), QColor(0, 0, 0, 0));
	//PaletteOfSplitter.setColor(m_Splitter->backgroundRole(), QColor(33, 37, 43));
	m_Splitter->setPalette(PaletteOfSplitter);
	m_Splitter->setChildrenCollapsible(false);						//子部件不可折叠

	//设置上方DataManager的圆角边框
	m_Splitter->addWidget(m_GroupboxDatamanager);
	m_GroupboxDatamanager->setTitle(tr("Data Manager"));
	m_GroupboxDatamanager->setObjectName("DataManager");
	/*m_GroupboxDatamanager->setStyleSheet(
		"QGroupBox"
		"{"
		"border: 0px solid;"
		"border-color:rgb(63, 63, 63);"
		"border-radius:8px;"
		"margin-top:10px;"
		"font-size:14px;"
		"}"
		"QGroupBox::title"
		"{"
		"color: rgb(63, 63, 63);"
		"subcontrol-origin: margin;"
		"left: 10px;"
		"}"
	);*/
	QVBoxLayout *vboxGroupboxDatamanager = new QVBoxLayout;
	vboxGroupboxDatamanager->addWidget(m_DataManagerWidget);
	m_GroupboxDatamanager->setLayout(vboxGroupboxDatamanager);	
	
	m_SidebarLayout->addWidget(m_Splitter);
	m_BaseWidget->setLayout(m_SidebarLayout);

	setMaximumWidth(SIDEBAR_MAXWIDTH);
	setMinimumWidth(SIDEBAR_MINWIDTH);
}
//子模块初始化
void ASSideBar::SubModuleInit()
{
	m_listWidgetInSideBarBelow = new QList<structWidgetInSideBarBelow*>;
	/*QString GroupBoxStyleSheet(
		"QGroupBox"
		"{"
		"border: 1px solid;"
		"border-color:rgb(63, 63, 63);"
		"border-radius:8px;"
		"margin-top:14px;"
		"font-size:14px;"
		"}"
		"QGroupBox::title"
		"{"
		"color:rgb(63, 63, 63);"
		"subcontrol-origin: margin;"
		"left: 10px;"
		"}"
	);*/
	// 0 图像导航，包括图像焦点、窗位窗宽。
	m_ImageNavigatorWidget = new ASImageNavigatorWidget(m_BaseWidget);
	structWidgetInSideBarBelow* ImageNavigator = new structWidgetInSideBarBelow;
	ImageNavigator->ID = SIDEBAR_IMAGENAVIGATOR;
	ImageNavigator->pWidget = m_ImageNavigatorWidget;
	ImageNavigator->pGroupbox = new QGroupBox(m_BaseWidget);
	m_Splitter->addWidget(ImageNavigator->pGroupbox);
	ImageNavigator->pGroupbox->setTitle(tr("Image Navigator"));
	ImageNavigator->pGroupbox->setObjectName("mImageNavigator");

	ImageNavigator->pLayout = new QHBoxLayout();
	ImageNavigator->pLayout->addWidget(m_ImageNavigatorWidget);
	ImageNavigator->pGroupbox->setLayout(ImageNavigator->pLayout);
	ImageNavigator->pGroupbox->hide();
	m_listWidgetInSideBarBelow->append(ImageNavigator);
	// 1 遮罩
	//m_MaskWidget = new ASMaskWidget(m_BaseWidget);
	//structWidgetInSideBarBelow* Mask = new structWidgetInSideBarBelow;
	//Mask->ID = SIDEBAR_Mask;
	//Mask->pWidget = m_MaskWidget;
	//Mask->pGroupbox = new QGroupBox(m_BaseWidget);
	//m_Splitter->addWidget(Mask->pGroupbox);
	//Mask->pGroupbox->setTitle(tr("Mask"));
	//Mask->pGroupbox->setStyleSheet(GroupBoxStyleSheet);
	//Mask->pLayout = new QHBoxLayout();
	//Mask->pLayout->addWidget(m_MaskWidget);
	//Mask->pGroupbox->setLayout(Mask->pLayout);
	//Mask->pGroupbox->hide();
	//m_listWidgetInSideBarBelow->append(Mask);
	//2 种子点
	m_PaintingSeedsWidgets = new ASPaintingSeedsWidget(m_BaseWidget);
	structWidgetInSideBarBelow* PaintingSeeds = new structWidgetInSideBarBelow;
	PaintingSeeds->ID = SIDEBAR_PaintingSeeds;
	PaintingSeeds->pWidget = m_PaintingSeedsWidgets;
	PaintingSeeds->pGroupbox = new QGroupBox(m_BaseWidget);
	m_Splitter->addWidget(PaintingSeeds->pGroupbox);
	PaintingSeeds->pGroupbox->setTitle(tr("Seeds"));
	PaintingSeeds->pGroupbox->setObjectName("mPaintingSeeds");

	PaintingSeeds->pLayout = new QHBoxLayout();
	PaintingSeeds->pLayout->addWidget(m_PaintingSeedsWidgets);
	PaintingSeeds->pGroupbox->setLayout(PaintingSeeds->pLayout);
	PaintingSeeds->pGroupbox->hide();
	m_listWidgetInSideBarBelow->append(PaintingSeeds);
	// 轮廓线
	m_PaintingContourWidget = new ASPaintingContourWidget(m_BaseWidget);
	structWidgetInSideBarBelow* PaintingContour = new structWidgetInSideBarBelow;
	PaintingContour->ID = SIDEBAR_PaintingContour;
	PaintingContour->pWidget = m_PaintingContourWidget;
	PaintingContour->pGroupbox = new QGroupBox(m_BaseWidget);
	m_Splitter->addWidget(PaintingContour->pGroupbox);
	PaintingContour->pGroupbox->setTitle(tr("Contour"));
	PaintingContour->pGroupbox->setObjectName("mPaintingContour");

	PaintingContour->pLayout = new QHBoxLayout();
	PaintingContour->pLayout->addWidget(m_PaintingContourWidget);
	PaintingContour->pGroupbox->setLayout(PaintingContour->pLayout);
	PaintingContour->pGroupbox->hide();
	m_listWidgetInSideBarBelow->append(PaintingContour);
	// 标定点
	m_PaintingLandmarkWidget = new ASPaintingLandmarkWidget(m_BaseWidget);
	structWidgetInSideBarBelow* PaintingLandmark = new structWidgetInSideBarBelow;
	PaintingLandmark->ID = SIDEBAR_PaintingLandmark;
	PaintingLandmark->pWidget = m_PaintingLandmarkWidget;
	PaintingLandmark->pGroupbox = new QGroupBox(m_BaseWidget);
	m_Splitter->addWidget(PaintingLandmark->pGroupbox);
	PaintingLandmark->pGroupbox->setTitle(tr("Lankmark"));
	PaintingLandmark->pGroupbox->setObjectName("mPaintingLandmark");

	PaintingLandmark->pLayout = new QHBoxLayout();
	PaintingLandmark->pLayout->addWidget(m_PaintingLandmarkWidget);
	PaintingLandmark->pGroupbox->setLayout(PaintingLandmark->pLayout);
	PaintingLandmark->pGroupbox->hide();
	m_listWidgetInSideBarBelow->append(PaintingLandmark);

	// 图割算法，Graphcuts
	m_GraphCut = new ASGraphCutWidget(m_BaseWidget);
	structWidgetInSideBarBelow* GraphCuts = new structWidgetInSideBarBelow;
	GraphCuts->ID = SIDEBAR_GraphCuts;
	GraphCuts->pWidget = m_GraphCut;
	GraphCuts->pGroupbox = new QGroupBox(m_BaseWidget);
	m_Splitter->addWidget(GraphCuts->pGroupbox);
	GraphCuts->pGroupbox->setTitle(tr("Graph Cut"));
	GraphCuts->pGroupbox->setObjectName("mGraphCuts");
	GraphCuts->pLayout = new QHBoxLayout();
	GraphCuts->pLayout->addWidget(m_GraphCut);
	GraphCuts->pGroupbox->setLayout(GraphCuts->pLayout);
	GraphCuts->pGroupbox->hide();
	m_listWidgetInSideBarBelow->append(GraphCuts);
	// 面数据变形
	m_PolyDeformation = new ASPolyDeformationWidget(m_BaseWidget);
	structWidgetInSideBarBelow* PolyDeformation = new structWidgetInSideBarBelow;
	PolyDeformation->ID = SIDEBAR_PolyDeformation;
	PolyDeformation->pWidget = m_PolyDeformation;
	PolyDeformation->pGroupbox = new QGroupBox(m_BaseWidget);
	m_Splitter->addWidget(PolyDeformation->pGroupbox);
	PolyDeformation->pGroupbox->setTitle(tr("Poly Deformation"));
	PolyDeformation->pGroupbox->setObjectName("mPolyDeformation");

	PolyDeformation->pLayout = new QHBoxLayout();
	PolyDeformation->pLayout->addWidget(m_PolyDeformation);
	PolyDeformation->pGroupbox->setLayout(PolyDeformation->pLayout);
	PolyDeformation->pGroupbox->hide();
	m_listWidgetInSideBarBelow->append(PolyDeformation);
	// 分割
	m_Operation = new ASOperationWidget(m_BaseWidget);
	structWidgetInSideBarBelow* Operation = new structWidgetInSideBarBelow;
	Operation->ID = SIDEBAR_Operation;
	Operation->pWidget = m_Operation;
	Operation->pGroupbox = new QGroupBox(m_BaseWidget);
	m_Splitter->addWidget(Operation->pGroupbox);
	Operation->pGroupbox->setTitle(tr("Segmentation"));
	Operation->pGroupbox->setObjectName("mSegmentation");

	Operation->pLayout = new QHBoxLayout();
	Operation->pLayout->addWidget(m_Operation);
	Operation->pGroupbox->setLayout(Operation->pLayout);
	Operation->pGroupbox->hide();
	m_listWidgetInSideBarBelow->append(Operation);
	// ROI
	m_ROI = new ASROIWidget(m_BaseWidget);
	structWidgetInSideBarBelow* ROI = new structWidgetInSideBarBelow;
	ROI->ID = SIDEBAR_ROI;
	ROI->pWidget = m_ROI;
	ROI->pGroupbox = new QGroupBox(m_BaseWidget);
	m_Splitter->addWidget(ROI->pGroupbox);
	ROI->pGroupbox->setTitle(tr("ROI"));
	ROI->pGroupbox->setObjectName("mROI");
	ROI->pLayout = new QHBoxLayout();
	ROI->pLayout->addWidget(m_ROI);
	ROI->pGroupbox->setLayout(ROI->pLayout);
	ROI->pGroupbox->hide();
	m_listWidgetInSideBarBelow->append(ROI);
}

//通过工具栏选择侧边栏下方的Widget
void ASSideBar::slotSetWidgetBelow(int IDOfWidget, bool isShown)
{
	int numOfWidget = m_listWidgetInSideBarBelow->size();
	for (int nCrntWidget = 0; nCrntWidget < numOfWidget; nCrntWidget++)
	{
		if (m_listWidgetInSideBarBelow->at(nCrntWidget)->ID == IDOfWidget)
		{
			if (m_listWidgetInSideBarBelow->at(nCrntWidget)->ID == SIDEBAR_Operation)
			{
				int numOfWidget = m_listWidgetInSideBarBelow->size();
				structWidgetInSideBarBelow* Operation;
				for (int i = 0; i < numOfWidget; i++)
				{
					if (m_listWidgetInSideBarBelow->at(i)->ID == SIDEBAR_Operation)
					{
						Operation = m_listWidgetInSideBarBelow->at(i);
					}
				}
				ASOperation* cnrtOperation = ASTransAddins::GetCrntOperation();
				if (cnrtOperation)
				{
					switch (cnrtOperation->GetAddinType())
					{
					case AddinType_ImgProcess:
						Operation->pGroupbox->setTitle(tr("Image Processing"));
						break;
					case AddinType_MeshProcess:
						Operation->pGroupbox->setTitle(tr("Mesh Processing"));
						break;
					case AddinType_Segmentation:
						Operation->pGroupbox->setTitle(tr("Segmentation"));
						break;
					default:
						break;
					}
				}				
				m_listWidgetInSideBarBelow->at(nCrntWidget)->pGroupbox->setVisible(isShown);
			}
			else if (m_listWidgetInSideBarBelow->at(nCrntWidget)->ID == SIDEBAR_PolyDeformation)
			{
				m_listWidgetInSideBarBelow->at(nCrntWidget)->pGroupbox->setVisible(isShown);
			}
			m_listWidgetInSideBarBelow->at(nCrntWidget)->pWidget->SetEnabled(isShown);
		}
		else if (isShown)
		{
			if (IDOfWidget == SIDEBAR_Operation ||
				IDOfWidget == SIDEBAR_PolyDeformation)
			{
				m_listWidgetInSideBarBelow->at(nCrntWidget)->pGroupbox->setVisible(false);
				m_listWidgetInSideBarBelow->at(nCrntWidget)->pWidget->SetEnabled(false);
			}
		}
	}
	if (IDOfWidget == SIDEBAR_Operation)
	{
		ASTransSidebarGeneral::BtnRst();
	}
}

void ASSideBar::slotSidebarChange(bool checked)
{
	QPalette Palette = this->palette();
	if (checked)
		Palette.setColor(m_BaseWidget->backgroundRole(), QColor(40, 44, 52));
	else
		Palette.setColor(m_BaseWidget->backgroundRole(), QColor(238, 238, 238));
	m_BaseWidget->setPalette(Palette);
}
