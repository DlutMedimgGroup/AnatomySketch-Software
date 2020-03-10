#include "ASFModelContour.h"
#include "ASContourData.h"
#include "ASPipelineContour.h"
#include "ASPipelineManager.h"
#include "ASDataTree.h"
#include "ASTransDataGeneral.h"
#include "ASArrayImageData.h"
#include "ASMainWidget.h"
#include "ASTransContour.h"
#include "ASAnnotation.h"
#include <QMessageBox>

ASFModelContour* ASFModelContour::ms_SelfPointer = nullptr;

ASFModelContour::ASFModelContour(QObject *parent)
	: ASFunctionModelBase(parent)
{
	ms_SelfPointer = this;
}

ASFModelContour::~ASFModelContour()
{

}
// 当前绘制的种子点的ID
void ASFModelContour::SetIDOfPaintingData(const int c_ID)
{
	ms_SelfPointer->m_IDOfPaintingData = c_ID;
}
int ASFModelContour::GetIDOfPaintingData()
{
	return ms_SelfPointer->m_IDOfPaintingData;
}
// 当前绘制数据节点中轮廓线的序号
void ASFModelContour::SetCurrentContourID(const int c_ID)
{
	m_CurrentContourID = c_ID;
}
int ASFModelContour::GetCurrentContourID()
{
	return ms_SelfPointer->m_CurrentContourID;
}
// 模块使能状态
void ASFModelContour::SetContourModleEnabled(const bool c_enabled)
{
	if (this->m_ContourModelEnabled == c_enabled)
	{
		return;
	}
	this->m_ContourModelEnabled = c_enabled;
	// 遍历轮廓管线设置是否可以拾取
	if (m_PipelineContour != nullptr)
	{
		m_PipelineContour->SetPickable(m_ContourModelEnabled);
	}
}
bool ASFModelContour::GetContourModleEnabled()
{
	return ms_SelfPointer->m_ContourModelEnabled;
}
// 没建立管线就建立管线
void ASFModelContour::InitContourPipeline()
{
	if (m_PipelineContour == nullptr)
	{
		m_PipelineContour = static_cast<ASPipelineContour*>(
			ASPipelineManager::AddPipeline(PipeLineType_Contour));
		m_PipelineContour->SetPickable(m_ContourModelEnabled);
		m_PipelineContour->SetShown(m_ContourShown);
		m_PipelineContour->RefreshSplineContourDisplay(0);
		m_PipelineContour->RefreshSplineContourDisplay(1);
		m_PipelineContour->RefreshSplineContourDisplay(2);
	}
}
// 轮廓线管线是否显示，没有2D内容时不显示种子点
void ASFModelContour::SetContourShown(const bool c_Shown)
{
	m_ContourShown = c_Shown;
	if (m_PipelineContour != nullptr)
	{
		m_PipelineContour->SetShown(m_ContourShown);
	}
}
// 交互模式。0, 普通; 1, 触控
void ASFModelContour::SetInterfaceModel(const int c_model)
{
	m_InterfaceModel = c_model;
}
int ASFModelContour::GetInterfaceModel()
{
	return ms_SelfPointer->m_InterfaceModel;
}

// 轮廓线操作
// 添加临时控制点
void ASFModelContour::AddContrlPointsTemp(double* pPosition, const int ViewLabel, double* pSpacing)
{
	ASContourData* crntData = ASTransContour::GetCurrentContourData();
	if (crntData != nullptr)
	{
		crntData->AddContrlPointsTemp(pPosition, ViewLabel, pSpacing, m_CurrentContourID);
	}
}
// 添加控制点
void ASFModelContour::AddContrlPoints(double* pPosition, const int ViewLabel, double* pSpacing)
{
	ASContourData* crntData = ASTransContour::GetCurrentContourData();
	if (crntData != nullptr)
	{
		crntData->AddContrlPoints(pPosition, ViewLabel, pSpacing, m_CurrentContourID);
	}
}
// 移动控制点
void ASFModelContour::MoveContrlPoints(double* pPosition, const int c_ViewLabel, double* pSpacing, 
	const int c_ID, const int c_ContourID, const int c_IndexOfContour, const int c_IndexOfHandle)
{
	ASContourData* crntData = ASTransContour::GetCurrentContourData();
	if (crntData != nullptr)
	{
		if (crntData->getDataNodeID() == c_ID || c_ID == -1)
		{
			crntData->MoveContrlPoints(pPosition, c_ViewLabel, pSpacing, c_ContourID,
				c_IndexOfContour, c_IndexOfHandle);
		}
	}
}
// 闭合轮廓线
void ASFModelContour::SetContourClosed(const int c_id, const int c_viewlabel, 
	const int c_indexOfContour, const int c_indexOfHandle, const bool c_Closed)
{
	ASContourData* crntData = ASTransContour::GetCurrentContourData();
	if (crntData != nullptr)
	{
		if (crntData->getDataNodeID() == c_id)
		{
			crntData->SetContourClosed(c_viewlabel, c_indexOfContour, c_indexOfHandle, c_Closed, 
				m_CurrentContourID);
		}
	}
}
// 删除控制点
void ASFModelContour::DeleteContrlPoints(const int c_ViewLabel, double* pSpacing, const int c_ID, 
	const int c_ContourID, const int c_IndexOfContour, const int c_IndexOfHandle)
{
	ASContourData* crntData = ASTransContour::GetCurrentContourData();
	if (crntData != nullptr)
	{
		if (crntData->getDataNodeID() == c_ID)
		{
			crntData->DeleteContrlPoints(c_ViewLabel, pSpacing, c_ContourID, c_IndexOfContour,
				c_IndexOfHandle);
		}
	}
}
// 删除最后一个控制点
void ASFModelContour::DeleteContrlPointsTemp(double* pPosition, const int c_ViewLabel, double* pSpacing)
{
	ASContourData* crntData = ASTransContour::GetCurrentContourData();
	if (crntData != nullptr)
	{
		crntData->DeleteContrlPointsTemp(pPosition, c_ViewLabel, pSpacing, m_CurrentContourID);
	}
}
// 插入控制点
void ASFModelContour::InsertContrlPoints(double* pPosition, const int c_ViewLabel, double* pSpacing, 
	const int c_ID, const int c_IndexOfContour, const int c_IndexOfHandle)
{
	ASContourData* crntData = ASTransContour::GetCurrentContourData();
	if (crntData != nullptr)
	{
		if (crntData->getDataNodeID() == c_ID)
		{
			crntData->InsertContrlPoints(pPosition, c_ViewLabel, pSpacing, m_CurrentContourID, 
				c_IndexOfContour, c_IndexOfHandle);
		}
	}
}
// 删除轮廓
void ASFModelContour::DeleteContour(const int c_id, const int c_ContourID, const int c_viewlabel, 
	const int c_indexOfContour, const int c_indexOfHandle)
{
	ASContourData* crntData = ASTransContour::GetCurrentContourData();
	if (crntData != nullptr)
	{
		if (crntData->getDataNodeID() == c_id)
		{
			crntData->DeleteContour(c_viewlabel, c_indexOfContour, c_indexOfHandle, c_ContourID);
		}
	}
}
// 删除整个轮廓数据
void ASFModelContour::DeleteWholeContour(const int c_DataID, const int c_ContourID)
{
	ASAbstractData* crntData = ASAbstractData::getDataNodeFromID(c_DataID);
	if (crntData->isTypeInherited(DataType_ContourData))
	{
		ASContourData* crntSeed = static_cast<ASContourData*>(crntData);
		crntSeed->DeleteWholeContour(c_ContourID);
	}
	//ASContourData* crntData = ASTransContour::GetCurrentContourData();
	//if (crntData != nullptr && crntData->getDataNodeID() == c_DataID)
	//{
	//	crntData->DeleteWholeContour(c_ContourID);
	//}
}
// 设置控制点直径
void ASFModelContour::ReSetCtrlRadius(const int c_ViewLabel)
{
	if (m_PipelineContour != nullptr)
	{
		m_PipelineContour->ReSetCtrlRadius(c_ViewLabel);
	}
}
// 显示
void ASFModelContour::RefreshSplineContourDisplay(const int c_ViewLabel)
{
	if (m_PipelineContour != nullptr)
	{
		m_PipelineContour->RefreshSplineContourDisplay(c_ViewLabel);
	}
}
// 根据actor地址查询控制点信息
bool ASFModelContour::ReserchHandleSplineContour(vtkActor* pActor, int* ID, int* ContourID, int* ViewLabel,
	int* ItemOfContour, int* ItemOfHandle)
{
	if (m_PipelineContour == nullptr)
	{
		return false;
	}
	return m_PipelineContour->reserchHandleSplineContour(pActor, ID, ContourID, ViewLabel, ItemOfContour, 
		ItemOfHandle);
}
// 插入控制点时根据坐标判断控制点位置
bool ASFModelContour::GetInsertHandleIndex(double* pos, const int c_viewLabel, int* ID, 
	int* ItemOfContour, int* ItemOfHandle)
{
	if (m_PipelineContour == nullptr)
	{
		return false;
	}
	return m_PipelineContour->getInsertHandleIndex(pos, c_viewLabel, ID, ItemOfContour, ItemOfHandle);
}

// 读取轮廓线数据
void ASFModelContour::LoadContourData(const QString c_FileName)
{
	ASArrayImageData* GenData = static_cast<ASArrayImageData*>(
		ASTransDataGeneral::GetGeneratingData());
	if (!GenData)
	{
		// 无当前数据，新建失败
		QMessageBox::information(ASMainWidget::GetMainWidget(), tr("New Seed Point"),
			tr("Please set current greyscale data."), QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	// 判断是否需要新建标记文件
	ASAnnotation* AnnotationData = static_cast<ASAnnotation*>(
		ASTransDataGeneral::GetGeneratingData()->getChild(DataType_Annotation));
	if (!AnnotationData)
	{
		// 新建Annotation
		AnnotationData = ASTransDataGeneral::NewAnnotationData(ASTransDataGeneral::GetGeneratingData());
	}
	// 判断是否需要新建轮廓线文件
	ASContourData* ContourData = static_cast<ASContourData*>(AnnotationData->getChild(DataType_ContourData));
	if (!ContourData)
	{
		ContourData = new ASContourData();
		ContourData->setName(tr("Contour data"));
		ContourData->setShown(true);
		ASDataTree::NewDataNode(ContourData, AnnotationData);
		if (!ContourData)
		{
			qDebug("Error: Null pointer of current ContourData. From ASPaintingContourWidget.");
			return;
		}
	}
	// 读取
	QStringList StringList;
	QFile file(c_FileName);
	QStringList list;
	QStringList list2;
	if (file.open(QIODevice::ReadOnly))
	{
		while (!file.atEnd())
			StringList << file.readLine();

		file.close();
	}
	int ncrntLine = 0;
	// 0 检查文件头
	if (StringList.at(ncrntLine++) != QString("#Anatomy Sketch Contour File\n"))
	{
		return;
	}
	// 1 曲面轮廓数量
	if (StringList.at(ncrntLine++) != QString("#NUMBER OF DATA:\n"))
	{
		return;
	}
	int NumOfContourData = StringList.at(ncrntLine++).toInt();
	// 2 DATALIST
	if (StringList.at(ncrntLine++) != QString("#DATA LIST DEFINITION:\n"))
	{
		return;
	}
	for (int ncrntContourData = 0; ncrntContourData < NumOfContourData; ncrntContourData++)
	{
		// 2.1 DATA LIST NUMBER
		if (StringList.at(ncrntLine++) != QString("#DATA LIST NUMBER ") + 
			QString::number(ncrntContourData) + QString(" DEFINITION:\n"))
		{
			return;
		}
		// 2.2 data_list_index
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("data_list_index\n"))
		{
			return;
		}
		if (list.at(0).toInt() != ncrntContourData)
		{
			return;
		}
		// 2.3 data_name
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("data_name\n"))
		{
			return;
		}
		QString name = list.at(0);
		// 2.4 Color
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("r_color g_color b_color\n"))
		{
			return;
		}
		list2 = list.at(0).split(" ");
		QColor color(list2.at(0).toInt(), list2.at(1).toInt(), list2.at(2).toInt());
		// 2.5 Label
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("data_label\n"))
		{
			return;
		}
		int label = list.at(0).toInt();
		// 2.6 number_of_contours
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("number_of_contours\n"))
		{
			return;
		}
		int id = ContourData->NewContour(name, label, color);
		if (id < 0)
		{
			qDebug("error: Bad new contour id. From ASFModelContour");
			continue;
		}
		int NumOfContour = list.at(0).toInt();
		structContour* crntContour = ContourData->GetContourFromID(id);
		for (int ncrntContour = 0; ncrntContour < NumOfContour; ncrntContour++)
		{
			structCurve* crntCurve = new structCurve;
			if (StringList.at(ncrntLine++) != QString("#CONTOUR LIST NUMBER ") + 
				QString::number(ncrntContour) + QString(" DEFINITION:\n"))
			{
				return;
			}
			// 2.6.1 contour_index
			list = StringList.at(ncrntLine++).split(" # ");
			if (list.at(1) != QString("contour_index\n"))
			{
				return;
			}
			if (list.at(0).toInt() != ncrntContour)
			{
				return;
			}
			// 2.6.2 contour_ViewLabel
			list = StringList.at(ncrntLine++).split(" # ");
			if (list.at(1) != QString("contour_ViewLabel\n"))
			{
				return;
			}
			crntCurve->ViewLabel = list.at(0).toInt();
			// 2.6.3 contour_isClosure
			list = StringList.at(ncrntLine++).split(" # ");
			if (list.at(1) != QString("contour_isClosure\n"))
			{
				return;
			}
			if (list.at(0) == QString("true"))
			{
				crntCurve->isClosure = true;
			} 
			else
			{
				crntCurve->isClosure = false;
			}
			// 2.6.4 contour_dLayer
			list = StringList.at(ncrntLine++).split(" # ");
			if (list.at(1) != QString("contour_dLayer\n"))
			{
				return;
			}
			crntCurve->dLayer = list.at(0).toDouble();
			// 2.6.5 Points
			if (StringList.at(ncrntLine++) != QString("#START_POINTS_POS\n"))
			{
				return;
			}
			while (true)
			{
				list = StringList.at(ncrntLine++).split(" ");
				if (list.size() != 3)
				{
					break;
				}
				structPoint* ncrntPoint = new structPoint;
				ncrntPoint->dX = list.at(0).toDouble();
				ncrntPoint->dY = list.at(1).toDouble();
				ncrntPoint->dZ = list.at(2).toDouble();
				crntCurve->pvectorPoint.append(ncrntPoint);
			}
			crntContour->pListCurves->append(crntCurve);
		}
	}
}