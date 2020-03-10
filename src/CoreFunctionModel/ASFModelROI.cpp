#include "ASFModelROI.h"
#include "ASPipelineManager.h"
#include "ASPipelineROICube.h"
#include "ASVisualizationManager2D.h"
#include "ASROIData.h"
#include "ASROITypeBase.h"
#include "ASROITypeCube.h"
#include "ASDataTree.h"
#include "ASTransDataGeneral.h"
#include "ASArrayImageData.h"
#include "ASAnnotation.h"
#include "ASMainWidget.h"
#include "ASTransAddDeleteData.h"
#include <QMessageBox>

ASFModelROI* ASFModelROI::ms_SelfPointer = nullptr;

ASFModelROI::ASFModelROI(QObject *parent)
	: ASFunctionModelBase(parent)
{
	ms_SelfPointer = this;

}

ASFModelROI::~ASFModelROI()
{

}

// 没建立管线就建立管线
void ASFModelROI::InitROICubePipeline()
{
	if (m_ROICubePipeLine == nullptr)
	{
		m_ROICubePipeLine = static_cast<ASPipelineROICube*>(
			ASPipelineManager::AddPipeline(PipeLineType_ROICube));
	}
	m_ROICubePipeLine->Update2DTransformMatrix();
}
// 刷新ROICube显示
void ASFModelROI::RefreshROICubeDisplay()
{
	if (m_ROICubePipeLine == nullptr)
	{
		m_ROICubePipeLine = static_cast<ASPipelineROICube*>(
			ASPipelineManager::AddPipeline(PipeLineType_ROICube));
	}
	m_ROICubePipeLine->Update2DTransformMatrix();
	ASVisualizationManager2D::Render();
}
// 读取ROI数据
void ASFModelROI::LoadROIData(const QString c_FileName)
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
	// 判断是否需要新建ROI文件
	ASROIData* ROIData = static_cast<ASROIData*>(AnnotationData->getChild(DataType_ROI));
	if (!ROIData)
	{
		ROIData = new ASROIData();
		ROIData->setName(tr("Bounding box data"));
		ROIData->setShown(true);
		ASDataTree::NewDataNode(ROIData, AnnotationData);
		ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_ROI);
		if (!ROIData)
		{
			qDebug("Error: Null pointer of current ROIData. From ASROIWidget(2).");
			return;
		}
	}
	// 读取数据
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
	if (StringList.at(ncrntLine++) != QString("#Anatomy Sketch ROI File\n"))
	{
		return;
	}
	// 1 ROI数量
	if (StringList.at(ncrntLine++) != QString("#NUMBER OF ROIs:\n"))
	{
		return;
	}
	int NumOfROI = StringList.at(ncrntLine++).toInt();
	// 2 ROI LIST
	if (StringList.at(ncrntLine++) != QString("#ROI LIST DEFINITION:\n"))
	{
		return;
	}
	for (int ncrntROI = 0; ncrntROI < NumOfROI; ncrntROI++)
	{
		// 2.1 DATA LIST NUMBER
		if (StringList.at(ncrntLine++) != QString("#ROI LIST NUMBER ") +
			QString::number(ncrntROI) + QString(" DEFINITION:\n"))
		{
			return;
		}
		// 2.2 data_list_index
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("data_list_index\n"))
		{
			return;
		}
		if (list.at(0).toInt() != ncrntROI)
		{
			return;
		}
		// 2.3 data_name
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("roi_name\n"))
		{
			return;
		}
		QString roiname = list.at(0);
		// 2.4 Color
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("r_color g_color b_color\n"))
		{
			return;
		}
		list2 = list.at(0).split(" ");
		QColor color(list2.at(0).toInt(), list2.at(1).toInt(), list2.at(2).toInt());
		// 2.5 Type
		list = StringList.at(ncrntLine++).split(" # ");
		if (list.at(1) != QString("roi_type\n"))
		{
			return;
		}
		int roitype = list.at(0).toInt();

		ASROITypeBase* roi = ROIData->NewROI(roiname, roitype);
		switch (roitype)
		{
		case ROIType_Cube:
		{
			ASROITypeCube* cube = static_cast<ASROITypeCube*>(roi);
			cube->SetName(roiname);
			cube->SetColor(color);
			cube->SetROIID(ncrntROI);
			if (StringList.at(ncrntLine++) != QString("#ROI Cube DEFINITION:\n"))
			{
				return;
			}
			list = StringList.at(ncrntLine++).split(" # ");
			if (list.at(1) != QString("Range\n"))
			{
				return;
			}
			list2 = list.at(0).split(" ");
			double range[6];
			for (int i = 0; i < 6; i++)
			{
				range[i] = list2.at(i).toDouble();
			}
			cube->SetRange(range);		
			if (StringList.at(ncrntLine++) != QString("#ROI Cube END:\n"))
			{
				return;
			}
			break;
		}
		default:
			break;
		}
	}
}