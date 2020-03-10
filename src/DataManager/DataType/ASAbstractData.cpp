#include "ASAbstractData.h"
#include <QMutexLocker>
//#include "common.h"

ASAbstractData* ASAbstractData::ms_RootDataNode = nullptr;
int ASAbstractData::ms_nNumberOfData = -1;		//根节点建立时调用构造函数会使数字加1
int ASAbstractData::ms_nNumberOfDataShown = 0;

ASAbstractData::ASAbstractData()
{
	m_pChildren = new QList<ASAbstractData*>;
	m_nDataNoteID = ASAbstractData::getNewDataNodeID();
	ms_nNumberOfData += 1;

	m_Datatype = DataType_AbstractData; // 构造函数执行顺序为先执行基类的构造函数后执行派生类的构造函数，所以该类型会被覆盖。

	m_Mutex = new QMutex();
}

ASAbstractData::~ASAbstractData()
{
	ms_nNumberOfData -= 1;
	int numOfChildren = m_pChildren->size();
	for (int ncrntChild = 0; ncrntChild < numOfChildren; ncrntChild++)
	{
		ASAbstractData* crntDataNode = m_pChildren->at(ncrntChild);
		this->removeChild(crntDataNode);
		delete crntDataNode;
	}
	m_pChildren->clear();
	delete m_pChildren;
}

// 调用此函数来释放节点
void ASAbstractData::DeleteNode()
{
	QMutexLocker Locker(m_Mutex);
	delete this;
}
// 检查子节点是否为只读
bool ASAbstractData::isChildrenBeingProcess()
{
	int numOfChildren = m_pChildren->size();
	for (int ncrntChild = 0; ncrntChild < numOfChildren; ncrntChild++)
	{
		if (m_pChildren->at(ncrntChild)->isBeingProcess())
		{
			return true;
		}
	}
	return false;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASAbstractData::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_AbstractData)
	{
		return true;
	}
	return false;
}
bool ASAbstractData::isTypeAbsolute(const int c_Type)
{
	QMutexLocker Locker(m_Mutex);
	if (c_Type == DataType_AbstractData)
	{
		return true;
	} 
	return false;
}
bool ASAbstractData::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_AbstractData)
	{
		return true;
	}
	return false;
}

// 数据结构的操作
void ASAbstractData::addChild(ASAbstractData* child)
{
	QMutexLocker Locker(m_Mutex);
	//m_pChildren->append(child);
	m_pChildren->insert(0, child); // 插入到最前面
}
void ASAbstractData::addChild(ASAbstractData* child, int insertPos)
{
	QMutexLocker Locker(m_Mutex);
	m_pChildren->insert(insertPos, child);
}
// 删除子节点
void ASAbstractData::removeChild(ASAbstractData* child)
{
	QMutexLocker Locker(m_Mutex);
	m_pChildren->removeOne(child);
}
void ASAbstractData::removeChild(int removePos)
{
	QMutexLocker Locker(m_Mutex);
	m_pChildren->removeAt(removePos);
}
// 得到子节点，父节点
QList<ASAbstractData*>* ASAbstractData::getChild()
{
	QMutexLocker Locker(m_Mutex);
	return m_pChildren;
}
// 得到第一个某类型的子节点
ASAbstractData* ASAbstractData::getChild(const int c_Type)
{
	QMutexLocker Locker(m_Mutex);
	int numOfChildren = m_pChildren->size();
	for (int i = 0; i < numOfChildren; i++)
	{
		ASAbstractData* crntChild = m_pChildren->at(i);
		if (crntChild->isTypeInherited(c_Type))
		{
			return crntChild;
		}
	}
	return nullptr;
}
ASAbstractData* ASAbstractData::getFather()
{
	QMutexLocker Locker(m_Mutex);
	return m_pFather;
}
void ASAbstractData::setFather(ASAbstractData* father)
{
	QMutexLocker Locker(m_Mutex);
	m_pFather = father;
}
// 得到根节点，即第一级的数据节点
ASAbstractData* ASAbstractData::getRoot()
{
	if (this->getFather()->getDataNodeID() < 0)
	{
		return this;
	}
	else
	{
		return this->getFather()->getRoot();
	}
}
// 是否为空节点
bool ASAbstractData::isVoidNode()
{
	QMutexLocker Locker(m_Mutex);
	return this->m_isVoidNode;
}
void ASAbstractData::setVoidNode(const bool c_isVoidNode)
{
	QMutexLocker Locker(m_Mutex);
	this->m_isVoidNode = c_isVoidNode;
}

// 数据类型
int ASAbstractData::getDataType()
{
	QMutexLocker Locker(m_Mutex);
	return m_Datatype;
}
// 第几个孩子
int ASAbstractData::getOrderInBrother(ASAbstractData* child)
{
	QMutexLocker Locker(m_Mutex);
	return m_pChildren->indexOf(child);
}
// 设置是否正在被运算
void ASAbstractData::setBeingProcess(const bool isBeingProcess)
{
	QMutexLocker Locker(m_Mutex);
	m_isBeingProcess = isBeingProcess;
}
bool ASAbstractData::isBeingProcess()
{
	QMutexLocker Locker(m_Mutex);
	return m_isBeingProcess;
}
// 设置是否显示
void ASAbstractData::setShown(const bool shown)
{
	QMutexLocker Locker(m_Mutex);
	if (m_isShown == shown)
	{
		return;
	}
	m_isShown = shown;
	if (m_isShown)
	{
		ms_nNumberOfDataShown += 1;
	}
	else
	{
		ms_nNumberOfDataShown -= 1;
	}
}
bool ASAbstractData::isShown()
{
	return m_isShown;
}
// 得到该节点的ID
int ASAbstractData::getDataNodeID()
{
	QMutexLocker Locker(m_Mutex);
	return m_nDataNoteID;
}
// Name
void ASAbstractData::setName(QString name)
{
	QMutexLocker Locker(m_Mutex);
	m_Name = name;
}
QString ASAbstractData::getName()
{
	QMutexLocker Locker(m_Mutex);
	return m_Name;
}
void ASAbstractData::setFileName(QString filename)
{
	QMutexLocker Locker(m_Mutex);
	m_FileName = filename;
}
QString ASAbstractData::getFileName()
{
	QMutexLocker Locker(m_Mutex);
	return m_FileName;
}
// 数据范围
double* ASAbstractData::getDataExtent()
{
	QMutexLocker Locker(m_Mutex);
	return m_adDataExtent;
}
int ASAbstractData::getNumberOfFrame()
{
	QMutexLocker Locker(m_Mutex);
	return m_NumberOfFrames;
}
// 多线程
QMutex* ASAbstractData::GetMutex()
{
	return m_Mutex;
}
// 静态成员函数
// 由ID返回节点
ASAbstractData* ASAbstractData::getDataNodeFromID(int id)
{
	if (ms_RootDataNode == nullptr)
	{
		return nullptr;
	}
	return ms_RootDataNode->findID(id);
}
// 得到当前已经打开的文件数量
int ASAbstractData::getNumberOfData()
{
	return ASAbstractData::ms_nNumberOfData;
}
// 得到当前已经显示的文件数量
int ASAbstractData::getNumberOfDataShown()
{
	return ms_nNumberOfDataShown;
}

// 查找具有该ID的节点，该函数为迭代函数
ASAbstractData* ASAbstractData::findID(int nID)
{
	QMutexLocker Locker(m_Mutex);
	if (nID == m_nDataNoteID)
	{
		return this;
	}
	int numOfChildren = m_pChildren->size();
	for (int ncrntChild = 0; ncrntChild < numOfChildren; ncrntChild++)
	{
		ASAbstractData* p = m_pChildren->at(ncrntChild)->findID(nID);
		if (p != nullptr)
		{
			return p;
		}
	}
	return nullptr;
}
// 该函数只可以被ASDataStructureWalker调用，用于数据的遍历。实现的功能只是添加该节点的子节点到给定的队列中
void ASAbstractData::Traverse(std::queue<ASAbstractData*>* queue)
{
	QMutexLocker Locker(m_Mutex);
	for (int crntChild = 0; crntChild < m_pChildren->size(); crntChild++)
	{
		queue->push(m_pChildren->at(crntChild));
	}
}

//静态成员和静态成员函数
// 设定根节点
void ASAbstractData::setRootDataNode(ASAbstractData* node)
{
	ASAbstractData::ms_RootDataNode = node;
}
// 得到新的ID，与先有ID均不重复。
int ASAbstractData::getNewDataNodeID()
{
	int newID = 0;
	if (ms_RootDataNode == nullptr)
	{
		//此时根节点还没有实例，所以该节点就是根节点，其ID为-1
		return -1;
	}
	while (ms_RootDataNode->findID(newID) != nullptr)
	{
		newID++;
	}
	return newID;
}