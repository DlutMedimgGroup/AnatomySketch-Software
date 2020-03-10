#include "ASTitleWidget.h"
#include "ASApplication.h"
#include "ASPushButtonInTitleWidget.h"
#include "AboutWidget.h"
#include "ASHelpWidget.h"
#include "ASMainWidget.h"
#include "ASTransSeeds.h"
#include "ASTransContour.h"
#include "ASTransLandmark.h"
#include "ASTransAddDeleteData.h"
#include "ASTransDICOMLoader.h"
#include "ASTransROI.h"
#include "ASTransTheme.h"
#include "ASTransSetting.h"
#include "ASTransLang.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <QSettings>

ASTitleWidget::ASTitleWidget(QWidget *parent)
	: QWidget(parent)
{
	this->UIInit();
	setFixedHeight(TITLE_HEIGHT);

	//在鼠标没有点击是跟踪鼠标移动，用于窗口边界缩放
	setMouseTracking(true);

	mouse_press = false;
	//子界面
	m_AboutWidget = new AboutWidget();
	m_HelpWidget = new ASHelpWidget();
}

ASTitleWidget::~ASTitleWidget()
{

}

void ASTitleWidget::UIInit()
{
	//实例化按钮
	m_min_button = new ASPushButtonInTitleWidget(this);
	m_max_button = new ASPushButtonInTitleWidget(this);
	m_close_button = new ASPushButtonInTitleWidget(this);
	m_min_button->setAutoDefault(false);
	m_max_button->setAutoDefault(false);
	m_close_button->setAutoDefault(false);

	//设置图片
	m_min_button->setPicName(QString(":/sysButton/min"));
	m_min_button->setButtomSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_max_button->setPicName(QString(":/sysButton/maxcancel"));
	m_max_button->setButtomSize(BUTTON_WIDTH, BUTTON_HEIGHT);
	m_close_button->setPicName(QString(":/sysButton/close"));
	m_close_button->setButtomSize(BUTTON_WIDTH, BUTTON_HEIGHT);

	connect(m_min_button, SIGNAL(clicked()), this, SIGNAL(showMin()));
	connect(m_max_button, SIGNAL(clicked()), this, SLOT(maxClicked()));
	connect(m_close_button, SIGNAL(clicked()), this, SIGNAL(programexit()));

	m_BtnOpen = new QPushButton(tr("Open"), this);
	m_BtnOpen->setAutoDefault(false);
	m_BtnOpen->setObjectName("BtnOpen");
	m_BtnOpen->setShortcut(QKeySequence(QLatin1String("Alt+F")));
	connect(this->m_BtnOpen, SIGNAL(clicked()), this, SLOT(slotOpen()));
	m_BtnDICOM = new QPushButton(tr("DICOM"), this);
	m_BtnDICOM->setAutoDefault(false);
	m_BtnDICOM->setObjectName("BtnDICOM");
	m_BtnDICOM->setShortcut(QKeySequence(QLatin1String("Alt+D")));
	connect(this->m_BtnDICOM, SIGNAL(clicked()), this, SLOT(slotDICOM()));
	m_BtnHelp = new QPushButton(tr("Help"), this);
	m_BtnHelp->setAutoDefault(false);
	m_BtnHelp->setObjectName("BtnHelp");
	m_BtnHelp->setShortcut(QKeySequence(QLatin1String("F1")));
	connect(this->m_BtnHelp, SIGNAL(clicked()), this, SLOT(slotHelp()));
	m_BtnAbout = new QPushButton(tr("About"), this);
	m_BtnAbout->setAutoDefault(false);
	m_BtnAbout->setObjectName("BtnAbout");
	m_BtnAbout->setShortcut(QKeySequence(QLatin1String("Alt+B")));
	connect(this->m_BtnAbout, SIGNAL(clicked()), this, SLOT(slotAbout()));
	m_BtnTheme = new QPushButton(tr(""), this);
	m_BtnTheme->setMouseTracking(true);
	m_BtnTheme->setAutoDefault(false);
	m_BtnTheme->setObjectName("BtnTheme");
	m_BtnTheme->setShortcut(QKeySequence(QLatin1String("F11")));
	m_BtnTheme->setCheckable(true); 
	bool default = ASTransSetting::GetSetting()->value("DefaultTheme").toBool();
	m_BtnTheme->setChecked(default);
	connect(this->m_BtnTheme, SIGNAL(clicked(bool)), this, SLOT(slotQSSChange(bool)));
	connect(this->m_BtnTheme, SIGNAL(clicked(bool)), this, SLOT(slotOperationbarThemeChange(bool)));
	connect(this->m_BtnTheme, SIGNAL(clicked(bool)), this, SLOT(slotBackgroundChange(bool)));
	connect(this->m_BtnTheme, SIGNAL(clicked(bool)), this, SLOT(slotSidebarChange(bool)));
	//m_BtnLang = new QPushButton(tr("中"), this);
	//m_BtnLang->setMouseTracking(true);
	//m_BtnLang->setAutoDefault(false);
	//m_BtnLang->setObjectName("BtnLang");
	//m_BtnLang->setShortcut(QKeySequence(QLatin1String("F12")));
	//m_BtnLang->setCheckable(true);
	bool defaultLang = ASTransSetting::GetSetting()->value("DefaultLang").toBool();
	//m_BtnLang->setChecked(defaultLang);
	//connect(this->m_BtnLang, SIGNAL(clicked(bool)), this, SLOT(slotLanguageChange(bool)));

	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addWidget(m_BtnOpen);
	mainLayout->addWidget(m_BtnDICOM);
	mainLayout->addWidget(m_BtnHelp);
	mainLayout->addWidget(m_BtnAbout);
	mainLayout->addStretch();
	//mainLayout->addWidget(m_BtnLang, 0, Qt::AlignTop);
	mainLayout->addWidget(m_BtnTheme, 0, Qt::AlignTop);
	mainLayout->addWidget(m_min_button, 0, Qt::AlignTop);
	mainLayout->addWidget(m_max_button, 0, Qt::AlignTop);
	mainLayout->addWidget(m_close_button, 0, Qt::AlignTop);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 5, 0);
	setLayout(mainLayout);
}

void ASTitleWidget::maxClicked()
{
	
	if (is_Maximized == false) // 当前不是最大化
	{
		is_Maximized = true;
		emit showMax();		
		m_max_button->setPicName(QString(":/sysButton/maxcancel"));
		m_max_button->setButtomSize(BUTTON_WIDTH, BUTTON_HEIGHT);
		m_max_button->repaint();
	}
	else // 当前为最大化
	{
		is_Maximized = false;
		emit ShowNormal();
		m_max_button->setPicName(QString(":/sysButton/max"));
		m_max_button->setButtomSize(BUTTON_WIDTH, BUTTON_HEIGHT);
		m_max_button->repaint();
	}

}
// 打开
void ASTitleWidget::slotOpen()
{
	QString lastPath = ASTransSetting::GetSetting()->value("LastFilePath").toString();
	//打开文件标准对话框
	QStringList OperatingFileNames = QFileDialog::getOpenFileNames(ASMainWidget::GetMainWidget(),	// 指定父类
		"Filename",												// 文件对话框标题栏名称
		lastPath,														// 指定打开文件目录
		"Image File(*mhd *nii *nii.gz *bmp *jpg *png *tif *stl *ply *obj *seed *ctr *ldm *roi);;DICOM-RT(*dcm)");			// 指定打开文件类型
	if (OperatingFileNames.size() < 1)
	{
		return;
	}
	QFileInfo fi(OperatingFileNames.first());
	ASTransSetting::GetSetting()->setValue("LastFilePath", QVariant(fi.path()));
	// 把 seed, ctr, ldm, roi 格式挑出来后读入
	QStringList seedFileNames;
	QStringList ctrFileNames;
	QStringList ldmFileNames;
	QStringList roiFileNames;
	for (int nit = 0; nit < OperatingFileNames.size();)
	{
		QFileInfo pFileInfo = QFileInfo(OperatingFileNames.at(nit));
		QString qstrSuffix = pFileInfo.suffix();
		if (strcmp(qstrSuffix.toStdString().c_str(), "seed") == 0)
		{
			seedFileNames.append(OperatingFileNames.at(nit));
			OperatingFileNames.removeAt(nit);
		}
		else if (strcmp(qstrSuffix.toStdString().c_str(), "ctr") == 0)
		{
			ctrFileNames.append(OperatingFileNames.at(nit));
			OperatingFileNames.removeAt(nit);
		}
		else if (strcmp(qstrSuffix.toStdString().c_str(), "ldm") == 0)
		{
			ldmFileNames.append(OperatingFileNames.at(nit));
			OperatingFileNames.removeAt(nit);
		}
		else if (strcmp(qstrSuffix.toStdString().c_str(), "roi") == 0)
		{
			roiFileNames.append(OperatingFileNames.at(nit));
			OperatingFileNames.removeAt(nit);
		}
		else
		{
			nit++;
		}
	}
	ASTransAddDeleteData::AddLoadWaitingArea(seedFileNames, 0);
	ASTransAddDeleteData::AddLoadWaitingArea(ctrFileNames, 1);
	ASTransAddDeleteData::AddLoadWaitingArea(ldmFileNames, 2);
	ASTransAddDeleteData::AddLoadWaitingArea(roiFileNames, 3);
	ASTransAddDeleteData::LoadDataFromDisk(OperatingFileNames);
}
// DICOM
void ASTitleWidget::slotDICOM()
{
	// 打开DICOM文件扫描的对话框
	ASTransDICOMLoader::OpenDICOMScanWidget();
}
// Help
void ASTitleWidget::slotHelp()
{
	m_HelpWidget->show();
}
// About
void ASTitleWidget::slotAbout()
{
	m_AboutWidget->show();
}
// Theme
void ASTitleWidget::slotQSSChange(bool checked)
{
	ASTransTheme::QSSChange(checked);
}

void ASTitleWidget::slotOperationbarThemeChange(bool checked)
{
	ASTransTheme::OperationbarThemeChange(checked);
}

void ASTitleWidget::slotBackgroundChange(bool checked)
{
	ASTransTheme::BackgroundChange(checked);
}

void ASTitleWidget::slotSidebarChange(bool checked)
{
	ASTransTheme::SidebarChange(checked);
}

void ASTitleWidget::slotLanguageChange(bool checked)
{
	ASTransLang::LanguageChange(checked);
}

void ASTitleWidget::mousePressEvent(QMouseEvent *event)
{
	//只能是鼠标左键移动和改变大小
	if (event->button() == Qt::LeftButton)
	{
		mouse_press = true;
	}

	//窗口移动距离
	move_point = event->globalPos() - this->parentWidget()->pos();
	//move_point = event->globalPos();

	event->ignore();
}

void ASTitleWidget::mouseReleaseEvent(QMouseEvent *event)
{
	mouse_press = false;
	event->ignore();
}

void ASTitleWidget::mouseMoveEvent(QMouseEvent *event)
{
	//移动窗口
	if (mouse_press && is_Maximized == false)
	{
		QPoint move_pos = event->globalPos();
		emit movewindow(move_pos.x() - move_point.x(), move_pos.y() - move_point.y());
	}
	event->ignore();
}

void ASTitleWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (is_Maximized == false) // 当前不是最大化
	{
		is_Maximized = true;
		emit showMax();
		m_max_button->setPicName(QString(":/sysButton/maxcancel"));
		m_max_button->setButtomSize(BUTTON_WIDTH, BUTTON_HEIGHT);
		m_max_button->repaint();
	}
	else // 当前为最大化
	{
		is_Maximized = false;
		emit ShowNormal();
		m_max_button->setPicName(QString(":/sysButton/max"));
		m_max_button->setButtomSize(BUTTON_WIDTH, BUTTON_HEIGHT);
		m_max_button->repaint();
	}
}

