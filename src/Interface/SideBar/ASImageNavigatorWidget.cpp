#include "ASImageNavigatorWidget.h"
#include "ASTransSidebarGeneral.h"
#include "ASTransGeneralInteractor.h"
#include "ASTransInteractorStyle.h"
#include "ASInteractorStyleManager.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

ASImageNavigatorWidget::ASImageNavigatorWidget(QWidget *parent)
	: ASWidgetInSideBarBase(parent)
{
	GUIInit();

	// 接收Transmission信号
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalSetDisplayPosition(double, double, double)), 
		this, SLOT(slotSetDisplayPosition(double, double, double)));
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalSetDisplayFrame(int)), 
		this, SLOT(slotSetDisplayFrame(int)));
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalSetImageSpacing(double*)), 
		this, SLOT(slotSetImageSpacing(double*)));
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalSetDisplayExtent(double*, int)), 
		this, SLOT(slotSetDisplayExtent(double*, int)));
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalSetWindowLevel(double, double)), 
		this, SLOT(slotSetWindowLevel(double, double)));
}

ASImageNavigatorWidget::~ASImageNavigatorWidget()
{
}

//界面初始化
void ASImageNavigatorWidget::GUIInit()
{

	//焦点，世界坐标系
	QHBoxLayout* HBoxLayout0 = new QHBoxLayout;				//行，焦点世界坐标系
	m_LocationDoubleSpinBox[0] = new QDoubleSpinBox(this);
	m_LocationDoubleSpinBox[0]->setMinimumWidth(20);
	m_LocationDoubleSpinBox[0]->setStyleSheet(
		"QDoubleSpinBox"
		"{"
		"border: 1px solid;"
		"border-color:rgb(107, 33, 0);"
		"}"
	);
	m_LocationDoubleSpinBox[1] = new QDoubleSpinBox(this);
	m_LocationDoubleSpinBox[1]->setMinimumWidth(20);
	m_LocationDoubleSpinBox[1]->setStyleSheet(
		"QDoubleSpinBox"
		"{"
		"border: 1px solid;"
		"border-color:rgb(0, 23, 112);"
		"}"
	);
	m_LocationDoubleSpinBox[2] = new QDoubleSpinBox(this);
	m_LocationDoubleSpinBox[2]->setMinimumWidth(20);
	m_LocationDoubleSpinBox[2]->setStyleSheet(
		"QDoubleSpinBox"
		"{"
		"border: 1px solid;"
		"border-color:rgb(0, 93, 15);"
		"}"
	);

	HBoxLayout0->addWidget(m_LocationDoubleSpinBox[2]);
	HBoxLayout0->addWidget(m_LocationDoubleSpinBox[1]);
	HBoxLayout0->addWidget(m_LocationDoubleSpinBox[0]);

	//焦点，像素坐标系
	QHBoxLayout* HBoxLayout1 = new QHBoxLayout;				//行，Axial
	QHBoxLayout* HBoxLayout2 = new QHBoxLayout;				//行，Sagittal
	QHBoxLayout* HBoxLayout3 = new QHBoxLayout;				//行，Coronal
	QHBoxLayout* HBoxLayout4 = new QHBoxLayout;				//行，Time
	m_FocusSlider[0] = new QSlider(Qt::Horizontal, this);
	m_FocusSlider[1] = new QSlider(Qt::Horizontal, this);
	m_FocusSlider[2] = new QSlider(Qt::Horizontal, this);
	m_FocusSpinBox[0] = new QSpinBox(this);
	m_FocusSpinBox[0]->setFixedWidth(60);
	m_FocusSpinBox[0]->setStyleSheet(
		"QSpinBox"
		"{"
		"border: 1px solid;"
		"border-color:rgb(107, 33, 0);"
		"}"
	);
	m_FocusSpinBox[1] = new QSpinBox(this);
	m_FocusSpinBox[1]->setFixedWidth(60);
	m_FocusSpinBox[1]->setStyleSheet(
		"QSpinBox"
		"{"
		"border: 1px solid;"
		"border-color:rgb(0, 23, 112);"
		"}"
	);
	m_FocusSpinBox[2] = new QSpinBox(this);
	m_FocusSpinBox[2]->setFixedWidth(60);
	m_FocusSpinBox[2]->setStyleSheet(
		"QSpinBox"
		"{"
		"border: 1px solid;"
		"border-color:rgb(0, 93, 15);"
		"}"
	);
	m_TimeSlider = new QSlider(Qt::Horizontal, this);
	m_TimeSpinBox = new QSpinBox(this);
	m_TimeSpinBox->setFixedWidth(60);
	m_TimeSpinBox->setStyleSheet(
		"QSpinBox"
		"{"
		"border: 1px solid #0F0F0F;"
		"}"
	);
	HBoxLayout1->addWidget(m_FocusSlider[0]);
	HBoxLayout1->addWidget(m_FocusSpinBox[0]);
	HBoxLayout2->addWidget(m_FocusSlider[1]);
	HBoxLayout2->addWidget(m_FocusSpinBox[1]);
	HBoxLayout3->addWidget(m_FocusSlider[2]);
	HBoxLayout3->addWidget(m_FocusSpinBox[2]);
	HBoxLayout4->addWidget(m_TimeSlider);
	HBoxLayout4->addWidget(m_TimeSpinBox);

	//右上部分
	QVBoxLayout* VBoxLayout0 = new QVBoxLayout;
	VBoxLayout0->addLayout(HBoxLayout0);
	VBoxLayout0->addLayout(HBoxLayout1);
	VBoxLayout0->addLayout(HBoxLayout2);
	VBoxLayout0->addLayout(HBoxLayout3);
	VBoxLayout0->addLayout(HBoxLayout4);

	//左上部分
	QVBoxLayout* VBoxLayout1 = new QVBoxLayout;
	m_Label[0] = new QLabel("Location(mm)");
	m_Label[1] = new QLabel("Axial");
	m_Label[2] = new QLabel("Sagittal");
	m_Label[3] = new QLabel("Coronal");
	m_Label[4] = new QLabel("Time");
	VBoxLayout1->addWidget(m_Label[0]);
	VBoxLayout1->addWidget(m_Label[1]);
	VBoxLayout1->addWidget(m_Label[2]);
	VBoxLayout1->addWidget(m_Label[3]);
	VBoxLayout1->addWidget(m_Label[4]);

	//上部分
	QHBoxLayout* HBoxLayout5 = new QHBoxLayout;
	HBoxLayout5->addLayout(VBoxLayout1);
	HBoxLayout5->addLayout(VBoxLayout0);

	//窗位窗宽
	QHBoxLayout* HBoxLayout6 = new QHBoxLayout;
	m_LevelWindowDoubleSpinBox[0] = new QDoubleSpinBox(this);
	m_LevelWindowDoubleSpinBox[1] = new QDoubleSpinBox(this);
	m_LevelWindowDoubleSpinBox[0]->setMinimumWidth(30);
	m_LevelWindowDoubleSpinBox[1]->setMinimumWidth(30);
	m_Label[5] = new QLabel("Level");
	m_Label[6] = new QLabel("Window");
	HBoxLayout6->addWidget(m_Label[5]);
	HBoxLayout6->addWidget(m_LevelWindowDoubleSpinBox[0]);
	HBoxLayout6->addWidget(m_Label[6]);
	HBoxLayout6->addWidget(m_LevelWindowDoubleSpinBox[1]);

	//下部分
	QVBoxLayout* VBoxLayout2 = new QVBoxLayout;
	VBoxLayout2->addLayout(HBoxLayout6);

	//总布局
	QVBoxLayout* MainLayout = new QVBoxLayout;
	MainLayout->addLayout(HBoxLayout5);
	MainLayout->addLayout(VBoxLayout2);
	MainLayout->addStretch();

	this->setLayout(MainLayout);

	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_LocationDoubleSpinBox[ViewLabel]->setMaximum(9999999);
		m_LocationDoubleSpinBox[ViewLabel]->setMinimum(-9999999);
		m_FocusSpinBox[ViewLabel]->setMinimum(0);
		m_FocusSpinBox[ViewLabel]->setMaximum(9999999);
	}
	m_LevelWindowDoubleSpinBox[0]->setMaximum(9999999);
	m_LevelWindowDoubleSpinBox[0]->setMinimum(-9999999);
	m_LevelWindowDoubleSpinBox[1]->setMaximum(9999999);
	m_LevelWindowDoubleSpinBox[1]->setMinimum(-9999999);
	m_LevelWindowDoubleSpinBox[0]->setEnabled(false);
	m_LevelWindowDoubleSpinBox[1]->setEnabled(false);

	m_ImageExtent[0] = -1;
	m_ImageExtent[1] = 1;
	m_ImageExtent[2] = -1;
	m_ImageExtent[3] = 1;
	m_ImageExtent[4] = -1;
	m_ImageExtent[5] = 1;
	m_ImageLocation[0] = 0;
	m_ImageLocation[1] = 0;
	m_ImageLocation[2] = 0;
	m_ImageSpace[0] = 1;
	m_ImageSpace[1] = 1;
	m_ImageSpace[2] = 1;

	//控件信号槽链接
	connect(this->m_LocationDoubleSpinBox[0], SIGNAL(valueChanged(double)), this, SLOT(slotLocationXValueChanged(double)));
	connect(this->m_LocationDoubleSpinBox[1], SIGNAL(valueChanged(double)), this, SLOT(slotLocationYValueChanged(double)));
	connect(this->m_LocationDoubleSpinBox[2], SIGNAL(valueChanged(double)), this, SLOT(slotLocationZValueChanged(double)));
	connect(this->m_FocusSpinBox[0], SIGNAL(valueChanged(int)), this, SLOT(slotFocusXValueChanged(int)));
	connect(this->m_FocusSpinBox[1], SIGNAL(valueChanged(int)), this, SLOT(slotFocusYValueChanged(int)));
	connect(this->m_FocusSpinBox[2], SIGNAL(valueChanged(int)), this, SLOT(slotFocusZValueChanged(int)));
	connect(this->m_FocusSlider[0], SIGNAL(valueChanged(int)), this, SLOT(slotSliderXValueChanged(int)));
	connect(this->m_FocusSlider[1], SIGNAL(valueChanged(int)), this, SLOT(slotSliderYValueChanged(int)));
	connect(this->m_FocusSlider[2], SIGNAL(valueChanged(int)), this, SLOT(slotSliderZValueChanged(int)));
	connect(this->m_TimeSlider, SIGNAL(valueChanged(int)), this, SLOT(slotTimeValueChanged(int)));
	connect(this->m_TimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(slotTimeValueChanged(int)));

	connect(this->m_LevelWindowDoubleSpinBox[0], SIGNAL(valueChanged(double)), 
		this, SLOT(slotLevelValueChanged(double)));
	connect(this->m_LevelWindowDoubleSpinBox[1], SIGNAL(valueChanged(double)), 
		this, SLOT(slotWindowValueChanged(double)));
	connect(this->m_LevelWindowDoubleSpinBox[0], SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateLevel()));
	connect(this->m_LevelWindowDoubleSpinBox[1], SIGNAL(valueChanged(double)),
		this, SLOT(slotUpdateWindow()));

}
//将新的焦点更新到控件显示
void ASImageNavigatorWidget::GeneratePosition()
{
	isValueChanging = true;
	int aFocus[3];	//像素坐标系焦点
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_LocationDoubleSpinBox[ViewLabel]->setValue(m_ImageLocation[ViewLabel]);
		aFocus[ViewLabel] = (int)((m_ImageLocation[ViewLabel] - m_ImageExtent[2 * ViewLabel]) / m_ImageSpace[ViewLabel]);
		m_FocusSpinBox[ViewLabel]->setValue(aFocus[ViewLabel]);
		m_FocusSlider[ViewLabel]->setValue(aFocus[ViewLabel]);
	}
	m_TimeSpinBox->setValue(m_crntFrame);
	m_TimeSlider->setValue(m_crntFrame);
	isValueChanging = false;
}

//-----------------------------控件值改变------------------------------

void ASImageNavigatorWidget::slotLocationXValueChanged(double d)
{
	if (isValueChanging)
	{
		return;
	}

	if (d > m_ImageExtent[1])
	{
		m_ImageLocation[0] = m_ImageExtent[1];
	}
	else if (d < m_ImageExtent[0])
	{
		m_ImageLocation[0] = m_ImageExtent[0];
	}
	else
	{
		m_ImageLocation[0] = d;
	}
	ASTransGeneralInteractor::SetVisualizationPosition(m_ImageLocation[0], m_ImageLocation[1], m_ImageLocation[2]);
}
void ASImageNavigatorWidget::slotLocationYValueChanged(double d)
{
	if (isValueChanging)
	{
		return;
	}

	if (d > m_ImageExtent[3])
	{
		m_ImageLocation[1] = m_ImageExtent[3];
	}
	else if (d < m_ImageExtent[2])
	{
		m_ImageLocation[1] = m_ImageExtent[2];
	}
	else
	{
		m_ImageLocation[1] = d;
	}
	ASTransGeneralInteractor::SetVisualizationPosition(m_ImageLocation[0], m_ImageLocation[1], m_ImageLocation[2]);
}
void ASImageNavigatorWidget::slotLocationZValueChanged(double d)
{
	if (isValueChanging)
	{
		return;
	}

	if (d > m_ImageExtent[5])
	{
		m_ImageLocation[2] = m_ImageExtent[5];
	}
	else if (d < m_ImageExtent[4])
	{
		m_ImageLocation[2] = m_ImageExtent[4];
	}
	else
	{
		m_ImageLocation[2] = d;
	}
	ASTransGeneralInteractor::SetVisualizationPosition(m_ImageLocation[0], m_ImageLocation[1], m_ImageLocation[2]);
}
void ASImageNavigatorWidget::slotFocusXValueChanged(int i)
{
	if (isValueChanging)
	{
		return;
	}
	double d = i*m_ImageSpace[0] + m_ImageExtent[0];
	if (d > m_ImageExtent[1])
	{
		m_ImageLocation[0] = m_ImageExtent[1];
	}
	else if (d < m_ImageExtent[0])
	{
		m_ImageLocation[0] = m_ImageExtent[0];
	}
	else
	{
		m_ImageLocation[0] = d;
	}
	ASTransGeneralInteractor::SetVisualizationPosition(m_ImageLocation[0], m_ImageLocation[1], m_ImageLocation[2]);
}
void ASImageNavigatorWidget::slotFocusYValueChanged(int i)
{
	if (isValueChanging)
	{
		return;
	}
	double d = i*m_ImageSpace[1] + m_ImageExtent[2];
	if (d > m_ImageExtent[3])
	{
		m_ImageLocation[1] = m_ImageExtent[3];
	}
	else if (d < m_ImageExtent[2])
	{
		m_ImageLocation[1] = m_ImageExtent[2];
	}
	else
	{
		m_ImageLocation[1] = d;
	}
	ASTransGeneralInteractor::SetVisualizationPosition(m_ImageLocation[0], m_ImageLocation[1], m_ImageLocation[2]);
}
void ASImageNavigatorWidget::slotFocusZValueChanged(int i)
{
	if (isValueChanging)
	{
		return;
	}
	double d = i*m_ImageSpace[2] + m_ImageExtent[4];
	if (d > m_ImageExtent[5])
	{
		m_ImageLocation[2] = m_ImageExtent[5];
	}
	else if (d < m_ImageExtent[4])
	{
		m_ImageLocation[2] = m_ImageExtent[4];
	}
	else
	{
		m_ImageLocation[2] = d;
	}
	ASTransGeneralInteractor::SetVisualizationPosition(m_ImageLocation[0], m_ImageLocation[1], m_ImageLocation[2]);
}
void ASImageNavigatorWidget::slotSliderXValueChanged(int i)
{
	if (isValueChanging)
	{
		return;
	}
	double d = i*m_ImageSpace[0] + m_ImageExtent[0];
	if (d > m_ImageExtent[1])
	{
		m_ImageLocation[0] = m_ImageExtent[1];
	}
	else if (d < m_ImageExtent[0])
	{
		m_ImageLocation[0] = m_ImageExtent[0];
	}
	else
	{
		m_ImageLocation[0] = d;
	}
	ASTransGeneralInteractor::SetVisualizationPosition(m_ImageLocation[0], m_ImageLocation[1], m_ImageLocation[2]);
}
void ASImageNavigatorWidget::slotSliderYValueChanged(int i)
{
	if (isValueChanging)
	{
		return;
	}
	double d = i*m_ImageSpace[1] + m_ImageExtent[2];
	if (d > m_ImageExtent[3])
	{
		m_ImageLocation[1] = m_ImageExtent[3];
	}
	else if (d < m_ImageExtent[2])
	{
		m_ImageLocation[1] = m_ImageExtent[2];
	}
	else
	{
		m_ImageLocation[1] = d;
	}
	ASTransGeneralInteractor::SetVisualizationPosition(m_ImageLocation[0], m_ImageLocation[1], m_ImageLocation[2]);
}
void ASImageNavigatorWidget::slotSliderZValueChanged(int i)
{
	if (isValueChanging)
	{
		return;
	}
	double d = i*m_ImageSpace[2] + m_ImageExtent[4];
	if (d > m_ImageExtent[5])
	{
		m_ImageLocation[2] = m_ImageExtent[5];
	}
	else if (d < m_ImageExtent[4])
	{
		m_ImageLocation[2] = m_ImageExtent[4];
	}
	else
	{
		m_ImageLocation[2] = d;
	}
	ASTransGeneralInteractor::SetVisualizationPosition(m_ImageLocation[0], m_ImageLocation[1], m_ImageLocation[2]);
}
void ASImageNavigatorWidget::slotTimeValueChanged(int i)
{
	if (isValueChanging)
	{
		return;
	}
	if (i > m_NumOfFrames - 1)
	{
		m_crntFrame = m_NumOfFrames - 1;
	}
	else if (i < 0)
	{
		m_crntFrame = 0;
	}
	else
	{
		m_crntFrame = i;
	}
	ASTransGeneralInteractor::SetVisualizationFrame(m_crntFrame);
}
void ASImageNavigatorWidget::slotLevelValueChanged(double d)
{
	if (isValueChanging)
	{
		return;
	}
	ASTransGeneralInteractor::SetVisualizationLevelWindow(
		m_LevelWindowDoubleSpinBox[0]->value(), m_LevelWindowDoubleSpinBox[1]->value());
}
void ASImageNavigatorWidget::slotWindowValueChanged(double d)
{
	if (isValueChanging)
	{
		return;
	}
	if (d < 0.01)
	{
		m_LevelWindowDoubleSpinBox[1]->setValue(0.01);
	}
	ASTransGeneralInteractor::SetVisualizationLevelWindow(
		m_LevelWindowDoubleSpinBox[0]->value(), m_LevelWindowDoubleSpinBox[1]->value());
}

// 被动更新焦点
void ASImageNavigatorWidget::slotSetDisplayPosition(double PosX, double PosY, double PosZ)
{
	m_ImageLocation[0] = PosX;
	m_ImageLocation[1] = PosY;
	m_ImageLocation[2] = PosZ;
	GeneratePosition();
}
// 被动更新帧
void ASImageNavigatorWidget::slotSetDisplayFrame(const int c_Frame)
{
	m_crntFrame = c_Frame;
	GeneratePosition();
}
// 更新像素尺寸
void ASImageNavigatorWidget::slotSetImageSpacing(double* Spacing)
{
	m_ImageSpace[0] = Spacing[0];
	m_ImageSpace[1] = Spacing[1];
	m_ImageSpace[2] = Spacing[2];
	isValueChanging = true;
	m_FocusSlider[0]->setMaximum((m_ImageExtent[1] - m_ImageExtent[0]) / m_ImageSpace[0]);
	m_FocusSlider[1]->setMaximum((m_ImageExtent[3] - m_ImageExtent[2]) / m_ImageSpace[1]);
	m_FocusSlider[2]->setMaximum((m_ImageExtent[5] - m_ImageExtent[4]) / m_ImageSpace[2]);
	GeneratePosition();
}
// 更新图像范围和帧数
void ASImageNavigatorWidget::slotSetDisplayExtent(double* Extent, const int c_NumOfFrames)
{
	for (int i = 0; i < 6; i++)
	{
		m_ImageExtent[i] = Extent[i];
	}
	m_NumOfFrames = c_NumOfFrames;
	isValueChanging = true;
	m_FocusSlider[0]->setMaximum((m_ImageExtent[1] - m_ImageExtent[0]) / m_ImageSpace[0]);
	m_FocusSlider[1]->setMaximum((m_ImageExtent[3] - m_ImageExtent[2]) / m_ImageSpace[1]);
	m_FocusSlider[2]->setMaximum((m_ImageExtent[5] - m_ImageExtent[4]) / m_ImageSpace[2]);
	m_TimeSlider->setMaximum(m_NumOfFrames - 1);
	m_TimeSpinBox->setMaximum(m_NumOfFrames - 1);
	isValueChanging = false;
	GeneratePosition();
}
// 被动更新窗位窗宽
void ASImageNavigatorWidget::slotSetWindowLevel(double window, double level)
{
	if (window == -1)
	{
		isValueChanging = true;
		m_LevelWindowDoubleSpinBox[0]->setValue(0);
		m_LevelWindowDoubleSpinBox[1]->setValue(0);
		isValueChanging = false;
		m_LevelWindowDoubleSpinBox[0]->setEnabled(false);
		m_LevelWindowDoubleSpinBox[1]->setEnabled(false);
	}
	else
	{
		m_LevelWindowDoubleSpinBox[0]->setEnabled(true);
		m_LevelWindowDoubleSpinBox[1]->setEnabled(true);
		isValueChanging = true;
		m_LevelWindowDoubleSpinBox[0]->setValue(level);
		m_LevelWindowDoubleSpinBox[1]->setValue(window);
		isValueChanging = false;
	}
}

void ASImageNavigatorWidget::slotUpdateWindow()
{
	ASTransGeneralInteractor::SetWindow(m_LevelWindowDoubleSpinBox[1]->value());
}

void ASImageNavigatorWidget::slotUpdateLevel()
{
	ASTransGeneralInteractor::SetLevel(m_LevelWindowDoubleSpinBox[0]->value());
}
