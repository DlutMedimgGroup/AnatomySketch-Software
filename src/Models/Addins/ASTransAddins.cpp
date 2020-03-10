#include "ASTransAddins.h"
#include "ASArrayImageData.h"
#include "ASAbstractData.h"
#include "ASOperation.h"
#include "ASDataTree.h"
#include "ASTransAddDeleteData.h"
#include "ASTransSidebarGeneral.h"
#include "ASFModelAddins.h"
#include "ASAddinsBase.h"
#include <QInputDialog>

ASTransAddins* ASTransAddins::ms_SelfPointer = nullptr;

ASTransAddins::ASTransAddins(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_SelfPointer = this;
	m_Addins = new ASFModelAddins();
}

ASTransAddins::~ASTransAddins()
{

}
// 唯一对象
ASTransAddins* ASTransAddins::GetSelfPointer()
{
	return ms_SelfPointer;
}

// 新建插件操作
void ASTransAddins::NewAddinOperation(ASAbstractData* pParent, const int c_Type)
{
	static int s_num_Img = 0;
	static int s_num_Mesh = 0;
	static int s_num_Seg = 0;
	// 1 新建分割
	bool isOK;
	QString s1, s2;
	ASOperation* crntOpera = new ASOperation();
	switch (c_Type)
	{
	case AddinType_ImgProcess:
		s1 = "ImgProcess_" + QString::number(s_num_Img++);
		s2 = tr("New Image Processing");
		crntOpera->SetAddinType(AddinType_ImgProcess);
		break;
	case AddinType_MeshProcess:
		s1 = "MeshProcess_" + QString::number(s_num_Mesh++);
		s2 = tr("New Mesh Processing");
		crntOpera->SetAddinType(AddinType_MeshProcess);
		break;
	case AddinType_Segmentation:
		s1 = "Segmentation_" + QString::number(s_num_Seg++);
		s2 = tr("New Segmentation");
		crntOpera->SetAddinType(AddinType_Segmentation);
		break;
	default:
		break;
	}
	QString text = QInputDialog::getText(NULL, s2, tr("Please enter name: "),
		QLineEdit::Normal, s1, &isOK);
	if (!isOK)
	{
		delete crntOpera;
		return;
	}
	crntOpera->setName(text);
	crntOpera->setVoidNode(true);
	crntOpera->setShown(true);
	ASDataTree::NewDataNode(crntOpera, pParent);
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_Operation);
	ASTransAddins::SetCrntOperation(crntOpera);
	ASTransSidebarGeneral::SetSidebarBelowWidget(SIDEBAR_Operation, true);
}

// 当前分割
void ASTransAddins::SetCrntOperation(ASAbstractData* opera)
{
	if (opera == nullptr)
	{
		ms_SelfPointer->m_Addins->SetCrntOperation(nullptr);
		ASTransSidebarGeneral::SetSidebarBelowWidget(SIDEBAR_Operation, false);
		emit ms_SelfPointer->signalRefreshWidget();
	}
	else if (opera->getDataType() == DataType_Operation)
	{
		ms_SelfPointer->m_Addins->SetCrntOperation(static_cast<ASOperation*>(opera));
	}
	emit ms_SelfPointer->signalCurrentSegChanged();
}
ASOperation* ASTransAddins::GetCrntOperation()
{
	return ms_SelfPointer->m_Addins->GetCrntOperation();
}
// 有数据被移除
void ASTransAddins::DataRemoved(ASAbstractData* opera)
{
	if (opera->isTypeInherited(DataType_Operation))
	{
		ASOperation* copera = static_cast<ASOperation*>(opera);
		if (copera = ms_SelfPointer->m_Addins->GetCrntOperation())
		{
			SetCrntOperation(nullptr);
		}
	}
}
// 新建遮罩
void ASTransAddins::NewMask(ASAbstractData* pParent, QString name, Fun1 f)
{
	ms_SelfPointer->m_f = f;
	ASOperation* crntOpera = new ASOperation();
	crntOpera->SetAddinType(AddinType_Segmentation);
	crntOpera->setName(name);
	crntOpera->setVoidNode(true);
	crntOpera->setShown(true);
	ASDataTree::NewDataNode(crntOpera, pParent);
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_Operation);
	ms_SelfPointer->m_NewMaskGenerating = ASTransAddins::GetCrntOperation();
	ASTransAddins::SetCrntOperation(crntOpera);
	emit ms_SelfPointer->signalSetCurrentMethod("Threshold");
	ASTransSidebarGeneral::SetSidebarBelowWidget(SIDEBAR_Operation, true);
}
void ASTransAddins::NewMaskFinish()
{
	if (ms_SelfPointer->m_NewMaskGenerating)
	{
		ASTransAddins::SetCrntOperation(ms_SelfPointer->m_NewMaskGenerating);
		ms_SelfPointer->m_NewMaskGenerating = nullptr;
		ms_SelfPointer->m_f();
	}
}