#include "ASOperationWidget.h"
#include "ASFModelAddins.h"
#include "ASAddinsBase.h"
#include "ASAddinsInputBase.h"
#include "ASAddinsBase.h"
#include "ASAddinsInputArrayImage.h"
#include "ASAddinsInputMask.h"
#include "ASAddinsInputLandmark.h"
#include "ASAddinsInputContour.h"
#include "ASAddinsInputSeed.h"
#include "ASAddinsInputROI.h"
#include "ASAddinsInputSwitch.h"
#include "ASAddinsInputPara.h"
#include "ASAddinsInputPolyImage.h"
#include "ASAddinsOutputBase.h"
#include "ASAddinsOutputParameter.h"
#include "ASAbstractData.h"
#include "ASLandmarkData.h"
#include "ASContourData.h"
#include "ASSeedData.h"
#include "ASROIData.h"
#include "ASDataStructureWalker.h"
#include "ASTransAddins.h"
#include "ASOperation.h"
#include "ASArrayImageData.h"
#include "ASPolyImageData.h"
#include "ASAddinsComboBox.h"
#include "ASTransAddDeleteData.h"
#include "ASAddinsBase.h"
#include <QLabel>
#include <QDoubleSpinBox>
#include <QTextCodec>

ASOperationWidget::ASOperationWidget(QWidget *parent)
	: ASWidgetInSideBarBase(parent)
{
	m_Combox = new QComboBox(this);
	connect(m_Combox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotComboxChanged()));
	m_Description = new QLabel(this);
	m_Description->setWordWrap(true);
	m_Description->setAlignment(Qt::AlignTop);
	m_mainlayout = new QVBoxLayout();
	m_mainlayout->addWidget(m_Combox);
	m_mainlayout->addStretch();
	this->setLayout(m_mainlayout);
	m_GeneButn = new QPushButton(tr("Calculate"), this);
	connect(m_GeneButn, SIGNAL(clicked()), this, SLOT(slotGenerate()));
	m_GeneButn->hide();
	m_BtnFinish = new QPushButton(tr(u8"完成"), this);
	connect(m_BtnFinish, SIGNAL(clicked()), this, SLOT(slotFinish()));
	m_BtnFinish->hide();

	connect(ASTransAddins::GetSelfPointer(), SIGNAL(signalRefreshWidget()),
		this, SLOT(slotRefreshWidget()));
	connect(ASTransAddins::GetSelfPointer(), SIGNAL(signalCurrentSegChanged()),
		this, SLOT(slotCrntOperationChanged()));
	connect(ASTransAddDeleteData::GetSelfPointer(), SIGNAL(signalUpdateDataNodeLists(int)),
		this, SLOT(slotUpdateDataNodeLists(int)));
	connect(ASTransAddins::GetSelfPointer(), SIGNAL(signalSetCurrentMethod(QString)),
		this, SLOT(slotSetCurrentMethod(QString)));
}

ASOperationWidget::~ASOperationWidget()
{

}

void ASOperationWidget::SetEnabled(const bool c_bEnabled)
{
	if (!c_bEnabled && m_FlagFromAddin)
	{
		m_FlagFromAddin = false;
		m_BtnFinish->hide();
	}
}

void ASOperationWidget::ComboInit(const int c_AddinType)
{
	m_GeneratingCombo = true;
	m_Combox->clear();
	switch (c_AddinType)
	{
	case AddinType_ImgProcess:
		m_Combox->addItem(tr("Please select image processing method"));
		break;
	case AddinType_MeshProcess:
		m_Combox->addItem(tr("Please select mesh processing method"));
		break;
	case AddinType_Segmentation:
		m_Combox->addItem(tr("Please select segmentation method"));
		break;
	default:
		break;
	}
	int NumOfAddins = ASFModelAddins::GetNumOfAddins();
	for (int index = 0; index < NumOfAddins; index++)
	{
		structAddin* addin = ASFModelAddins::GetAddinAt(index);
		if (addin->type == c_AddinType)
		{
			m_Combox->addItem(addin->name);
		}
	}
	m_GeneratingCombo = false;

	ASOperation* crntOperation = ASTransAddins::GetCrntOperation();
	if (crntOperation)
	{
		ASAddinsBase* crntAddins = crntOperation->GetAddin();
		if (crntAddins)
		{
			int index = m_Combox->findText(crntAddins->GetName());
			if (index >= 0)
			{
				m_Combox->setCurrentIndex(index);
			}
			else
			{
				m_Combox->setCurrentIndex(0);
			}
		}
	}
}
void ASOperationWidget::RemoveLayout(QLayout* layout)
{
	QLayoutItem* child;
	while (layout->count() != 0)
	{
		child = layout->takeAt(0);
		if (child->layout() != 0)
		{
			RemoveLayout(child->layout());
		}
		else if (child->widget() != 0 && child->widget() != m_Combox
			&& child->widget() != m_GeneButn)
		{
			child->widget()->hide();
			layout->removeWidget(child->widget());
			if (child->widget() != m_Description)
			{
				QLabel * label = qobject_cast<QLabel*>(child->widget());
				if (label)
				{
					delete label;
				}
			}		
		}
		delete child;
	}
}

// 根据当前操作刷新界面
void ASOperationWidget::RefreshCrntOperationUI()
{
	ASOperation* crntOpera = ASTransAddins::GetCrntOperation();
	if (!crntOpera || !crntOpera->GetAddin())
	{
		this->RemoveLayout(m_mainlayout);
		m_mainlayout->addWidget(m_Combox);
		m_GeneButn->hide();
		m_mainlayout->addStretch();
	}
	else
	{
		m_crntOperation = static_cast<ASAddinsBase*>(crntOpera->GetAddin());
		this->RemoveLayout(m_mainlayout);
		m_mainlayout->addWidget(m_Combox);
		m_mainlayout->addSpacing(10);
		int numofinput = m_crntOperation->GetNumOfInput();
		for (int index = 0; index < numofinput; index++)
		{
			ASAddinsInputBase* crntInput = m_crntOperation->GetInputAt(index);
			switch (crntInput->GetType())
			{
			case AddinInput_ArrayImage:
			{
				ASAddinsInputArrayImage* Input =
					static_cast<ASAddinsInputArrayImage*>(crntInput);
				if (Input->GetName() == "ParentImage")
				{
					ASAbstractData* f = crntOpera->getFather();
					if (f->isTypeInherited(DataType_ArrayImageData) == false)
					{
						qDebug("error: Bad data type. From ASSegmentationWidget.");
						return;
					}
					Input->SetImageData(static_cast<ASArrayImageData*>(f));
				}
				else
				{
					Input->RefreshCombo(DataType_ArrayImageData);
					QComboBox* combo = Input->GetCombobox();					
					QLabel* label = new QLabel(Input->GetName(), this);
					label->setMaximumWidth(120);
					QHBoxLayout* layout = new QHBoxLayout();
					layout->addWidget(label);
					layout->addWidget(combo);					
					m_mainlayout->addLayout(layout);
					combo->show(); // 在添加到layout后，否则会闪出一个窗口
				}
				break;
			}
			case AddinInput_PolyImage:
			{
				ASAddinsInputPolyImage* Input = static_cast<ASAddinsInputPolyImage*>(crntInput);
				if (Input->GetName() == "ParentPoly")
				{
					ASAbstractData* f = crntOpera->getFather();
					if (f->isTypeInherited(DataType_PolyImageData) == false)
					{
						qDebug("error: Bad data type. From ASSegmentationWidget.");
						return;
					}
					Input->SetPolyData(static_cast<ASPolyImageData*>(f));
				} 
				else
				{
					Input->RefreshCombo(DataType_PolyImageData);
					QComboBox* combo = Input->GetCombobox();
					QLabel* label = new QLabel(Input->GetName(), this);
					label->setMaximumWidth(120);
					QHBoxLayout* layout = new QHBoxLayout();
					layout->addWidget(label);
					layout->addWidget(combo);
					m_mainlayout->addLayout(layout);
					combo->show(); // 在添加到layout后，否则会闪出一个窗口
				}
				break;
			}
			case AddinInput_Mask:
			{
				ASAddinsInputMask* Input = static_cast<ASAddinsInputMask*>(crntInput);
				Input->RefreshCombo(DataType_ArrayImageData);
				QComboBox* combo = Input->GetCombobox();
				QLabel* label = new QLabel(Input->GetName(), this);
				label->setMaximumWidth(120);
				QHBoxLayout* layout = new QHBoxLayout();
				layout->addWidget(label);
				layout->addWidget(combo);
				m_mainlayout->addLayout(layout);
				combo->show(); // 在添加到layout后，否则会闪出一个窗口
				break;
			}
			case AddinInput_Landmark:
			{
				ASAddinsInputLandmark* Input =
					static_cast<ASAddinsInputLandmark*>(crntInput);
				Input->RefreshCombo(DataType_LandmarkData);
				int NumOfLandmark = Input->GetNumOfInputLandMark();
				for (int i = 0; i < NumOfLandmark; i++)
				{
					structInputLandMark* crntInputLandmark = Input->GetInputLandMarkAt(i);
					ASAddinsComboBox* combo = crntInputLandmark->ComboBox;
					QLabel* label = new QLabel(crntInputLandmark->Name, this);
					label->setMaximumWidth(120);
					QHBoxLayout* layout = new QHBoxLayout();
					layout->addWidget(label);
					layout->addWidget(combo);
					m_mainlayout->addLayout(layout);
					combo->show(); // 在添加到layout后，否则会闪出一个窗口
				}
				break;
			}
			case AddinInput_Contour:
			{
				ASAddinsInputContour* Input =
					static_cast<ASAddinsInputContour*>(crntInput);
				Input->RefreshCombo(DataType_ContourData);
				int NumOfSeed = Input->GetNumOfInputContour();
				for (int i = 0; i < NumOfSeed; i++)
				{
					structInputContour* crntInputContour = Input->GetInputContourAt(i);
					ASAddinsComboBox* combo = crntInputContour->ComboBox;
					QLabel* label = new QLabel(crntInputContour->Name, this);
					label->setMaximumWidth(120);
					QHBoxLayout* layout = new QHBoxLayout();
					layout->addWidget(label);
					layout->addWidget(combo);
					m_mainlayout->addLayout(layout);
					combo->show(); // 在添加到layout后，否则会闪出一个窗口
				}
				break;
			}
			case AddinInput_Seed:
			{
				ASAddinsInputSeed* Input =
					static_cast<ASAddinsInputSeed*>(crntInput);
				Input->RefreshCombo(DataType_SeedData);
				int NumOfSeed = Input->GetNumOfInputSeed();
				for (int i = 0; i < NumOfSeed; i++)
				{
					structInputSeed* crntInputSeed = Input->GetInputSeedAt(i);
					QComboBox* combo = crntInputSeed->ComboBox;
					QLabel* label = new QLabel(crntInputSeed->Name, this);
					label->setMaximumWidth(120);
					QHBoxLayout* layout = new QHBoxLayout();
					layout->addWidget(label);
					layout->addWidget(combo);
					m_mainlayout->addLayout(layout);
					combo->show(); // 在添加到layout后，否则会闪出一个窗口
				}
				break;
			}
			case AddinInput_ROI:
			{
				ASAddinsInputROI* Input =
					static_cast<ASAddinsInputROI*>(crntInput);
				Input->RefreshCombo(DataType_ROI);
				int NumOfROI = Input->GetNumOfInputROI();
				for (int i = 0; i < NumOfROI; i++)
				{
					structInputROI* crntInputROI = Input->GetInputROIAt(i);
					QComboBox* combo = crntInputROI->ComboBox;
					QLabel* label = new QLabel(crntInputROI->Name, this);
					label->setMaximumWidth(120);
					QHBoxLayout* layout = new QHBoxLayout();
					layout->addWidget(label);
					layout->addWidget(combo);
					m_mainlayout->addLayout(layout);
					combo->show(); // 在添加到layout后，否则会闪出一个窗口
				}
				break;
			}
			case AddinInput_Switch:
			{
				ASAddinsInputSwitch* Input = static_cast<ASAddinsInputSwitch*>(crntInput);
				QComboBox* ComboBox = Input->GetComboBox();
				QLabel* label = new QLabel(Input->GetName(), this);
				label->setMaximumWidth(120);
				QHBoxLayout* layout = new QHBoxLayout();
				layout->addWidget(label);
				layout->addWidget(ComboBox);
				m_mainlayout->addLayout(layout);
				ComboBox->show(); // 在添加到layout后，否则会闪出一个窗口
				break;
			}
			case AddinInput_Parameter:
			{
				ASAddinsInputPara* Input = static_cast<ASAddinsInputPara*>(crntInput);
				QDoubleSpinBox* DouSpin = Input->GetDouSpinBox();
				QLabel* label = new QLabel(Input->GetName(), this);
				label->setMaximumWidth(120);
				QHBoxLayout* layout = new QHBoxLayout();
				layout->addWidget(label);
				layout->addWidget(DouSpin);
				m_mainlayout->addLayout(layout);
				DouSpin->show(); // 在添加到layout后，否则会闪出一个窗口
				break;
			}
			default:
				break;
			}
		}
		int numofoutput = m_crntOperation->GetNumOfOutput();
		for (int index = 0; index < numofoutput; index++)
		{
			ASAddinsOutputBase* crntOutput = m_crntOperation->GetOutputAt(index);
			switch (crntOutput->GetType())
			{
			case AddinOutput_Parameter:
			{
				ASAddinsOutputParameter* Output = static_cast<ASAddinsOutputParameter*>(crntOutput);
				QLineEdit* LineEdit = Output->GetLineEdit();
				QLabel* label = new QLabel(Output->GetName(), this);
				label->setMaximumWidth(120);
				QHBoxLayout* layout = new QHBoxLayout();
				layout->addWidget(label);
				layout->addWidget(LineEdit);
				m_mainlayout->addLayout(layout);
				LineEdit->show(); // 在添加到layout后，否则会闪出一个窗口
				break;
			}
			default:
				break;
			}
		}
		if (m_crntOperation->GetDescription().size() > 0)
		{
			QByteArray ba = m_crntOperation->GetDescription().toLatin1();
			m_Description->setText(QString::fromLocal8Bit(ba));
			m_mainlayout->addWidget(m_Description);
			m_Description->adjustSize();
			//m_Description->setMinimumSize(m_Description->width(), m_Description->height());
			m_Description->setMinimumHeight(m_Description->height());
			m_Description->show();
		}		
		m_mainlayout->addStretch();
		m_GeneButn->show();
		m_mainlayout->addWidget(m_GeneButn);
		m_mainlayout->addWidget(m_BtnFinish);
	}
}

// combobox 
void ASOperationWidget::slotComboxChanged()
{
	if (m_GeneratingCombo)
	{
		return;
	}
	QString crntMethod = m_Combox->currentText();
	ASOperation* crntOpera = ASTransAddins::GetCrntOperation();
	if (!crntOpera)
	{
		return;
	}
	if (crntOpera->GetAddin() && crntOpera->GetAddin()->GetName() == crntMethod)
	{
		return;
	}
	if (m_Combox->currentIndex() == 0)
	{
		crntOpera->ClearMethod();
	}
	else
	{
		// 选择与当前不同
		int NumOfAddins = ASFModelAddins::GetNumOfAddins();
		for (int index = 0; index < NumOfAddins; index++)
		{
			structAddin* addin = ASFModelAddins::GetAddinAt(index);
			if (addin->name == crntMethod && addin->type == crntOpera->GetAddinType())
			{
				crntOpera->InitMethod(addin->name, addin->path);
				break;
			}
		}
	}
	this->RefreshCrntOperationUI();
}
// PushButton
void ASOperationWidget::slotGenerate()
{
	if (m_crntOperation)
	{
		m_crntOperation->Generate();
	}
}
// 刷新，如删除数据后
void ASOperationWidget::slotRefreshWidget()
{
	m_Combox->setCurrentIndex(0);
}
// 更换当前分割
void ASOperationWidget::slotCrntOperationChanged()
{
	ASOperation* crntOpera = ASTransAddins::GetCrntOperation();
	if (!crntOpera)
	{
		return;
	}
	this->ComboInit(crntOpera->GetAddinType());
	if (crntOpera->GetAddin())
	{
		int index = m_Combox->findText(crntOpera->GetAddin()->GetName());
		if (index < 1)
		{
			m_Combox->setCurrentIndex(0);
		}
		else
		{
			m_Combox->setCurrentIndex(index);
		}
	}
	else
	{
		m_Combox->setCurrentIndex(0);
	}
	this->RefreshCrntOperationUI();
}
// 数据列表变化，刷新combobox
void ASOperationWidget::slotUpdateDataNodeLists(int Type)
{
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_Operation);
		item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_Operation))
	{
		ASOperation* crntOpera = static_cast<ASOperation*>(item);
		if (crntOpera->GetAddin())
		{
			int numofinput = crntOpera->GetAddin()->GetNumOfInput();
			for (int index = 0; index < numofinput; index++)
			{
				ASAddinsInputBase* crntInput = crntOpera->GetAddin()->GetInputAt(index);
				crntInput->RefreshCombo(Type);
			}
		}
	}
}
// 设置当前方法
void ASOperationWidget::slotSetCurrentMethod(QString s)
{
	int index = m_Combox->findText(s);
	if (index > 0)
	{
		m_Combox->setCurrentText(s);
		this->m_BtnFinish->show();
		m_FlagFromAddin = true;
	}
}
// Finish
void ASOperationWidget::slotFinish()
{
	ASTransAddins::NewMaskFinish();
}