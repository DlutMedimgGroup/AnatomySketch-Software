#pragma once
#include "ASPipelineBase.h"
#include <vtkSmartPointer.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <QList>

typedef struct structTextPipe
{
	int textID;
	int VeiwLabel;
	vtkSmartPointer<vtkTextProperty> pTexProper;
	vtkSmartPointer<vtkTextActor> textActor;
}structTextPipe;

class ASPipelineText : public ASPipelineBase
{
public:
	ASPipelineText(const int c_ID);
	~ASPipelineText();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type);
	virtual bool isTypeAbsolute(const int c_Type);
	virtual void DeleteNode();	// 调用此函数来释放管线

	// 虚函数，切图焦点改变
	virtual void Update2DTransformMatrix();

	// 显示文字，返回文字ID，消除文字时需要使用
	static int ShowText(const int c_ViewLabel, const QString c_text, double* pos1, double* pos2);
	// 移除文字
	static void DeleteText(const int c_TextID);

private:

	// 该管线是唯一的，唯一对象
	static ASPipelineText* ms_SelfPointer;

	QList<structTextPipe*>* m_listTextPipe;
	
};
