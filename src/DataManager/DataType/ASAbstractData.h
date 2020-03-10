#pragma once

//#include "common.h"
#include <QString>
#include <QList>
#include <queue>
#include <QMutex>

//数据类型
#define DataType_AbstractData		0
#define DataType_ArrayImageData		1
#define DataType_ContourData		2
#define DataType_DICOMData			3
#define DataType_DICOMRTData		4
#define DataType_ImageData			5
#define DataType_MarkData			6
#define DataType_PolyImageData		7
#define DataType_SeedData			8
#define DataType_LandmarkData		9
#define DataType_Operation			10
#define DataType_ROI				11
#define DataType_Annotation			12

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

class ASAbstractData
{
public:
	ASAbstractData();
	~ASAbstractData();
	virtual void DeleteNode();	// 调用此函数来释放节点
	bool isChildrenBeingProcess(); // 检查子节点是否为只读

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);
	// 数据结构的操作
	// 添加子节点
	void addChild(ASAbstractData* child);
	void addChild(ASAbstractData* child, int insertPos);
	// 删除子节点
	void removeChild(ASAbstractData* child);
	void removeChild(int removePos);
	// 得到子节点
	QList<ASAbstractData*>* getChild();
	// 得到第一个某类型的子节点
	ASAbstractData* getChild(const int c_Type);
	// 得到父节点，更新父节点
	ASAbstractData* getFather();
	void setFather(ASAbstractData* father);
	// 得到根节点，即第一级的数据节点
	ASAbstractData* getRoot();
	// 是否为空节点
	bool isVoidNode();
	void setVoidNode(const bool c_isVoidNode);
	// 是否显示
	virtual void setShown(const bool shown);					
	bool isShown();
	// 数据类型		
	int getDataType();
	// 第几个孩子
	int getOrderInBrother(ASAbstractData* child);
	// 是否正在被运算
	void setBeingProcess(const bool isBeingProcess);	
	bool isBeingProcess();
	// 该节点的ID
	int getDataNodeID(); 
	// Name
	void setName(QString name);
	QString getName();
	void setFileName(QString filename);
	QString getFileName();
	// 数据范围和时间范围
	virtual double* getDataExtent(); // 得到数据范围，double[6]
	int getNumberOfFrame(); // 得到数据有几帧

	// 多线程
	QMutex* GetMutex();

	// 静态成员函数
	// 由ID返回节点
	static ASAbstractData* getDataNodeFromID(int id);
	// 得到当前已经打开的文件数量
	static int getNumberOfData();
	// 得到当前已经显示的文件数量
	static int getNumberOfDataShown();

	// ---------------------- 非外部接口函数 ----------------------
	// 数据结构
	static void setRootDataNode(ASAbstractData* node); // 设定根节点
	ASAbstractData* findID(int nID); // 查找具有该ID的节点，该函数为迭代函数，用于实现根据ID查找节点的功能功能。
	void Traverse(std::queue<ASAbstractData*>* queue); // 该函数只可以被ASDataStructureWalker调用，用于数据的遍历。实现的功能只是添加该节点的子节点到给定的队列中

private:
	bool			m_isShown;				// 是否显示
	bool			m_isBeingProcess;		// 是否正在被运算，运算状态下为只读	

	static int ms_nNumberOfData;			// 当前打开的数据的数量
	static int ms_nNumberOfDataShown;		// 当前显示的数据的数量

protected:
	int				m_Datatype;				// 数据类型
	//该节点唯一的表示符
	int m_nDataNoteID;
	//数据属性
	bool			m_isVoidNode;			// 是否为空节点，不含有数据
	QString			m_Name;					// 数据名
	QString			m_FileName;				// 路径名，包含文件名
	double			m_adDataExtent[6];		// 范围，世界坐标系
	int				m_NumberOfFrames = 1;	// 该数据有几帧
	//数据结构属性
	ASAbstractData* m_pFather;				// 父节点
	QList<ASAbstractData*>* m_pChildren;	// 树状存储结构中的孩子

	QMutex* m_Mutex; // 多线程线程锁

	//静态成员和静态成员函数
	static ASAbstractData* ms_RootDataNode; // 根节点
	static int getNewDataNodeID(); // 得到新的ID，与先有ID均不重复。

};
