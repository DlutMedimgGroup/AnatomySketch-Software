#pragma once

//#include "common.h"
#include <QObject>
#include <QList>
#include "ASAbstractData.h"

class ASArrayImageData;
class ASPolyImageData;
class ASDataStructureWalker;

class ASDataTree : public QObject
{
	Q_OBJECT

public:
	ASDataTree(QObject *parent = 0);
	~ASDataTree();

	//// 静态成员函数

	// 获取根节点，用于得到整个树，在数据结构树显示时需要用到
	static ASAbstractData* getRootDataNote();
	// 根据当前树的形状重新计算面数据层级
	static void RefreshArrayDataLayer();
	// 新建节点
	static void NewDataNode(ASAbstractData* DataNode, ASAbstractData* Father);

private:

	// 该类唯一对象
	static ASDataTree* m_DataTree;
	// 软件中所打开的数据均储存在一个树的数据结构中，根节点的每一个子树代表一个一级的数据，或称为工程（project）。根节点本身并不表示任何数据。
	static ASAbstractData* ms_RootDataNote;
	// 需要实例一个类对遍历器进行初始化
	ASDataStructureWalker* m_DataStructureWalker; 

	// 递归函数，先跟遍历数据树，重新计算体数据层级
	void FollowUpTraversalToRefreshLayer(ASAbstractData* DataNode);
	int m_CrntLayer = 0; // 辅助刷新体数据层级
	

private slots:

	// 删除节点
	int slotRemoveNode(ASAbstractData* DataNode);

};
