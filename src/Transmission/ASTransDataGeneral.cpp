#include "ASTransDataGeneral.h"
#include "ASArrayImageData.h"
#include "ASTransROI.h"
#include "ASAnnotation.h"
#include "ASTransAddDeleteData.h"
#include "ASDataTree.h"

ASTransDataGeneral* ASTransDataGeneral::ms_SelfPointer = nullptr;
ASArrayImageData* ASTransDataGeneral::m_GeneratingData = nullptr;

ASTransDataGeneral::ASTransDataGeneral(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_SelfPointer = this;
}

ASTransDataGeneral::~ASTransDataGeneral()
{

}

// 唯一对象
ASTransDataGeneral* ASTransDataGeneral::GetSelfPointer()
{
	return ms_SelfPointer;
}
// 设置某体数据为当前数据
void ASTransDataGeneral::SetGeneratingData(ASAbstractData* crntData)
{
	m_GeneratingData = static_cast<ASArrayImageData*>(crntData);
	emit ms_SelfPointer->signalRefreshCrntData();
	ASTransDataGeneral::ResetGeneratingData();
}
ASAbstractData* ASTransDataGeneral::GetGeneratingData()
{
	return m_GeneratingData;
}
void ASTransDataGeneral::ResetGeneratingData()
{
	ASTransROI::RefreshROIList();
}

// 新建标记数据
ASAnnotation* ASTransDataGeneral::NewAnnotationData(ASAbstractData* parent)
{
	ASAnnotation* crntDataNode = new ASAnnotation();
	crntDataNode->setName(tr("Annotation Data"));
	crntDataNode->setVoidNode(true);
	crntDataNode->setShown(true);
	ASDataTree::NewDataNode(crntDataNode, parent);
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_Annotation);
	return crntDataNode;
}
