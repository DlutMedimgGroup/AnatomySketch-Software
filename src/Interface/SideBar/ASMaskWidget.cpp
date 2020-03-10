#include "ASMaskWidget.h"
#include "ASDataStructureWalker.h"
#include "ASArrayImageData.h"
#include "ASTransSidebarGeneral.h"
#include "ASFModelMask.h"
#include "ASTransInteractorStyle.h"
#include "ASInteractorStyleManager.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

ASMaskWidget::ASMaskWidget(QWidget *parent)
	: ASWidgetInSideBarBase(parent)
{
	//界面初始化
	UIInit();

	// 接收Transmission信号
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalRefreshDataComboBox(int)), this, SLOT(slotRefreshDataComboBox(int)));
	connect(ASTransInteractorStyle::GetSelfPointer(), SIGNAL(signalInteractorStyle2DChanged(int)), this, SLOT(slotInteractorStyle2DChanged(int)));
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalSetMaskInteractorly(double, double, double)), this, SLOT(slotSetMaskInteractor(double, double, double)));
}

ASMaskWidget::~ASMaskWidget()
{

}

void ASMaskWidget::SetEnabled(const bool c_bEnabled)
{
	if (m_crntIDOfComboBox == -1)
	{
		return;
	}
	ASTransSidebarGeneral::SetMaskModleEnabled(c_bEnabled);
	if (c_bEnabled)
	{
		//变为使能
		for (int i = 0; i < 6; i++)
		{
			m_DSBoxCoRange[i]->setEnabled(true);
			m_PushButtonCoRange[i]->setEnabled(true);
		}
		m_DSBoxGrayRange[0]->setEnabled(true);
		m_DSBoxGrayRange[1]->setEnabled(true);
		m_DSBoxMDataGrayRange[0]->setEnabled(true);
		m_DSBoxMDataGrayRange[1]->setEnabled(true);
		this->UpdataSpinBox();
	}
	else
	{
		//变为关闭
		for (int i = 0; i < 6; i++)
		{
			m_DSBoxCoRange[i]->setValue(0);
			m_DSBoxCoRange[i]->setEnabled(false);
			m_PushButtonCoRange[i]->setChecked(false);
			m_PushButtonCoRange[i]->setEnabled(false);
		}
		m_DSBoxGrayRange[0]->setValue(0);
		m_DSBoxGrayRange[1]->setValue(0);
		m_DSBoxGrayRange[0]->setEnabled(false);
		m_DSBoxGrayRange[1]->setEnabled(false);
		m_DSBoxMDataGrayRange[0]->setValue(0);
		m_DSBoxMDataGrayRange[1]->setValue(0);
		m_DSBoxMDataGrayRange[0]->setEnabled(false);
		m_DSBoxMDataGrayRange[1]->setEnabled(false);
	}
}

//界面初始化
void ASMaskWidget::UIInit()
{
	// 0 文件选择
	QHBoxLayout* layout0 = new QHBoxLayout();
	QLabel* label00 = new QLabel("Input:", this);
	label00->setMaximumWidth(40);
	m_ComboBoxInput = new QComboBox();
	connect(m_ComboBoxInput, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCrntIndexChanged(int)));
	layout0->addWidget(label00);
	layout0->addWidget(m_ComboBoxInput);
	
	// 1 坐标范围
	QLabel* label10 = new QLabel(tr("Coordinate Range:"), this);
	QGridLayout* layout1 = new QGridLayout();
	QLabel*	label11 = new QLabel("Min", this);
	QLabel*	label12 = new QLabel("Max", this);
	QLabel*	label13 = new QLabel("X", this);
	label13->setMaximumWidth(10);
	QLabel*	label14 = new QLabel("Y", this);
	label14->setMaximumWidth(10);
	QLabel*	label15 = new QLabel("Z", this);
	label15->setMaximumWidth(10);
	for (int i = 0; i < 6; i++)
	{
		m_DSBoxCoRange[i] = new QDoubleSpinBox(this);
		connect(m_DSBoxCoRange[i], SIGNAL(valueChanged(double)), this, SLOT(slotSettingValueChanged()));
		m_DSBoxCoRange[i]->setEnabled(false);

		m_PushButtonCoRange[i] = new QPushButton(tr("Select"), this);
		m_PushButtonCoRange[i]->setMaximumWidth(40);
		m_PushButtonCoRange[i]->setCheckable(true);
		connect(m_PushButtonCoRange[i], SIGNAL(clicked(bool)), this, SLOT(slotSelectFromView(bool)));		
		m_PushButtonCoRange[i]->setEnabled(false);
	}

	layout1->addWidget(label11, 0, 1); // row = 0, column = 1.
	layout1->addWidget(label12, 0, 3);
	layout1->addWidget(label13, 1, 0);
	layout1->addWidget(label14, 2, 0);
	layout1->addWidget(label15, 3, 0);
	layout1->addWidget(m_DSBoxCoRange[0], 1, 1);
	layout1->addWidget(m_PushButtonCoRange[0], 1, 2);
	layout1->addWidget(m_DSBoxCoRange[1], 1, 3);
	layout1->addWidget(m_PushButtonCoRange[1], 1, 4);
	layout1->addWidget(m_DSBoxCoRange[2], 2, 1);
	layout1->addWidget(m_PushButtonCoRange[2], 2, 2);
	layout1->addWidget(m_DSBoxCoRange[3], 2, 3);
	layout1->addWidget(m_PushButtonCoRange[3], 2, 4);
	layout1->addWidget(m_DSBoxCoRange[4], 3, 1);
	layout1->addWidget(m_PushButtonCoRange[4], 3, 2);
	layout1->addWidget(m_DSBoxCoRange[5], 3, 3);
	layout1->addWidget(m_PushButtonCoRange[5], 3, 4);

	// 2 TabWidget
	m_TabWidget = new QTabWidget(this);
	// 2.1 Tab1 灰度范围
	QWidget* Tab1 = new QWidget(m_TabWidget);
	QLabel* label210 = new QLabel(tr("Greyscale value range(enter to take effect):"), Tab1);
	QLabel* label211 = new QLabel("Min value:", Tab1);
	label211->setMaximumWidth(80);
	QLabel* label212 = new QLabel("Max value:", Tab1);
	label212->setMaximumWidth(80);
	m_DSBoxGrayRange[0] = new QDoubleSpinBox();
	m_DSBoxGrayRange[1] = new QDoubleSpinBox();
	m_DSBoxGrayRange[0]->setEnabled(false);
	m_DSBoxGrayRange[1]->setEnabled(false);
	connect(m_DSBoxGrayRange[0], SIGNAL(editingFinished()), this, SLOT(slotSettingValueChanged()));
	connect(m_DSBoxGrayRange[1], SIGNAL(editingFinished()), this, SLOT(slotSettingValueChanged()));
	QHBoxLayout* layout211 = new QHBoxLayout();
	layout211->addWidget(label211);
	layout211->addWidget(m_DSBoxGrayRange[0]);
	layout211->addSpacing(10);
	layout211->addWidget(label212);
	layout211->addWidget(m_DSBoxGrayRange[1]);
	QVBoxLayout* layout21 = new QVBoxLayout();
	layout21->addWidget(label210);
	layout21->addLayout(layout211);
	layout21->addStretch();
	Tab1->setLayout(layout21);
	// 2.2 Tab2 输入遮罩文件
	QWidget* Tab2 = new QWidget(m_TabWidget);
	QLabel* label220 = new QLabel(tr("Use other mask image:"), Tab2);
	label220->setFixedWidth(150);
	m_ComboBoxMDataInput = new QComboBox(Tab2);
	connect(m_ComboBoxMDataInput, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCrntIndexChangedMData(int)));
	QHBoxLayout* layout221 = new QHBoxLayout();
	layout221->addWidget(label220);
	layout221->addWidget(m_ComboBoxMDataInput);
	QLabel* label222 = new QLabel(tr("Greyscale value range(enter to take effect):"), Tab1);
	QLabel* label223 = new QLabel("Min value:", Tab1);
	label223->setMaximumWidth(80);
	QLabel* label224 = new QLabel("Max value:", Tab1);
	label224->setMaximumWidth(80);
	m_DSBoxMDataGrayRange[0] = new QDoubleSpinBox();
	m_DSBoxMDataGrayRange[1] = new QDoubleSpinBox();
	m_DSBoxMDataGrayRange[0]->setEnabled(false);
	m_DSBoxMDataGrayRange[1]->setEnabled(false);
	connect(m_DSBoxMDataGrayRange[0], SIGNAL(editingFinished()), this, SLOT(slotSettingValueChanged()));
	connect(m_DSBoxMDataGrayRange[1], SIGNAL(editingFinished()), this, SLOT(slotSettingValueChanged()));
	QHBoxLayout* layout222 = new QHBoxLayout();
	layout222->addWidget(label223);
	layout222->addWidget(m_DSBoxMDataGrayRange[0]);
	layout222->addSpacing(10);
	layout222->addWidget(label224);
	layout222->addWidget(m_DSBoxMDataGrayRange[1]);
	m_LabelMDataStatus = new QLabel(tr("Please select image"), this);
	QVBoxLayout* layout22 = new QVBoxLayout();
	layout22->addLayout(layout221);
	layout22->addSpacing(5);
	layout22->addWidget(label222);
	layout22->addLayout(layout222);
	layout22->addWidget(m_LabelMDataStatus);
	Tab2->setLayout(layout22);
	// 2.3 TabWidget	
	m_TabWidget->addTab(Tab1, tr("Greyscale value range"));
	m_TabWidget->addTab(Tab2, tr("Select mask image"));
	// 大小策略
	QSizePolicy Tab1SizePolicy = m_TabWidget->sizePolicy();
	Tab1SizePolicy.setVerticalPolicy(QSizePolicy::Maximum);
	m_TabWidget->setSizePolicy(Tab1SizePolicy);
	connect(m_TabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotTabCurrentChanged(int)));

	// 4 mainlayout
	QVBoxLayout* mainlayout = new QVBoxLayout();
	mainlayout->addLayout(layout0);
	mainlayout->addSpacing(10);
	mainlayout->addWidget(label10);
	mainlayout->addLayout(layout1);
	mainlayout->addSpacing(10);
	mainlayout->addWidget(m_TabWidget);
	mainlayout->setStretch(4, 1);
	mainlayout->addSpacing(10);
	mainlayout->addStretch();
	mainlayout->setStretch(6, 0);
	this->setLayout(mainlayout);
}
// 更新控件，范围、当前值
void ASMaskWidget::UpdataSpinBox()
{
	//图像相关信息
	double adOrigin[3];
	ASFModelMask::GetMaskImageData()->GetOrigin(adOrigin);
	double adSpacing[3];
	ASFModelMask::GetMaskImageData()->GetSpacing(adSpacing);
	int anDimensions[3];
	ASFModelMask::GetMaskImageData()->GetDimensions(anDimensions);

	//更新输入框范围
	isEnabelforslotSettingValueChanged = false;
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_DSBoxCoRange[2 * ViewLabel]->setMaximum(adOrigin[ViewLabel] + anDimensions[ViewLabel] * adSpacing[ViewLabel]);
		m_DSBoxCoRange[2 * ViewLabel]->setMinimum(adOrigin[ViewLabel]);
		m_DSBoxCoRange[2 * ViewLabel + 1]->setMaximum(adOrigin[ViewLabel] + anDimensions[ViewLabel] * adSpacing[ViewLabel]);
		m_DSBoxCoRange[2 * ViewLabel + 1]->setMinimum(adOrigin[ViewLabel]);
	}
	ASArrayImageData* crntDataNode = (ASArrayImageData*)ASAbstractData::getDataNodeFromID(m_crntIDOfComboBox);
	double ScalarRange[2];
	crntDataNode->getArrayImageData()->GetScalarRange(ScalarRange);
	m_DSBoxGrayRange[0]->setMinimum(ScalarRange[0]);
	m_DSBoxGrayRange[0]->setMaximum(ScalarRange[1]);
	m_DSBoxGrayRange[1]->setMinimum(ScalarRange[0]);
	m_DSBoxGrayRange[1]->setMaximum(ScalarRange[1]);

	//设定初始值
	for (int i = 0; i < 6; i++)
	{
		m_DSBoxCoRange[i]->setValue(ASFModelMask::GetMaskExtent()[i]);		
		//m_DoubelSpinBoxCoRange[2 * ViewLabel]->setValue(adOrigin[ViewLabel]);
		//m_DoubelSpinBoxCoRange[2 * ViewLabel + 1]->setValue(adOrigin[ViewLabel] + anDimensions[ViewLabel] * adSpacing[ViewLabel]);
	}	
	m_DSBoxGrayRange[0]->setValue(ASFModelMask::GetMaskScaleRange()[0]);
	m_DSBoxGrayRange[1]->setValue(ASFModelMask::GetMaskScaleRange()[1]);

	isEnabelforslotSettingValueChanged = true;
}
// 判断遮罩图像是否符合要求
void ASMaskWidget::MDataCheck()
{
	if (ASTransSidebarGeneral::GetMaskModel() == 0)
	{
		return;
	}	
	if (m_crntIDOfComboBox < 0 || m_crntIDOfMdata < 0)
	{
		m_LabelMDataStatus->setText(tr("Please select image"));
		QPalette pe;
		pe.setColor(QPalette::WindowText, Qt::red);
		m_LabelMDataStatus->setPalette(pe);
		ASTransSidebarGeneral::SetMaskMData(-1);
	}
	else
	{
		ASArrayImageData* crntTagetData = static_cast<ASArrayImageData*>(ASAbstractData::getDataNodeFromID(m_crntIDOfComboBox));
		ASArrayImageData* crntMDataData = static_cast<ASArrayImageData*>(ASAbstractData::getDataNodeFromID(m_crntIDOfMdata));
		vtkImageData* TagetData = crntTagetData->getArrayImageData();
		vtkImageData* MData = crntMDataData->getArrayImageData();
		double space1[3], space2[3], origin1[3], origin2[3];
		int dim1[3], dim2[3];
		TagetData->GetSpacing(space1);
		TagetData->GetOrigin(origin1);
		TagetData->GetDimensions(dim1);
		MData->GetSpacing(space2);
		MData->GetOrigin(origin2);
		MData->GetDimensions(dim2);
		if (dim1[0] != dim2[0] || dim1[1] != dim2[1] || dim1[2] != dim2[2])
		{
			m_LabelMDataStatus->setText(tr("Image size does not match"));
			QPalette pe;
			pe.setColor(QPalette::WindowText, Qt::red);
			m_LabelMDataStatus->setPalette(pe);
			ASTransSidebarGeneral::SetMaskMData(-1);
		}
		else
		{
			if (space1[0] != space2[0] || space1[1] != space2[1] || space1[2] != space2[2] ||
				origin1[0] != origin2[0] || origin1[1] != origin2[1] || origin1[2] != origin2[2])
			{
				m_LabelMDataStatus->setText(tr("Origin or voxel size does not match, the attribute data of other masks will be used uniformly"));
				QPalette pe;
				pe.setColor(QPalette::WindowText, Qt::black);
				m_LabelMDataStatus->setPalette(pe);
			}
			else
			{
				m_LabelMDataStatus->setText(tr("The image fully meets the requirements"));
				QPalette pe;
				pe.setColor(QPalette::WindowText, Qt::black);
				m_LabelMDataStatus->setPalette(pe);
			}
			ASArrayImageData* crntMDataNode = (ASArrayImageData*)ASAbstractData::getDataNodeFromID(m_crntIDOfMdata);
			double ScalarRange[2];
			crntMDataNode->getArrayImageData()->GetScalarRange(ScalarRange);
			m_DSBoxMDataGrayRange[0]->setMinimum(ScalarRange[0]);
			m_DSBoxMDataGrayRange[0]->setMaximum(ScalarRange[1]);
			m_DSBoxMDataGrayRange[1]->setMinimum(ScalarRange[0]);
			m_DSBoxMDataGrayRange[1]->setMaximum(ScalarRange[1]);
			ASTransSidebarGeneral::SetMaskMData(m_crntIDOfMdata);
			m_DSBoxMDataGrayRange[0]->setValue(ASFModelMask::GetMDataScaleRange()[0]);
			m_DSBoxMDataGrayRange[1]->setValue(ASFModelMask::GetMDataScaleRange()[1]);
		}
	}
}
//文件下拉选框选项改变
void ASMaskWidget::slotCrntIndexChanged(int index)
{
	if (isEnabelforslotComboBoxCurrentIndexChanged == false)
	{
		return;
	}
	if (index == -1)
	{
		m_crntIDOfComboBox = -1;
		/*		m_PushButtonEnable->setChecked(false);
				slotEnablePushButtonClicked();	*/
		ASTransSidebarGeneral::SetMaskWorkingImageID(-1);
		
		return;
	}
	//如果当前order与之前order相同，直接返回
	if (m_listIDForIndex.at(index) == m_crntIDOfComboBox)
	{
		return;
	}
	m_crntIDOfComboBox = m_listIDForIndex.at(index);
	ASTransSidebarGeneral::SetMaskWorkingImageID(m_crntIDOfComboBox);	
	if (ASFModelMask::IsMaskEnable())
	{
		this->UpdataSpinBox();
		slotSettingValueChanged();//参数无意义
	}
	this->MDataCheck();
}
void ASMaskWidget::slotCrntIndexChangedMData(int index)
{
	if (isEnabelforslotComboBoxCurrentIndexChanged == false)
	{
		return;
	}
	if (index == -1)
	{
		m_crntIDOfMdata = -1;

		return;
	}
	if (m_listIDForIndexMData.at(index) == m_crntIDOfMdata)
	{
		return;
	}
	m_crntIDOfMdata = m_listIDForIndexMData.at(index);
	this->MDataCheck();
}

//界面设置参数改变，更新遮罩数组
void ASMaskWidget::slotSettingValueChanged()
{
	//if (m_PushButtonEnable->isChecked() == false)
	//{
	//	return;
	//}
	if (isEnabelforslotSettingValueChanged == false)
	{
		return;
	}
	if (m_crntIDOfComboBox == -1)
	{
		//当前没有条目
		return;
	}

	double Extent[6];
	Extent[0] = m_DSBoxCoRange[0]->value();
	Extent[1] = m_DSBoxCoRange[1]->value();
	Extent[2] = m_DSBoxCoRange[2]->value();
	Extent[3] = m_DSBoxCoRange[3]->value();
	Extent[4] = m_DSBoxCoRange[4]->value();
	Extent[5] = m_DSBoxCoRange[5]->value();
	double ScaleRange[2];
	if (m_TabWidget->currentIndex() == 0)
	{
		ScaleRange[0] = m_DSBoxGrayRange[0]->value();
		ScaleRange[1] = m_DSBoxGrayRange[1]->value();
	} 
	else
	{
		ScaleRange[0] = m_DSBoxMDataGrayRange[0]->value();
		ScaleRange[1] = m_DSBoxMDataGrayRange[1]->value();
	}
	ASTransSidebarGeneral::SetMaskParameter(Extent, ScaleRange);
}

//刷新所有文件下拉选框
void ASMaskWidget::slotRefreshDataComboBox(int c_AffectType)
{
	isEnabelforslotComboBoxCurrentIndexChanged = false;
	// 1 需要添加遮罩的数据的下拉选框	
	// 清空原有内容
	m_ComboBoxInput->clear();
	m_listIDForIndex.clear();
	m_ComboBoxMDataInput->clear();
	m_listIDForIndexMData.clear();
	// 遍历数据，重新添加内容
	bool haveBefor = false;
	bool haveBeforMData = false;
	int IndexAfter = -1;
	int IndexAfterMData = -1;
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_ArrayImageData); item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_ArrayImageData))
	{
		ASArrayImageData* crntDataNode = (ASArrayImageData*)item;
		m_listIDForIndex.append(crntDataNode->getDataNodeID());
		m_ComboBoxInput->addItem(crntDataNode->getName());
		m_listIDForIndexMData.append(crntDataNode->getDataNodeID());
		m_ComboBoxMDataInput->addItem(crntDataNode->getName());
		if (crntDataNode->getDataNodeID() == m_crntIDOfComboBox)
		{
			//找到与刷新之前相同的文件
			IndexAfter = m_listIDForIndex.size() - 1;
			m_ComboBoxInput->setCurrentIndex(IndexAfter);
			haveBefor = true;
		}
		if (crntDataNode->getDataNodeID() == m_crntIDOfMdata)
		{
			IndexAfterMData = m_listIDForIndexMData.size() - 1;
			m_ComboBoxMDataInput->setCurrentIndex(IndexAfterMData);
			haveBeforMData = true;
		}
	}
	if (haveBefor == false)
	{
		// 没找到之前相同的文件
		if (m_ComboBoxInput->count() == 0)
		{
			// 条目为空
			IndexAfter = -1;
			if (ASFModelMask::IsMaskEnable())
			{
				ASTransSidebarGeneral::SetMaskModleEnabled(false);
				for (int i = 0; i < 6; i++)
				{
					m_DSBoxCoRange[i]->setEnabled(false);
				}
				m_DSBoxGrayRange[0]->setEnabled(false);
				m_DSBoxGrayRange[1]->setEnabled(false);
			}
		}
		else
		{
			// 条目不为空
			m_ComboBoxInput->setCurrentIndex(0);
			IndexAfter = 0;
		}
	}
	if (haveBeforMData == false)
	{
		// 没找到之前相同的文件
		if (m_ComboBoxMDataInput->count() == 0)
		{
			// 条目为空
			IndexAfterMData = -1;
		} 
		else
		{
			// 条目不为空
			m_ComboBoxMDataInput->setCurrentIndex(0);
			IndexAfterMData = 0;
		}
	}
	isEnabelforslotComboBoxCurrentIndexChanged = true;
	slotCrntIndexChanged(IndexAfter);
	slotCrntIndexChangedMData(IndexAfterMData);
}

// 在视图中选择遮罩
void ASMaskWidget::slotSelectFromView(bool b)
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	if (b)
	{
		m_PushButtonCoRange[0]->setChecked(false);
		m_PushButtonCoRange[1]->setChecked(false);
		m_PushButtonCoRange[2]->setChecked(false);
		m_PushButtonCoRange[3]->setChecked(false);
		m_PushButtonCoRange[4]->setChecked(false);
		m_PushButtonCoRange[5]->setChecked(false);
		btn->setChecked(true);
		// 改变交互方式
		if (ASInteractorStyleManager::GetInteractorStyle2D() != InterStyle2D_Mask)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_Mask);
		}
	}
	else
	{
		// 改变交互方式
		if (ASInteractorStyleManager::GetInteractorStyle2D() == InterStyle2D_Mask)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_General);
		}
	}
}
// 软件当前交互方式改变
void ASMaskWidget::slotInteractorStyle2DChanged(int interactor)
{
	if (interactor != InterStyle2D_Mask)
	{
		m_PushButtonCoRange[0]->setChecked(false);
		m_PushButtonCoRange[1]->setChecked(false);
		m_PushButtonCoRange[2]->setChecked(false);
		m_PushButtonCoRange[3]->setChecked(false);
		m_PushButtonCoRange[4]->setChecked(false);
		m_PushButtonCoRange[5]->setChecked(false);
	}
}
// 交互设置遮罩
void ASMaskWidget::slotSetMaskInteractor(double x, double y, double z)
{
	if (m_PushButtonCoRange[0]->isChecked())
	{
		m_DSBoxCoRange[0]->setValue(x);
	}
	if (m_PushButtonCoRange[1]->isChecked())
	{
		m_DSBoxCoRange[1]->setValue(x);
	}
	if (m_PushButtonCoRange[2]->isChecked())
	{
		m_DSBoxCoRange[2]->setValue(y);
	}
	if (m_PushButtonCoRange[3]->isChecked())
	{
		m_DSBoxCoRange[3]->setValue(y);
	}
	if (m_PushButtonCoRange[4]->isChecked())
	{
		m_DSBoxCoRange[4]->setValue(z);
	}
	if (m_PushButtonCoRange[5]->isChecked())
	{
		m_DSBoxCoRange[5]->setValue(z);
	}
}
void ASMaskWidget::slotTabCurrentChanged(int index)
{
	ASTransSidebarGeneral::SetMaskModel(index);
	this->slotSettingValueChanged();
}