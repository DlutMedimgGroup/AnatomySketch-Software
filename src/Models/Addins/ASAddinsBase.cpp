#include "ASAddinsBase.h"
#include "ASAddinsInputBase.h"
#include "ASAddinsOutputBase.h"
#include "ASAddinsInputArrayImage.h"
#include "ASAddinsInputMask.h"
#include "ASAddinsInputLandmark.h"
#include "ASAddinsInputContour.h"
#include "ASAddinsInputSeed.h"
#include "ASAddinsInputROI.h"
#include "ASAddinsInputSwitch.h"
#include "ASAddinsInputPara.h"
#include "ASAddinsInputPolyImage.h"
#include "ASAddinsOutputArrayImage.h"
#include "ASAddinsOutputPolyImage.h"
#include "ASAddinsOutputParameter.h"
#include "ASMainWidget.h"
#include "ASThreadAddins.h"
#include <QStringList>
#include <QMessageBox>
#include <QObject>
#include <QChar>

ASAddinsBase::ASAddinsBase(ASOperation* host, QObject *parent)
	: QObject(parent)
{
	//m_Type = AddinType_Base;
	m_listInputs = new QList<ASAddinsInputBase *>;
	m_listOutputs = new QList<ASAddinsOutputBase *>;
	m_listArgv = new QList<structArgEle *>;
	m_listPYEntrence = new QList<structArgEle *>;
	m_listPYExport = new QList<structArgEle *>;
	m_Host = host;
}

ASAddinsBase::~ASAddinsBase()
{
	for (int i = 0; i < m_listInputs->size(); i++)
	{
		delete m_listInputs->at(i);
	}
	m_listInputs->clear();
	delete m_listInputs;
	for (int i = 0; i < m_listOutputs->size(); i++)
	{
		delete m_listOutputs->at(i);
	}
	m_listOutputs->clear();
	delete m_listOutputs;
	for (int i = 0; i < m_listArgv->size(); i++)
	{
		delete m_listArgv->at(i);
	}
	m_listArgv->clear();
	delete m_listArgv;
}

// Type
int ASAddinsBase::GetType()
{
	return m_Type;
}
// Name
void ASAddinsBase::SetName(const QString c_name)
{
	m_Name = c_name;
}
QString ASAddinsBase::GetName()
{
	return m_Name;
}
// Model type
void ASAddinsBase::SetAddinType(const int c_type)
{
	this->m_AddinType = c_type;
}
int ASAddinsBase::GetAddinType()
{
	return this->m_AddinType;
}
// Interface type
void ASAddinsBase::SetInterfaceType(const int c_type)
{
	m_InterfaceType = c_type;
}
int ASAddinsBase::GetInterfaceType()
{
	return m_InterfaceType;
}
// Host
ASOperation* ASAddinsBase::GetHost()
{
	return m_Host;
}
// DLL
void ASAddinsBase::SetDLL(const QString c_name)
{
	m_DLL = c_name;
}
QString ASAddinsBase::GetDLL()
{
	return m_DLL;
}
// Python
void ASAddinsBase::SetPython(const QString c_name)
{
	m_Python = c_name;
}
QString ASAddinsBase::GetPython()
{
	return m_Python;
}
// Description
void ASAddinsBase::SetDescription(const QString c_description)
{
	m_Description = c_description;
}
QString ASAddinsBase::GetDescription()
{
	return m_Description;
}
// Input
void ASAddinsBase::AddInput(const int c_type, const QStringList c_value)
{
	ASAddinsInputBase* crntInput;
	switch (c_type)
	{
	case AddinInput_ArrayImage:
		crntInput = new ASAddinsInputArrayImage(this);		
		break;
	case AddinInput_Mask:
		crntInput = new ASAddinsInputMask(this);
		break;
	case AddinInput_Landmark:
		crntInput = new ASAddinsInputLandmark(this);
		break;
	case AddinInput_Contour:
		crntInput = new ASAddinsInputContour(this);
		break;
	case AddinInput_Seed:
		crntInput = new ASAddinsInputSeed(this);
		break;
	case AddinInput_ROI:
		crntInput = new ASAddinsInputROI(this);
		break;
	case AddinInput_Switch:
		crntInput = new ASAddinsInputSwitch(this);
		break;
	case AddinInput_Parameter:
		crntInput = new ASAddinsInputPara(this);
		break;
	case AddinInput_PolyImage:
		crntInput = new ASAddinsInputPolyImage(this);
		break;
	default:
		break;
	}
	crntInput->SetValue(c_value);
	m_listInputs->append(crntInput);
}
void ASAddinsBase::AddInput(const QString c_typename, const QStringList c_value)
{
	if (c_typename == "ArrayImage")
	{
		for (int item = 0; item < c_value.size(); item++)
		{
			QStringList sl;
			sl << c_value.at(item);
			this->AddInput(AddinInput_ArrayImage, sl);
		}		
	}
	if (c_typename == "Mask")
	{
		for (int item = 0; item < c_value.size(); item++)
		{
			QStringList sl;
			sl << c_value.at(item);
			this->AddInput(AddinInput_Mask, sl);
		}
	}
	else if (c_typename == "Landmark")
	{
		this->AddInput(AddinInput_Landmark, c_value);
	}
	else if (c_typename == "Contour")
	{
		this->AddInput(AddinInput_Contour, c_value);
	}
	else if (c_typename == "Seed")
	{
		this->AddInput(AddinInput_Seed, c_value);
	}
	else if (c_typename == "ROI")
	{
		this->AddInput(AddinInput_ROI, c_value);
	}
	else if (c_typename == "Switch")
	{
		for (int item = 0; item < c_value.size(); item++)
		{
			QStringList sl;
			sl << c_value.at(item);
			this->AddInput(AddinInput_Switch, sl);
		}
	}
	else if (c_typename == "Parameter")
	{
		for (int item = 0; item < c_value.size(); item++)
		{
			QStringList sl;
			sl << c_value.at(item);
			this->AddInput(AddinInput_Parameter, sl);
		}
	}
	else if (c_typename == "PolyImage")
	{
		this->AddInput(AddinInput_PolyImage, c_value);
	}
}
int ASAddinsBase::GetNumOfInput()
{
	return m_listInputs->size();
}
ASAddinsInputBase* ASAddinsBase::GetInputAt(const int c_index)
{
	return m_listInputs->at(c_index);
}
// Output
void ASAddinsBase::AddOutput(const int c_type, const QString c_name)
{
	ASAddinsOutputBase* crntOutput;
	switch (c_type)
	{
	case AddinOutput_ArrayImage:
		crntOutput = new ASAddinsOutputArrayImage(this);
		break;
	case AddinOutput_PolyImage:
		crntOutput = new ASAddinsOutputPolyImage(this);
		break;
	case AddinOutput_Parameter:
		crntOutput = new ASAddinsOutputParameter(this);
		break;
	default:
		break;
	}
	crntOutput->SetName(c_name);
	m_listOutputs->append(crntOutput);
}
void ASAddinsBase::AddOutput(const QString c_typename, const QString c_name)
{
	if (c_typename == "ArrayImage")
	{
		this->AddOutput(AddinOutput_ArrayImage, c_name);
	}
	else if (c_typename == "PolyImage")
	{
		this->AddOutput(AddinOutput_PolyImage, c_name);
	}
	else if (c_typename == "Parameter")
	{
		this->AddOutput(AddinOutput_Parameter, c_name);
	}
}
int ASAddinsBase::GetNumOfOutput()
{
	return m_listOutputs->size();
}
ASAddinsOutputBase* ASAddinsBase::GetOutputAt(const int c_index)
{
	return m_listOutputs->at(c_index);
}
// interface
void ASAddinsBase::AppendInterface(const QString c_s, const int c_type)
{
	QStringList slist = c_s.split(".");
	structArgEle* crntEle = new structArgEle;
	for (int i = 0; i < m_listInputs->size(); i++)
	{
		if (slist.at(0) == m_listInputs->at(i)->GetName())
		{
			crntEle->IOEle = m_listInputs->at(i);
			if (slist.size() > 1)
			{
				crntEle->Discri = slist.at(1);
			}
			else
			{
				crntEle->Discri = "";
			}
			switch (c_type)
			{
			case 0: // argv
				m_listArgv->append(crntEle);
				break;
			case 1: // PYEntrence
				m_listPYEntrence->append(crntEle);
				break;
			case 2: // PYExport
				m_listPYExport->append(crntEle);
				break;
			default:
				break;
			}			
			return;
		}
	}
	for (int i = 0; i < m_listOutputs->size(); i++)
	{
		if (slist.at(0) == m_listOutputs->at(i)->GetName())
		{
			crntEle->IOEle = m_listOutputs->at(i);
			if (slist.size() > 1)
			{
				crntEle->Discri = slist.at(1);
			}
			else
			{
				crntEle->Discri = "";
			}
			switch (c_type)
			{
			case 0: // argv
				m_listArgv->append(crntEle);
				break;
			case 1: // PYEntrence
				m_listPYEntrence->append(crntEle);
				break;
			case 2: // PYExport
				m_listPYExport->append(crntEle);
				break;
			default:
				break;
			}
			return;
		}
	}
}
// 运算
void ASAddinsBase::Generate()
{
	if (m_InterfaceType == InterfaceType_DLL)
	{
		m_MessageBox = new QMessageBox(QMessageBox::NoIcon, "Anatomy Sketch",
			tr(u8"计算中，请稍等。"), QMessageBox::Cancel, NULL);
		m_MessageBox->setModal(true);
		connect(m_MessageBox, SIGNAL(buttonClicked(QAbstractButton*)),
			this, SLOT(slotCancel(QAbstractButton*)));
		m_MessageBox->show();
		int argc = m_listArgv->size();
		void** argv = new void*[argc];
		for (int index = 0; index < argc; index++)
		{
			structArgEle* crntEle = m_listArgv->at(index);
			argv[index] = crntEle->IOEle->GetVoidPoint(crntEle->Discri);

			if (argv[index] == nullptr && crntEle->IOEle->isInput())
			{
				if (crntEle->IOEle->GetType() == AddinInput_ROI)
				{
					continue;
				}
				QString s = "Bad parameter " + crntEle->IOEle->GetName();
				QMessageBox::information(ASMainWidget::GetMainWidget(), "Error",
					s, QMessageBox::Ok, QMessageBox::Ok);
				disconnect(m_MessageBox, SIGNAL(buttonClicked(QAbstractButton*)),
					this, SLOT(slotCancel(QAbstractButton*)));
				delete m_MessageBox;
				m_MessageBox = nullptr;
				return;
			}
		}

		// 开线程计算
		if (m_ThreadAddin != nullptr)
		{
			return;
		}
		m_ThreadAddin = new ASThreadAddins();
		m_ThreadAddin->SetArgc(argc);
		m_ThreadAddin->SetArgv(argv);
		m_ThreadAddin->SetDLL(m_DLL);
		connect(m_ThreadAddin, SIGNAL(signalFinished(float)), this, SLOT(slotThreadFinished(float)));
		m_ThreadAddin->start();
	}
	else if (m_InterfaceType == InterfaceType_Python)
	{
		m_MessageBox = new QMessageBox(QMessageBox::NoIcon, "Anatomy Sketch",
			tr("Calculating..."), QMessageBox::Cancel, NULL);
		m_MessageBox->setModal(true);
		connect(m_MessageBox, SIGNAL(buttonClicked(QAbstractButton*)),
			this, SLOT(slotCancel(QAbstractButton*)));
		m_MessageBox->show();
		int szEntrence = m_listPYEntrence->size();	
		for (int index = 0; index < szEntrence; index++)
		{			
			structArgEle* crntEle = m_listPYEntrence->at(index);
			if (crntEle->IOEle->PythonInputGenerate() < 0)
			{
				QString s = "Bad parameter " + crntEle->IOEle->GetName();
				QMessageBox::information(ASMainWidget::GetMainWidget(), "Error",
					s, QMessageBox::Ok, QMessageBox::Ok);
				disconnect(m_MessageBox, SIGNAL(buttonClicked(QAbstractButton*)),
					this, SLOT(slotCancel(QAbstractButton*)));
				delete m_MessageBox;
				m_MessageBox = nullptr;
				return;
			}
		}
		// 开线程计算
		if (m_ThreadAddin != nullptr)
		{
			return;
		}
		m_ThreadAddin = new ASThreadAddins();
		m_ThreadAddin->SetPython(m_Python);		
		connect(m_ThreadAddin, SIGNAL(signalFinished(float)), this, SLOT(slotThreadFinished(float)));
		// save a pointer to the main PyThreadState object
		m_ThreadAddin->start();
	}
}
void ASAddinsBase::slotThreadFinished(float RValue)
{
	int inferencetype = m_ThreadAddin->GetInferenceType();
	if (RValue == 1.0)
	{		
		if (inferencetype == 0)
		{
			// DLL
			int sz = m_listOutputs->size();
			for (int crntOut = 0; crntOut < sz; crntOut++)
			{
				m_listOutputs->at(crntOut)->GenerateFinished(InterfaceType_DLL);
			}
		}
		else if (inferencetype == 1)
		{
			// shut down the interpreter
			int szExport = m_listPYExport->size();
			int sz = m_listOutputs->size();
			for (int crntOut = 0; crntOut < sz; crntOut++)
			{
				m_listOutputs->at(crntOut)->GenerateFinished(InterfaceType_Python);
			}
		}
	}
	else if (RValue == -1.0)
	{
		qDebug("error: Bad argc.");
	}
	if (inferencetype == 0)
	{
		// DLL
		for (int index = 0; index < m_listArgv->size(); index++)
		{
			structArgEle* crntEle = m_listArgv->at(index);
			crntEle->IOEle->FreeMemory();
		}
	}
	else if (inferencetype == 1)
	{
		// python		
		for (int index = 0; index < m_listPYEntrence->size(); index++)
		{
			structArgEle* crntEle = m_listPYEntrence->at(index);
			crntEle->IOEle->FreeMemory();
		}
		for (int index = 0; index < m_listPYExport->size(); index++)
		{
			structArgEle* crntEle = m_listPYExport->at(index);
			crntEle->IOEle->FreeMemory();
		}
	}

	delete m_ThreadAddin;
	m_ThreadAddin = nullptr;
	disconnect(m_MessageBox, SIGNAL(buttonClicked(QAbstractButton*)),
		this, SLOT(slotCancel(QAbstractButton*)));
	delete m_MessageBox;
	m_MessageBox = nullptr;
}
void ASAddinsBase::slotCancel(QAbstractButton* b)
{
	if (m_ThreadAddin)
	{
		m_ThreadAddin->terminate();
		m_ThreadAddin->wait();
	}
}