#include "ASPipelineManager.h"
#include "ASPipelineBase.h"
#include "ASPipelineArrayImage.h"
#include "ASPipelinePolyImage.h"
#include "ASPipelineCrossLine2D.h"
#include "ASPipelineOuterBorder3D.h"
#include "ASPipelineMask.h"
#include "ASPipelineSeeds.h"
#include "ASPipelineContour.h"
#include "ASPipelineLandmark.h"
#include "ASPipelineText.h"
#include "ASPipelineROICube.h"

ASPipelineManager* ASPipelineManager::m_PipelineManager = nullptr;
int ASPipelineManager::m_PosTraversing = 0;

ASPipelineManager::ASPipelineManager()
{
	m_listPipeline = new QList<ASPipelineBase*>;
	m_PipelineManager = this;
}

ASPipelineManager::~ASPipelineManager()
{

}

ASPipelineBase* ASPipelineManager::AddPipeline(const int c_TypeOfPipeline)
{
	int NewPipeID = -1;
	bool FindNewID = false;
	while (FindNewID == false)
	{
		FindNewID = true;
		NewPipeID++;
		foreach(ASPipelineBase* crntPipe, *(m_PipelineManager->m_listPipeline))
		{
			if (crntPipe->getIDOfPipeline() == NewPipeID)
			{
				FindNewID = false;
			}
		}
	}

	ASPipelineBase* newPipeLine;
	switch (c_TypeOfPipeline)
	{
	case PipeLineType_ArrayImage:
		newPipeLine = new ASPipelineArrayImage(NewPipeID);
		break;
	case PipeLineType_PolyImage:
		newPipeLine = new ASPipelinePolyImage(NewPipeID);
		break;
	case PipeLineType_CrossLine2D:
		newPipeLine = new ASPipelineCrossLine2D(NewPipeID);
		break;
	case PipeLineType_OuterBorder3D:
		newPipeLine = new ASPipelineOuterBorder3D(NewPipeID);
		break;
	case PipeLineType_Mask:
		newPipeLine = new ASPipelineMask(NewPipeID);
		break;
	case PipeLineType_Seeds:
		newPipeLine = new ASPipelineSeeds(NewPipeID);
		break;
	case PipeLineType_Contour:
		newPipeLine = new ASPipelineContour(NewPipeID);
		break;
	case PipeLineType_Landmark:
		newPipeLine = new ASPipelineLandmark(NewPipeID);
		break;
	case PipeLineType_Text:
		newPipeLine = new ASPipelineText(NewPipeID);
		break;
	case PipeLineType_ROICube:
		newPipeLine = new ASPipelineROICube(NewPipeID);		
		break;
	default:
		break;
	}
	m_PipelineManager->m_listPipeline->append(newPipeLine);
	return newPipeLine;
}
void ASPipelineManager::RemovePipeline(ASPipelineBase* pPipeline)
{
	m_PipelineManager->m_listPipeline->removeOne(pPipeline);
	pPipeline->DeleteNode();
}
void ASPipelineManager::UpdatePipeline2DTransformMatrix()
{
	foreach(ASPipelineBase* crntPipeline, *(m_PipelineManager->m_listPipeline))
	{
		crntPipeline->Update2DTransformMatrix();
	}
}

// 由数据ID查找管线，一个数据可能对应多个管线
void ASPipelineManager::GetPipelineForDataID(QList<ASPipelineBase*>* pList, const int c_DataID)
{
	foreach(ASPipelineBase* crntPipeline, *(m_PipelineManager->m_listPipeline))
	{
		if (crntPipeline->getIDOfData() == c_DataID)
		{
			pList->append(crntPipeline);
		}
	}
}

// 渲染管线的遍历
ASPipelineBase* ASPipelineManager::bigen_inherit(int type)
{
	m_PosTraversing = 0;
	for (m_PosTraversing; m_PosTraversing < m_PipelineManager->m_listPipeline->size(); m_PosTraversing++)
	{
		if (m_PipelineManager->m_listPipeline->at(m_PosTraversing)->isTypeInherited(type))
		{
			return m_PipelineManager->m_listPipeline->at(m_PosTraversing++);
		}		
	}
	return nullptr;
}
ASPipelineBase* ASPipelineManager::bigen_single(int type)
{
	m_PosTraversing = 0;
	for (m_PosTraversing; m_PosTraversing < m_PipelineManager->m_listPipeline->size(); m_PosTraversing++)
	{
		if (m_PipelineManager->m_listPipeline->at(m_PosTraversing)->isTypeAbsolute(type))
		{
			return m_PipelineManager->m_listPipeline->at(m_PosTraversing++);
		}
	}
	return nullptr;
}
ASPipelineBase* ASPipelineManager::next_inherit(int type)
{
	for (m_PosTraversing; m_PosTraversing < m_PipelineManager->m_listPipeline->size(); m_PosTraversing++)
	{
		if (m_PipelineManager->m_listPipeline->at(m_PosTraversing)->isTypeInherited(type))
		{
			return m_PipelineManager->m_listPipeline->at(m_PosTraversing++);
		}
	}
	return nullptr;
}
ASPipelineBase* ASPipelineManager::next_single(int type)
{
	for (m_PosTraversing; m_PosTraversing < m_PipelineManager->m_listPipeline->size(); m_PosTraversing++)
	{
		if (m_PipelineManager->m_listPipeline->at(m_PosTraversing)->isTypeAbsolute(type))
		{
			return m_PipelineManager->m_listPipeline->at(m_PosTraversing++);
		}
	}
	return nullptr;
}
