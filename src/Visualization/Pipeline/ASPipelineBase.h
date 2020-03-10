#pragma once

// 管线
#define PipeLineType_Base			0
#define PipeLineType_ArrayImage		1
#define PipeLineType_PolyImage		2
#define PipeLineType_CrossLine2D	3
#define PipeLineType_OuterBorder3D	4
#define PipeLineType_Mask			5
#define PipeLineType_Seeds			6
#define PipeLineType_Contour		7
#define PipeLineType_Landmark		8
#define PipeLineType_Text			9
#define PipeLineType_ROICube		10

class ASPipelineBase
{
public:
	ASPipelineBase(const int c_ID);
	~ASPipelineBase();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type); 
	virtual bool isTypeAbsolute(const int c_Type);

	// ID
	int getIDOfPipeline();
	int getIDOfData();
	// Type
	int GetPipeLineType();

	// 虚函数
	virtual void DeleteNode();	// 调用此函数来释放管线
	virtual void SetDataID(const int c_DataID); // 设置对应数据的ID
	virtual void Update2DTransformMatrix(); // 切图焦点改变
	virtual void SetShown(const bool c_Shown); // 是否显示
	virtual void ReinitDisplay(); // 重置显示

	
	static int GetNumOfShowingObj2D();	// 得到当前二维窗口显示出来的对象数目
	static int GetNumOfObj2D();	// 得到当前二维窗口所有的对象数目

protected:
	int m_PipelineType = -1;	// 渲染管线种类
	int m_IDOfPipeline = -1;	// 渲染管线的ID
	int m_IDOfData = -1;		// 对应的数据的ID，

	static int ms_NumOfShowingObj2D;
	static int ms_NumOfObj2D;

private:


};
