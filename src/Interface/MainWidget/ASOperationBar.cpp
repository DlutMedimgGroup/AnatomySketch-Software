#include "ASOperationBar.h"
#include "ASDiplayFrame.h"
#include "ASQVTKWidget.h"
#include "GlobelDeclaration.h"
#include "ASTransGeneralInteractor.h"
#include "ASTransTheme.h"
#include <QSettings>
#include <QPixmapCache>
#include <QPushButton>
#include <QPainter>

ASOperationBar* ASOperationBar::ms_pOperationBar = nullptr;

ASOperationBar::ASOperationBar(QWidget* parent)
	: QWidget(parent)
{
	bool default_theme = ASTransTheme::GetTheme();
	ms_pOperationBar = this;

	// 设置控件背景颜色
	slotBackgroundColorChange(default_theme);
	connect(ASTransTheme::GetSelfPointer(), SIGNAL(signalOperationbarThemeChange(bool)), this,
	        SLOT(slotBackgroundColorChange(bool)));

	//在鼠标没有点击是跟踪鼠标移动，用于窗口边界缩放
	setMouseTracking(true);

	for (int numOfView = 0; numOfView < 4; numOfView++)
	{
		m_apDisplayFrame[numOfView] = new ASDiplayFrame(this);
		m_apDisplayFrame[numOfView]->setViewSelection(numOfView);
		connect(m_apDisplayFrame[numOfView], SIGNAL(signalButtonLayoutClicked(int)), this,
		        SLOT(slotButtonLayoutClicked(int)));
		connect(m_apDisplayFrame[numOfView], SIGNAL(signalButtonCrossShairClicked(int)), this,
		        SLOT(slotCrossShairClicked(int)));
	}

	GUIInit();
}

ASOperationBar::~ASOperationBar()
{
}

void ASOperationBar::GetQVTKWidget(ASQVTKWidget** pQVTKWidget)
{
	ms_pOperationBar->fGetQVTKWidget(pQVTKWidget);
}

//界面初始化，包括五种窗口布局
void ASOperationBar::GUIInit()
{
	m_OperationbarLayout = new QGridLayout();
	setLayoutMode(OpeBarLayoutLabel_Default);
}

// 静态函数对应到唯一对象中的函数
void ASOperationBar::fGetQVTKWidget(ASQVTKWidget** pQVTKWidget)
{
	pQVTKWidget[0] = m_apDisplayFrame[0]->getASQVTKWidget();
	pQVTKWidget[1] = m_apDisplayFrame[1]->getASQVTKWidget();
	pQVTKWidget[2] = m_apDisplayFrame[2]->getASQVTKWidget();
	pQVTKWidget[3] = m_apDisplayFrame[3]->getASQVTKWidget();
}

void ASOperationBar::setLayoutMode(int mode)
{
	m_LayoutMode = mode;
	switch (mode)
	{
	case OpeBarLayoutLabel_Default:
		while ((m_OperationbarLayout->takeAt(0)) != nullptr);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Axial], 0, 0);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Sagittal], 0, 1);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Coronal], 1, 0);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_3D], 1, 1);
		m_OperationbarLayout->setRowStretch(0, 1);
		m_OperationbarLayout->setRowStretch(1, 1);
		m_OperationbarLayout->setRowStretch(2, 0);
		m_OperationbarLayout->setColumnStretch(0, 1);
		m_OperationbarLayout->setColumnStretch(1, 1);
		this->setLayout(m_OperationbarLayout);
		m_apDisplayFrame[ViewLabel_Axial]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_Sagittal]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_Coronal]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_3D]->setExpanded(false);
		break;
	case OpeBarLayoutLabel_Sagittal:
		while ((m_OperationbarLayout->takeAt(0)) != nullptr);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Sagittal], 0, 1, 3, 1);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Axial], 0, 0);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Coronal], 1, 0);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_3D], 2, 0);
		m_OperationbarLayout->setRowStretch(0, 1);
		m_OperationbarLayout->setRowStretch(1, 1);
		m_OperationbarLayout->setRowStretch(2, 1);
		m_OperationbarLayout->setColumnStretch(0, 1);
		m_OperationbarLayout->setColumnStretch(1, 3);
		this->setLayout(m_OperationbarLayout);
		m_apDisplayFrame[ViewLabel_Axial]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_Sagittal]->setExpanded(true);
		m_apDisplayFrame[ViewLabel_Coronal]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_3D]->setExpanded(false);
		break;
	case OpeBarLayoutLabel_Coronal:
		while ((m_OperationbarLayout->takeAt(0)) != nullptr);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Coronal], 0, 1, 3, 1);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Axial], 0, 0);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Sagittal], 1, 0);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_3D], 2, 0);
		m_OperationbarLayout->setRowStretch(0, 1);
		m_OperationbarLayout->setRowStretch(1, 1);
		m_OperationbarLayout->setRowStretch(2, 1);
		m_OperationbarLayout->setColumnStretch(0, 1);
		m_OperationbarLayout->setColumnStretch(1, 3);
		this->setLayout(m_OperationbarLayout);
		m_apDisplayFrame[ViewLabel_Axial]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_Sagittal]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_Coronal]->setExpanded(true);
		m_apDisplayFrame[ViewLabel_3D]->setExpanded(false);
		break;
	case OpeBarLayoutLabel_Axial:
		while ((m_OperationbarLayout->takeAt(0)) != nullptr);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Axial], 0, 1, 3, 1);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Sagittal], 0, 0);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Coronal], 1, 0);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_3D], 2, 0);
		m_OperationbarLayout->setRowStretch(0, 1);
		m_OperationbarLayout->setRowStretch(1, 1);
		m_OperationbarLayout->setRowStretch(2, 1);
		m_OperationbarLayout->setColumnStretch(0, 1);
		m_OperationbarLayout->setColumnStretch(1, 3);
		this->setLayout(m_OperationbarLayout);
		m_apDisplayFrame[ViewLabel_Axial]->setExpanded(true);
		m_apDisplayFrame[ViewLabel_Sagittal]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_Coronal]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_3D]->setExpanded(false);
		break;
	case OpeBarLayoutLabel_3D:
		while ((m_OperationbarLayout->takeAt(0)) != nullptr);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_3D], 0, 1, 3, 1);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Axial], 0, 0);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Sagittal], 1, 0);
		m_OperationbarLayout->addWidget(m_apDisplayFrame[ViewLabel_Coronal], 2, 0);
		m_OperationbarLayout->setRowStretch(0, 1);
		m_OperationbarLayout->setRowStretch(1, 1);
		m_OperationbarLayout->setRowStretch(2, 1);
		m_OperationbarLayout->setColumnStretch(0, 1);
		m_OperationbarLayout->setColumnStretch(1, 3);
		this->setLayout(m_OperationbarLayout);
		m_apDisplayFrame[ViewLabel_Axial]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_Sagittal]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_Coronal]->setExpanded(false);
		m_apDisplayFrame[ViewLabel_3D]->setExpanded(true);
		break;
	default:
		break;
	}
}

void ASOperationBar::slotButtonLayoutClicked(int viewlabel)
{
	if (m_LayoutMode == viewlabel + 1)
	{
		this->setLayoutMode(OpeBarLayoutLabel_Default);
	}
	else
	{
		this->setLayoutMode(viewlabel + 1);
	}
}

void ASOperationBar::slotCrossShairClicked(int viewlabel)
{
	if (viewlabel == ViewLabel_3D)
	{
		// 切换3D视图中的正交截面是否显示
		bool shown = ASTransGeneralInteractor::Switch3DImageStackShown();
	}
	else
	{
	}
}

void ASOperationBar::slotBackgroundColorChange(bool checked)
{
	QPalette Palette = this->palette();
	if (checked)
		Palette.setColor(this->backgroundRole(), QColor(33, 37, 43));
	else
	{
		Palette.setColor(this->backgroundRole(), QColor(245, 245, 245));
	}
	this->setAutoFillBackground(true);
	this->setPalette(Palette);
}

// 绘制边框
void ASOperationBar::paintEvent(QPaintEvent* e)
{
	bool themechoice = ASTransTheme::GetTheme();
	QPainter painter(this);
	if (themechoice)
		painter.setPen(QColor("#16181c"));
	else
		painter.setPen(QColor("#A0A0A0"));
	painter.drawRoundedRect(0, 0, width(), height() - 1, 0, 0);
	QWidget::paintEvent(e);
}
