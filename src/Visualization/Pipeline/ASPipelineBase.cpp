#include "ASPipelineBase.h"

int ASPipelineBase::ms_NumOfShowingObj2D = 0;
int ASPipelineBase::ms_NumOfObj2D = 0;

ASPipelineBase::ASPipelineBase(const int c_ID)
{
	m_IDOfPipeline = c_ID;
	m_PipelineType = PipeLineType_Base;
}

ASPipelineBase::~ASPipelineBase()
{

}

// 管线类型判断
bool ASPipelineBase::isTypeInherited(const int c_Type)
{
	if (c_Type == PipeLineType_Base)
	{
		return true;
	}
	return false;
}
bool ASPipelineBase::isTypeAbsolute(const int c_Type)
{
	if (c_Type == PipeLineType_Base)
	{
		return true;
	}
	return false;
}

// ID
int ASPipelineBase::getIDOfPipeline()
{
	return m_IDOfPipeline;
}
int ASPipelineBase::getIDOfData()
{
	return m_IDOfData;
}
// Type
int ASPipelineBase::GetPipeLineType()
{
	return this->m_PipelineType;
}

// 虚函数
// 调用此函数来释放管线
void ASPipelineBase::DeleteNode()
{
	delete this;
}
// 设置对应数据的ID
void ASPipelineBase::SetDataID(const int c_DataID)
{
	this->m_IDOfData = c_DataID;
}
// 切图焦点改变
void ASPipelineBase::Update2DTransformMatrix()
{

}
// 是否显示
void ASPipelineBase::SetShown(const bool c_Shown)
{

}
// 重置显示
void ASPipelineBase::ReinitDisplay()
{

}

// 得到当前二维窗口显示出来的对象数目
int ASPipelineBase::GetNumOfShowingObj2D()
{
	return ms_NumOfShowingObj2D;
}
// 得到当前二维窗口所有的对象数目
int ASPipelineBase::GetNumOfObj2D()
{
	return ms_NumOfObj2D;
}
