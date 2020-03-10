#include "ASPipelineText.h"
#include "ASVisualizationManager.h"
#include "ASVisualizationManager2D.h"
#include <QTextCodec>

ASPipelineText* ASPipelineText::ms_SelfPointer = nullptr;

ASPipelineText::ASPipelineText(const int c_ID) : ASPipelineBase(c_ID)
{
	m_PipelineType = PipeLineType_Text;

	ms_SelfPointer = this;

	m_listTextPipe = new QList<structTextPipe*>;
}

ASPipelineText::~ASPipelineText()
{
	
}

// 管线类型判断
bool ASPipelineText::isTypeInherited(const int c_Type)
{
	if (c_Type == PipeLineType_Text || ASPipelineBase::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASPipelineText::isTypeAbsolute(const int c_Type)
{
	if (c_Type == PipeLineType_Text)
	{
		return true;
	}
	return false;
}
void ASPipelineText::DeleteNode()
{

}

// 虚函数，切图焦点改变
void ASPipelineText::Update2DTransformMatrix()
{

}

int ASPipelineText::ShowText(const int c_ViewLabel, const QString c_text, double* pos1, double* pos2)
{
	structTextPipe* crntTextPipe = new structTextPipe;
	crntTextPipe->pTexProper = vtkSmartPointer<vtkTextProperty>::New();
	crntTextPipe->pTexProper->SetColor(1, 0, 0);
	crntTextPipe->pTexProper->SetFontSize(18);
	crntTextPipe->pTexProper->SetFontFamily(0);
	crntTextPipe->textActor = vtkSmartPointer<vtkTextActor>::New(); //声明3D文本
	std::string strStd = c_text.toStdString();
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	if (code) strStd = code->fromUnicode(c_text).data();
	crntTextPipe->textActor->SetInput(strStd.data());
	crntTextPipe->textActor->SetTextProperty(crntTextPipe->pTexProper);
	crntTextPipe->textActor->SetPosition(pos1);
	crntTextPipe->textActor->SetPosition2(pos2); //设置位置

	crntTextPipe->VeiwLabel = c_ViewLabel;
	bool succFlag = false;
	int crntID = 0;
	int numOfText = ms_SelfPointer->m_listTextPipe->size();
	while (succFlag == false)
	{
		succFlag = true;
		for (int ncrntItem = 0; ncrntItem < numOfText; ncrntItem++)
		{
			if (ms_SelfPointer->m_listTextPipe->at(ncrntItem)->textID == crntID)
			{
				succFlag = false;
				crntID++;
			}
		}
	}
	crntTextPipe->textID = crntID;
	ms_SelfPointer->m_listTextPipe->append(crntTextPipe);

	vtkSmartPointer<vtkRenderer> Renderer2D[3];
	ASVisualizationManager2D::GetRenderer2D(Renderer2D);
	Renderer2D[c_ViewLabel]->AddActor(crntTextPipe->textActor);

	return crntID;
}
void ASPipelineText::DeleteText(const int c_TextID)
{
	int numOfText = ms_SelfPointer->m_listTextPipe->size();
	for (int ncrntItem = 0; ncrntItem < numOfText; ncrntItem++)
	{
		if (ms_SelfPointer->m_listTextPipe->at(ncrntItem)->textID == c_TextID)
		{
			structTextPipe* crntPipe = ms_SelfPointer->m_listTextPipe->at(ncrntItem);
			vtkSmartPointer<vtkRenderer> Renderer2D[3];
			ASVisualizationManager2D::GetRenderer2D(Renderer2D);
			Renderer2D[crntPipe->VeiwLabel]->RemoveActor(crntPipe->textActor);
			crntPipe->textActor = nullptr;
			crntPipe->pTexProper = nullptr;
			ms_SelfPointer->m_listTextPipe->removeOne(crntPipe);
			return;
		}
	}
}