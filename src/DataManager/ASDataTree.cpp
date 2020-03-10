#include "ASDataTree.h"
#include "ASAbstractData.h"
#include "ASArrayImageData.h"
#include "ASPolyImageData.h"
#include "ASDICOMData.h"
#include "ASDICOMRTData.h"
#include "ASSeedData.h"
#include "ASDataStructureWalker.h"
#include "ASTransAddDeleteData.h"

ASAbstractData* ASDataTree::ms_RootDataNote = nullptr;
ASDataTree* ASDataTree::m_DataTree = nullptr;

ASDataTree::ASDataTree(QObject *parent)
	: QObject(parent)
{
	m_DataTree = this;
	ms_RootDataNote = new ASAbstractData();
	ms_RootDataNote->setFather(nullptr);
	ASAbstractData::setRootDataNode(ms_RootDataNote);
	m_DataStructureWalker = new ASDataStructureWalker(ms_RootDataNote);

	connect(ASTransAddDeleteData::GetSelfPointer(), SIGNAL(signalRemoveNode(ASAbstractData*)), this, SLOT(slotRemoveNode(ASAbstractData*)));
}

ASDataTree::~ASDataTree()
{

}

// 静态成员函数
// 静态函数获取根节点，用于得到整个树，在数据结构树显示时需要用到
ASAbstractData* ASDataTree::getRootDataNote()
{
	return ms_RootDataNote;
}
// 根据当前树的形状重新计算面数据层级
void ASDataTree::RefreshArrayDataLayer()
{
	m_DataTree->m_CrntLayer = 0;
	// 对数据树先跟遍历，使用递归方法
	m_DataTree->FollowUpTraversalToRefreshLayer(ms_RootDataNote);
}
// 新建节点
void ASDataTree::NewDataNode(ASAbstractData* DataNode, ASAbstractData* Father)
{
	ASAbstractData* FatherNode = Father;
	if (FatherNode == nullptr)
	{
		FatherNode = ms_RootDataNote;
	}
	FatherNode->addChild(DataNode);
	DataNode->setFather(FatherNode);
}

// 递归函数，先跟遍历数据树，重新计算体数据层级
void ASDataTree::FollowUpTraversalToRefreshLayer(ASAbstractData* DataNode)
{
	if (DataNode->isTypeInherited(DataType_ArrayImageData))
	{
		((ASArrayImageData*)DataNode)->setLayer(m_CrntLayer);
		m_CrntLayer++;
	}
	// 得到孩子节点
	QList<ASAbstractData*>* ChildrenNode = DataNode->getChild();
	for (int crntChild = ChildrenNode->size() - 1; crntChild >= 0; crntChild--)
	{
		FollowUpTraversalToRefreshLayer(ChildrenNode->at(crntChild));
	}
}

// 删除节点
int ASDataTree::slotRemoveNode(ASAbstractData* DataNode)
{
	DataNode->getFather()->removeChild(DataNode);
	DataNode->DeleteNode();	//delete data;
	return 0;
}