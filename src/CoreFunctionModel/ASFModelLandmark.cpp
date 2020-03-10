#include "ASFModelLandmark.h"
#include "ASLandmarkData.h"
#include "ASPipelineManager.h"
#include "ASPipelineBase.h"
#include "ASPipelineLandmark.h"
#include "ASDataTree.h"
#include "ASTransDataGeneral.h"
#include "ASDataStructureWalker.h"
#include "ASTransLandmark.h"
#include "ASArrayImageData.h"
#include "ASAnnotation.h"
#include "ASMainWidget.h"
#include "ASTransAddDeleteData.h"
#include <QMessageBox>

ASFModelLandmark* ASFModelLandmark::ms_SelfPointer = nullptr;

ASFModelLandmark::ASFModelLandmark(QObject *parent)
	: ASFunctionModelBase(parent)
{
	ms_SelfPointer = this;
}

ASFModelLandmark::~ASFModelLandmark()
{

}

//// 当前绘制的标定点的ID
//void ASFModelLandmark::SetIDOfPaintingData(const int c_ID)
//{
//	ms_SelfPointer->m_IDOfPaintingData = c_ID;
//}
//int ASFModelLandmark::GetIDOfPaintingData()
//{
//	return ms_SelfPointer->m_IDOfPaintingData;
//}
// 模块使能状态
void ASFModelLandmark::SetLandmarkModleEnabled(const bool c_enabled)
{
	if (this->m_LandmarkModelEnabled == c_enabled)
	{
		return;
	}
	this->m_LandmarkModelEnabled = c_enabled;
}
bool ASFModelLandmark::GetLandmarkModleEnabled()
{
	return ms_SelfPointer->m_LandmarkModelEnabled;
}
// 没建立管线就建立管线
void ASFModelLandmark::InitLandmarkPipeline()
{
	if (m_PipelineLandmark == nullptr)
	{
		m_PipelineLandmark = static_cast<ASPipelineLandmark*>(ASPipelineManager::AddPipeline(PipeLineType_Landmark));
		m_PipelineLandmark->SetShown(m_LandmarkShown);
		m_PipelineLandmark->RefreshLandmarkDisplay(0);
		m_PipelineLandmark->RefreshLandmarkDisplay(1);
		m_PipelineLandmark->RefreshLandmarkDisplay(2);
	}
}
// 轮廓线管线是否显示，没有2D内容时不显示种子点
void ASFModelLandmark::SetLandmarkShown(const bool c_Shown)
{
	m_LandmarkShown = c_Shown;
	if (m_PipelineLandmark != nullptr)
	{
		m_PipelineLandmark->SetShown(m_LandmarkShown);
	}
}

// 标定点操作
// 添加标定点
void ASFModelLandmark::AddLandmark(double* pPosition, QString qsname)
{
	ASLandmarkData* crntData = ASTransLandmark::GetCurrentLMData();
	if (crntData != nullptr)
	{
		crntData->AddLandMark(pPosition, qsname);
	}
}
// 移动标定点
void ASFModelLandmark::MoveLandmark(double* pPos, const int c_DataID, const int c_ViewLabel, const int c_PointID)
{
	ASLandmarkData* crntData = ASTransLandmark::GetCurrentLMData();
	if (crntData != nullptr || crntData->getDataNodeID() != c_DataID)
	{
		crntData->MoveLandMark(pPos, c_PointID);
	}
}
// 删除标定点
void ASFModelLandmark::DeleteLandmark(const int c_DataID, const int c_PointID)
{
	ASAbstractData* crntData = ASAbstractData::getDataNodeFromID(c_DataID);
	if (crntData->isTypeInherited(DataType_LandmarkData))
	{
		ASLandmarkData* crntSeed = static_cast<ASLandmarkData*>(crntData);
		crntSeed->DeleteLandMark(c_PointID);
	}
	//ASLandmarkData* crntData = ASTransLandmark::GetCurrentLMData();
	//if (crntData != nullptr && crntData->getDataNodeID() == c_DataID)
	//{
	//	crntData->DeleteLandMark(c_PointID);
	//}
}

// 设置控制点直径
void ASFModelLandmark::ReSetCtrlRadius(const int c_ViewLabel)
{
	if (m_PipelineLandmark != nullptr)
	{
		m_PipelineLandmark->ReSetCtrlRadius(c_ViewLabel);
	}
}
// 显示
void ASFModelLandmark::RefreshLMDisplay(const int c_ViewLabel)
{
	m_PipelineLandmark->RefreshLandmarkDisplay(c_ViewLabel);
}
// 根据actor地址查询控制点信息
bool ASFModelLandmark::ReserchLanmark(vtkActor* pActor, int* LandmarkDataID, int* ViewLabel, int* LandmarkPointID)
{
	if (m_PipelineLandmark == nullptr)
	{
		return false;
	}
	return m_PipelineLandmark->ReserchLanmark(pActor, LandmarkDataID, ViewLabel, LandmarkPointID);
}
// 读取标定点数据
void ASFModelLandmark::LoadLandmarkData(const QString c_FileName)
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
	// 判断当前是否由ROI文件
	ASLandmarkData* crntLandMData = static_cast<ASLandmarkData*>(AnnotationData->getChild(DataType_LandmarkData));
	if (!crntLandMData)
	{
		crntLandMData = new ASLandmarkData();
		crntLandMData->setBeingProcess(false);
		crntLandMData->setName(tr("Landmark data"));
		crntLandMData->setShown(true);
		ASDataTree::NewDataNode(crntLandMData, AnnotationData);
		if (!crntLandMData)
		{
			return;
		}
	}
	// 读取文件
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
	if (StringList.at(ncrntLine++) != QString("#Anatomy Sketch Landmark File\n"))
	{
		return;
	}
	// 1 标定点数量
	if (StringList.at(ncrntLine++) != QString("#NUMBER OF DATA:\n"))
	{
		return;
	}
	int NumOfLandMData = StringList.at(ncrntLine++).toInt();
	// 2 DATALIST
	if (StringList.at(ncrntLine++) != QString("#DATA LIST DEFINITION:\n"))
	{
		return;
	}
	for (int ncrntLandMData = 0; ncrntLandMData < NumOfLandMData; ncrntLandMData++)
	{
		// 2.1 DATA LIST NUMBER
		if (StringList.at(ncrntLine++) != QString("#DATA LIST NUMBER ") + 
			QString::number(ncrntLandMData) + QString(" DEFINITION:\n"))
		{
			return;
		}
		// 2.2 data_list_index
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("data_list_index\n"))
		{
			return;
		}
		if (list.at(0).toInt() != ncrntLandMData)
		{
			return;
		}
		// 2.3 data_name
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("data_name\n"))
		{
			return;
		}
		// 2.4 number_of_landmarks
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("number_of_landmarks\n"))
		{
			return;
		}
		int NumOfLandM = list.at(0).toInt();
		for (int ncrntLandM = 0; ncrntLandM < NumOfLandM; ncrntLandM++)
		{
			structLandmark* crntLandM = new structLandmark;
			if (StringList.at(ncrntLine++) != QString("#LANDMARK LIST NUMBER ") + 
				QString::number(ncrntLandM) + QString(" DEFINITION:\n"))
			{
				return;
			}
			// 2.4.1 landmark_index
			list = StringList.at(ncrntLine++).split(" # ");
			if (list.at(1) != QString("landmark_index\n"))
			{
				return;
			}
			if (list.at(0).toInt() != ncrntLandM)
			{
				return;
			}
			// 2.4.2 contour_ViewLabel
			list = StringList.at(ncrntLine++).split(" # ");
			if (list.at(1) != QString("landmark_name\n"))
			{
				return;
			}
			crntLandM->qsName = list.at(0);
			// 2.4.4 landmark_ID
			list = StringList.at(ncrntLine++).split(" # ");
			if (list.at(1) != QString("landmark_ID\n"))
			{
				return;
			}
			crntLandM->LandmarkID = list.at(0).toInt();
			// 2.4.5 Color
			list = StringList.at(ncrntLine++).split(" # ");
			if (list.at(1) != QString("r_color g_color b_color\n"))
			{
				return;
			}
			list2 = list.at(0).split(" ");
			QColor color(list2.at(0).toInt(), list2.at(1).toInt(), list2.at(2).toInt());
			crntLandM->color = color;
			// 2.5.6 Point
			if (StringList.at(ncrntLine++) != QString("#POINT_POS\n"))
			{
				return;
			}
			list = StringList.at(ncrntLine++).split(" ");
			if (list.size() != 3)
			{
				return;
			}
			crntLandM->dPosition[0] = list.at(0).toDouble();
			crntLandM->dPosition[1] = list.at(1).toDouble();
			crntLandM->dPosition[2] = list.at(2).toDouble();
			if (StringList.at(ncrntLine++) != QString("#END_POINT_POS\n"))
			{
				return;
			}
			crntLandMData->AppandLandmark(crntLandM);
		}
	}
}
