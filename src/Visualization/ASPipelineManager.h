#pragma once
#include <QList>

class ASPipelineBase;

// 用于数据存储结构的遍历
// 例如 for (ASPipelineBase* item = ASPipelineManager::bigen_single(PipeLineType_ArrayImage); item != nullptr; item = ASPipelineManager::next_single(PipeLineType_ArrayImage)){}
//      for (ASPipelineBase* item = ASPipelineManager::bigen_inherit(PipeLineType_ArrayImage); item != nullptr; item = ASPipelineManager::next_inherit(PipeLineType_ArrayImage)){}

// 查找管线
// QList<ASPipelineBase*>* listPipe = new QList<ASPipelineBase*>;
// ASPipelineManager::GetPipelineForDataID(listPipe, ID);

class ASPipelineManager
{
public:
	ASPipelineManager();
	~ASPipelineManager();

	// 新建，移除管线
	static ASPipelineBase* AddPipeline(const int c_TypeOfPipeline); // 新建管线，返回管线指针
	static void RemovePipeline(ASPipelineBase* pPipeline);
	// 调用所有管线的Update2DTransformMatrix函数，更新焦点
	static void UpdatePipeline2DTransformMatrix();
	// 由数据ID查找管线，一个数据可能对应多个管线
	static void GetPipelineForDataID(QList<ASPipelineBase*>* pList, const int c_DataID);

	// 渲染管线的遍历
	static ASPipelineBase* bigen_inherit(int type);
	static ASPipelineBase* bigen_single(int type);
	static ASPipelineBase* next_inherit(int type);
	static ASPipelineBase* next_single(int type);

private:

	// 唯一对象
	static ASPipelineManager* m_PipelineManager;
	// 存储管线的列表
	QList<ASPipelineBase*>* m_listPipeline;
	// 用于管线遍历，保存遍历位置
	static int m_PosTraversing;

};
