#include "ASFModelPolyDeformation.h"
#include "ASAbstractData.h"
#include "ASDataStructureWalker.h"
#include "ASPolyImageData.h"
#include "GlobelDeclaration.h"
#include <QMessageBox>

#pragma comment(lib,"FreeFormDeformation.lib")
extern "C" __declspec(dllimport)  float FreeFormDeformation(int argc, void* argv[]);

ASFModelPolyDeformation* ASFModelPolyDeformation::ms_SelfPointer = nullptr;

ASFModelPolyDeformation::ASFModelPolyDeformation(QObject *parent)
	: ASFunctionModelBase(parent)
{
	ms_SelfPointer = this;

	m_listPolyDeformationTemporaryData = new QList<structPolyDeformationTemporaryData*>;
}

ASFModelPolyDeformation::~ASFModelPolyDeformation()
{

}
void ASFModelPolyDeformation::SetDataNodeChoosen(const int c_ID, const bool c_Choosen)
{
	if (c_Choosen)
	{
		// 变为选中
		m_listChoosenID.removeOne(c_ID); // 避免重复
		m_listChoosenID.append(c_ID);
	}
	else
	{
		// 变为未选中
		m_listChoosenID.removeOne(c_ID);
	}
}
void ASFModelPolyDeformation::ResetDataNodeChoosen(QList<int> choosenID)
{
	m_listChoosenID = choosenID;
}
bool ASFModelPolyDeformation::IsPolyChoosen(const int c_ID)
{
	if (ms_SelfPointer->m_listChoosenID.indexOf(c_ID) < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
// 当前变换
int ASFModelPolyDeformation::GetCurrentTransType()
{
	return m_CurrentTransType;
}
// 开始变换
void ASFModelPolyDeformation::StartPolyDeformation(double* Pos, const int c_Type, double ViewRange)
{
	if (Pos == nullptr)
	{
		m_adPolyDeformationPos[0] = 0;
		m_adPolyDeformationPos[1] = 0;
		m_adPolyDeformationPos[2] = 0;
	}
	else
	{
		m_adPolyDeformationPos[0] = Pos[0];
		m_adPolyDeformationPos[1] = Pos[1];
		m_adPolyDeformationPos[2] = Pos[2];
	}
	m_dViewRange = ViewRange;
	m_FFDRadius = m_dViewRange / 6;
	// 遍历面数据，计算被影响数据的数量
	if (m_listChoosenID.size() == 0)
	{
		return;
	}
	m_CurrentTransType = c_Type;

	// 调整缓存数据
	if (m_listPolyDeformationTemporaryData->size() >= MaximumWithdrawalSteps)
	{
		structPolyDeformationTemporaryData* crntTemporaryData = m_listPolyDeformationTemporaryData->at(0);
		m_listPolyDeformationTemporaryData->removeAt(0);
		QList<structTemporaryPolyDataSet*>* DataSet = crntTemporaryData->pTemporaryDataSet;
		foreach(structTemporaryPolyDataSet* var, *DataSet)
		{
			DataSet->removeOne(var);
			var->pPolyData = NULL;
			delete var;
		}
		delete crntTemporaryData->pTemporaryDataSet;
		delete crntTemporaryData;
	}

	structPolyDeformationTemporaryData* crntDataSet = new structPolyDeformationTemporaryData;
	crntDataSet->DeformaitonType = c_Type;
	crntDataSet->pTemporaryDataSet = new QList<structTemporaryPolyDataSet*>;
	m_listPolyDeformationTemporaryData->append(crntDataSet);

	// 遍历数据，记录新的缓存数据
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_PolyImageData); item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_PolyImageData))
	{
		ASPolyImageData* crntDataNode = (ASPolyImageData*)item;
		if (m_listChoosenID.indexOf(crntDataNode->getDataNodeID()) >= 0)
		{
			if (crntDataNode->isBeingProcess())
			{
				QMessageBox::information(0, QObject::tr("information"), QObject::tr("Data is processing."), QMessageBox::Ok);
				return;
			}
			structTemporaryPolyDataSet* crntFFDTemporaryData = new structTemporaryPolyDataSet;
			crntFFDTemporaryData->ID = crntDataNode->getDataNodeID();
			crntFFDTemporaryData->pPolyData = vtkSmartPointer<vtkPolyData>::New();
			crntFFDTemporaryData->pPolyData->DeepCopy(crntDataNode->getPolyImageData());
			crntDataSet->pTemporaryDataSet->append(crntFFDTemporaryData);
		}
	}

	//如果是旋转或缩放，计算重心。
	if (c_Type == PolyDeformation_Rotation || c_Type == PolyDeformation_Zooming)
	{
		QList<structTemporaryPolyDataSet*>* plistTemporaryData = m_listPolyDeformationTemporaryData->last()->pTemporaryDataSet;
		int numOfList = plistTemporaryData->size();
		int numOfAllPoints = 0;
		m_PolyDeformationDataCenter[0] = 0;
		m_PolyDeformationDataCenter[1] = 0;
		m_PolyDeformationDataCenter[2] = 0;
		for (int ncrnt = 0; ncrnt < numOfList; ncrnt++)
		{
			structTemporaryPolyDataSet* crntDataSet = plistTemporaryData->at(ncrnt);
			int numOfPoints = crntDataSet->pPolyData->GetPoints()->GetNumberOfPoints();
			for (int crntPoint = 0; crntPoint < numOfPoints; crntPoint++)
			{
				double p[3];
				crntDataSet->pPolyData->GetPoint(crntPoint, p);
				m_PolyDeformationDataCenter[0] += p[0];
				m_PolyDeformationDataCenter[1] += p[1];
				m_PolyDeformationDataCenter[2] += p[2];
			}
			numOfAllPoints += numOfPoints;
		}
		m_PolyDeformationDataCenter[0] /= numOfAllPoints;
		m_PolyDeformationDataCenter[1] /= numOfAllPoints;
		m_PolyDeformationDataCenter[2] /= numOfAllPoints;
	}
}
// 继续变换
int ASFModelPolyDeformation::ContinuePolyDeformation(double* Pos, const int c_Viewlabel, const int c_Type)
{
	m_adPolyDeformationPos[3] = Pos[0];
	m_adPolyDeformationPos[4] = Pos[1];
	m_adPolyDeformationPos[5] = Pos[2];

	switch (c_Type)
	{
	case PolyDeformation_None:
		break;
	case PolyDeformation_Panning:
		return this->PanningDeformation();
		break;
	case PolyDeformation_Rotation:
		return this->RotationDeformation(c_Viewlabel);
		break;
	case PolyDeformation_Zooming:
		break;
	case PolyDeformation_FFD:
		return this->FFDDeformation();
		break;
	default:
		break;
	}
	return -1;
}
// 缩放
void ASFModelPolyDeformation::ContinueZoom(const double c_mX, const double c_mY, const double c_mZ)
{
	if (m_listPolyDeformationTemporaryData->size() == 0)
	{
		return;
	}

	//根据倍数变换点坐标
	QList<structTemporaryPolyDataSet*>* plistTemporaryData = m_listPolyDeformationTemporaryData->last()->pTemporaryDataSet;
	if (plistTemporaryData == NULL)
	{
		return;
	}
	int numOfTemporaryData = plistTemporaryData->size();
	for (int ncrntPanningPolyData = 0; ncrntPanningPolyData < numOfTemporaryData; ncrntPanningPolyData++)
	{
		structTemporaryPolyDataSet* TemporaryData = plistTemporaryData->at(ncrntPanningPolyData);
		vtkPoints* pcrntPoints = TemporaryData->pPolyData->GetPoints();
		ASPolyImageData* crntDataNode = (ASPolyImageData*)ASAbstractData::getDataNodeFromID(TemporaryData->ID);
		if (crntDataNode == nullptr)
		{
			continue;
		}
		vtkPoints* pcrntResultPoints = crntDataNode->getPolyImageData()->GetPoints();
		int numOfPoints = pcrntPoints->GetNumberOfPoints();
		for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
		{
			double p[3];
			pcrntPoints->GetPoint(ncrntPoint, p);
			p[0] = m_PolyDeformationDataCenter[0] + (p[0] - m_PolyDeformationDataCenter[0]) * c_mX;
			p[1] = m_PolyDeformationDataCenter[1] + (p[1] - m_PolyDeformationDataCenter[1]) * c_mY;
			p[2] = m_PolyDeformationDataCenter[2] + (p[2] - m_PolyDeformationDataCenter[2]) * c_mZ;
			pcrntResultPoints->SetPoint(ncrntPoint, p);
		}
		crntDataNode->getPolyImageData()->Modified();
	}
}
// 旋转
void ASFModelPolyDeformation::ContinueRotation(const double angle, const int viewlabel)
{
	if (m_listPolyDeformationTemporaryData->size() == 0)
	{
		return;
	}
	QList<structTemporaryPolyDataSet*>* plistTemporaryData = m_listPolyDeformationTemporaryData->last()->pTemporaryDataSet;
	if (plistTemporaryData == NULL)
	{
		return;
	}
	double theta = angle;
	// 根据旋转角度变换点坐标
	int numOfTemporaryData = plistTemporaryData->size();
	for (int ncrntPanningPolyData = 0; ncrntPanningPolyData < numOfTemporaryData; ncrntPanningPolyData++)
	{
		structTemporaryPolyDataSet* TemporaryData = plistTemporaryData->at(ncrntPanningPolyData);
		vtkPoints* pcrntPoints = TemporaryData->pPolyData->GetPoints();
		ASPolyImageData* crntDataNode = (ASPolyImageData*)ASAbstractData::getDataNodeFromID(TemporaryData->ID);
		if (crntDataNode == nullptr)
		{
			continue;
		}
		vtkPoints* pcrntResultPoints = crntDataNode->getPolyImageData()->GetPoints();
		int numOfPoints = pcrntPoints->GetNumberOfPoints();
		switch (viewlabel)
		{
		case ViewLabel_Sagittal:
		{
			for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
			{
				double p[3];
				pcrntPoints->GetPoint(ncrntPoint, p);
				double x = p[1] - m_PolyDeformationDataCenter[1];
				double y = p[2] - m_PolyDeformationDataCenter[2];
				double theta0 = atan2(x, y) - theta;
				double lenth = sqrt(x * x + y * y);
				x = lenth * sin(theta0);
				y = lenth * cos(theta0);
				p[1] = x + m_PolyDeformationDataCenter[1];
				p[2] = y + m_PolyDeformationDataCenter[2];
				pcrntResultPoints->SetPoint(ncrntPoint, p);
			}
		}break;
		case ViewLabel_Coronal:
		{
			for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
			{
				double p[3];
				pcrntPoints->GetPoint(ncrntPoint, p);
				double x = p[0] - m_PolyDeformationDataCenter[0];
				double y = p[2] - m_PolyDeformationDataCenter[2];
				double theta0 = atan2(x, y) - theta;
				double lenth = sqrt(x * x + y * y);
				x = lenth * sin(theta0);
				y = lenth * cos(theta0);
				p[0] = x + m_PolyDeformationDataCenter[0];
				p[2] = y + m_PolyDeformationDataCenter[2];
				pcrntResultPoints->SetPoint(ncrntPoint, p);
			}
		}break;
		case ViewLabel_Axial:
		{
			for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
			{
				double p[3];
				pcrntPoints->GetPoint(ncrntPoint, p);
				double x = p[0] - m_PolyDeformationDataCenter[0];
				double y = p[1] - m_PolyDeformationDataCenter[1];
				double theta0 = atan2(x, y) - theta;
				double lenth = sqrt(x * x + y * y);
				x = lenth * sin(theta0);
				y = lenth * cos(theta0);
				p[0] = x + m_PolyDeformationDataCenter[0];
				p[1] = y + m_PolyDeformationDataCenter[1];
				pcrntResultPoints->SetPoint(ncrntPoint, p);
			}
		}break;
		default:
			break;
		}
		crntDataNode->getPolyImageData()->Modified();
	}
}
// 改变FFD影响半径
int ASFModelPolyDeformation::FFDRadiusChange(int ZoominOrZoomout)
{
	//改变半径
	if (ZoominOrZoomout == 0)
	{
		m_FFDRadius *= 1.05;
	}
	else
	{
		m_FFDRadius /= 1.05;
	}
	return this->FFDDeformation();
}
// 撤销
int ASFModelPolyDeformation::UndoDef()
{
	if (m_listPolyDeformationTemporaryData->size() == 0)
	{
		return -1;
	}
	structPolyDeformationTemporaryData* crntTemporaryData = m_listPolyDeformationTemporaryData->last();
	int numOfFFDTemporaryData = crntTemporaryData->pTemporaryDataSet->size();
	for (int ncrntFFDTemporaryData = 0; ncrntFFDTemporaryData < numOfFFDTemporaryData; ncrntFFDTemporaryData++)
	{
		structTemporaryPolyDataSet* TemporaryData = crntTemporaryData->pTemporaryDataSet->at(ncrntFFDTemporaryData);
		ASPolyImageData* pCorrespondPolyData = (ASPolyImageData*)ASAbstractData::getDataNodeFromID(TemporaryData->ID);
		if (pCorrespondPolyData == nullptr)
		{
			continue;
		}
		pCorrespondPolyData->getPolyImageData()->DeepCopy(TemporaryData->pPolyData);				
	}

	//删除用于撤销的临时数据
	structPolyDeformationTemporaryData* deleteTemporaryData = m_listPolyDeformationTemporaryData->last();
	m_listPolyDeformationTemporaryData->removeLast();
	QList<structTemporaryPolyDataSet*>* DataSet = deleteTemporaryData->pTemporaryDataSet;
	foreach(structTemporaryPolyDataSet* var, *DataSet)
	{
		DataSet->removeOne(var);
		var->pPolyData = NULL;
		delete var;
	}
	delete deleteTemporaryData->pTemporaryDataSet;
	delete deleteTemporaryData;
	return 0;
}

// 进行平移变换
int ASFModelPolyDeformation::PanningDeformation()
{
	if (m_listPolyDeformationTemporaryData->size() == 0)
	{
		return -1;
	}

	// 计算参数delta
	double delta[3];
	delta[0] = m_adPolyDeformationPos[3] - m_adPolyDeformationPos[0];
	delta[1] = m_adPolyDeformationPos[4] - m_adPolyDeformationPos[1];
	delta[2] = m_adPolyDeformationPos[5] - m_adPolyDeformationPos[2];

	QList<structTemporaryPolyDataSet*>* plistTemporaryData = m_listPolyDeformationTemporaryData->last()->pTemporaryDataSet;
	if (plistTemporaryData == NULL)
	{
		return -1;
	}
	int numOfTemporaryData = plistTemporaryData->size();
	for (int ncrntPanningPolyData = 0; ncrntPanningPolyData < numOfTemporaryData; ncrntPanningPolyData++)
	{
		structTemporaryPolyDataSet* TemporaryData = plistTemporaryData->at(ncrntPanningPolyData);
		vtkPoints* pcrntPoints = TemporaryData->pPolyData->GetPoints();
		ASPolyImageData* crntDataNode = (ASPolyImageData*)ASAbstractData::getDataNodeFromID(TemporaryData->ID);
		if (crntDataNode == nullptr)
		{
			continue;
		}
		vtkPoints* pcrntResultPoints = crntDataNode->getPolyImageData()->GetPoints();
		int numOfPoints = pcrntPoints->GetNumberOfPoints();
		for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
		{
			double p[3];
			pcrntPoints->GetPoint(ncrntPoint, p);
			p[0] += delta[0];
			p[1] += delta[1];
			p[2] += delta[2];
			pcrntResultPoints->SetPoint(ncrntPoint, p);
		}
		crntDataNode->getPolyImageData()->Modified();
	}
	return 0;
}
// 进行旋转变换，鼠标交互
int ASFModelPolyDeformation::RotationDeformation(int viewlabel)
{
	if (m_listPolyDeformationTemporaryData->size() == 0)
	{
		return -1;
	}
	QList<structTemporaryPolyDataSet*>* plistTemporaryData = m_listPolyDeformationTemporaryData->last()->pTemporaryDataSet;
	if (plistTemporaryData == NULL)
	{
		return -1;
	}
	// 计算旋转的角度
	double theta;
	double x1, y1, x2, y2;
	switch (viewlabel)
	{
	case ViewLabel_Sagittal:
		x1 = m_adPolyDeformationPos[1] - m_PolyDeformationDataCenter[1];
		y1 = m_adPolyDeformationPos[2] - m_PolyDeformationDataCenter[2];
		x2 = m_adPolyDeformationPos[4] - m_PolyDeformationDataCenter[1];
		y2 = m_adPolyDeformationPos[5] - m_PolyDeformationDataCenter[2];
		break;
	case ViewLabel_Coronal:
		x1 = m_adPolyDeformationPos[0] - m_PolyDeformationDataCenter[0];
		y1 = m_adPolyDeformationPos[2] - m_PolyDeformationDataCenter[2];
		x2 = m_adPolyDeformationPos[3] - m_PolyDeformationDataCenter[0];
		y2 = m_adPolyDeformationPos[5] - m_PolyDeformationDataCenter[2];
		break;
	case ViewLabel_Axial:
		x1 = m_adPolyDeformationPos[0] - m_PolyDeformationDataCenter[0];
		y1 = m_adPolyDeformationPos[1] - m_PolyDeformationDataCenter[1];
		x2 = m_adPolyDeformationPos[3] - m_PolyDeformationDataCenter[0];
		y2 = m_adPolyDeformationPos[4] - m_PolyDeformationDataCenter[1];
		break;
	default:
		break;
	}
	theta = atan2(x1, y1) - atan2(x2, y2);
	if (theta > CV_PI)
		theta -= 2 * CV_PI;
	if (theta < -CV_PI)
		theta += 2 * CV_PI;

	// 根据旋转角度变换点坐标
	int numOfTemporaryData = plistTemporaryData->size();
	for (int ncrntPanningPolyData = 0; ncrntPanningPolyData < numOfTemporaryData; ncrntPanningPolyData++)
	{
		structTemporaryPolyDataSet* TemporaryData = plistTemporaryData->at(ncrntPanningPolyData);
		vtkPoints* pcrntPoints = TemporaryData->pPolyData->GetPoints();
		ASPolyImageData* crntDataNode = (ASPolyImageData*)ASAbstractData::getDataNodeFromID(TemporaryData->ID);
		if (crntDataNode == nullptr)
		{
			continue;
		}
		vtkPoints* pcrntResultPoints = crntDataNode->getPolyImageData()->GetPoints();
		int numOfPoints = pcrntPoints->GetNumberOfPoints();
		switch (viewlabel)
		{
		case ViewLabel_Sagittal:
		{
			for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
			{
				double p[3];
				pcrntPoints->GetPoint(ncrntPoint, p);
				double x = p[1] - m_PolyDeformationDataCenter[1];
				double y = p[2] - m_PolyDeformationDataCenter[2];
				double theta0 = atan2(x, y) - theta;
				double lenth = sqrt(x * x + y * y);
				x = lenth * sin(theta0);
				y = lenth * cos(theta0);
				p[1] = x + m_PolyDeformationDataCenter[1];
				p[2] = y + m_PolyDeformationDataCenter[2];
				pcrntResultPoints->SetPoint(ncrntPoint, p);
			}
		}break;
		case ViewLabel_Coronal:
		{
			for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
			{
				double p[3];
				pcrntPoints->GetPoint(ncrntPoint, p);
				double x = p[0] - m_PolyDeformationDataCenter[0];
				double y = p[2] - m_PolyDeformationDataCenter[2];
				double theta0 = atan2(x, y) - theta;
				double lenth = sqrt(x * x + y * y);
				x = lenth * sin(theta0);
				y = lenth * cos(theta0);
				p[0] = x + m_PolyDeformationDataCenter[0];
				p[2] = y + m_PolyDeformationDataCenter[2];
				pcrntResultPoints->SetPoint(ncrntPoint, p);
			}
		}break;
		case ViewLabel_Axial:
		{
			for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
			{
				double p[3];
				pcrntPoints->GetPoint(ncrntPoint, p);
				double x = p[0] - m_PolyDeformationDataCenter[0];
				double y = p[1] - m_PolyDeformationDataCenter[1];
				double theta0 = atan2(x, y) - theta;
				double lenth = sqrt(x * x + y * y);
				x = lenth * sin(theta0);
				y = lenth * cos(theta0);
				p[0] = x + m_PolyDeformationDataCenter[0];
				p[1] = y + m_PolyDeformationDataCenter[1];
				pcrntResultPoints->SetPoint(ncrntPoint, p);
			}
		}break;
		default:
			break;
		}
		crntDataNode->getPolyImageData()->Modified();
	}
	return 0;
}
// 进行FFD变换
int ASFModelPolyDeformation::FFDDeformation()
{
	if (m_listPolyDeformationTemporaryData->size() == 0)
	{
		return -1;
	}
	//数据整理，更新FFD操作记录。
	int nTotalNumberOfPoints = 0;
	QList<structTemporaryPolyDataSet*>* plistFFDTemporaryData = m_listPolyDeformationTemporaryData->last()->pTemporaryDataSet;
	if (plistFFDTemporaryData == NULL)
	{
		return -1;
	}
	int numOfFFDTemporaryData = plistFFDTemporaryData->size();
	if (numOfFFDTemporaryData == 0)
	{
		return -1;
	}
	for (int ncrntFFDTemporaryData = 0; ncrntFFDTemporaryData < numOfFFDTemporaryData; ncrntFFDTemporaryData++)
	{
		structTemporaryPolyDataSet* FFDTemporaryData = plistFFDTemporaryData->at(ncrntFFDTemporaryData);
		nTotalNumberOfPoints += FFDTemporaryData->pPolyData->GetPoints()->GetNumberOfPoints();
	}
	//构建顶点坐标矩阵
	double* pfArrayOfPoints = (double*)malloc(sizeof(double) * 3 * nTotalNumberOfPoints);
	int itempoints = 0;
	for (int ncrntFFDPolyData = 0; ncrntFFDPolyData < numOfFFDTemporaryData; ncrntFFDPolyData++)
	{
		structTemporaryPolyDataSet* FFDTemporaryData = plistFFDTemporaryData->at(ncrntFFDPolyData);
		vtkPoints* pcrntPoints = FFDTemporaryData->pPolyData->GetPoints();
		int numOfPoints = pcrntPoints->GetNumberOfPoints();
		for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
		{
			double* p = pcrntPoints->GetPoint(ncrntPoint);
			pfArrayOfPoints[itempoints * 3] = p[0];
			pfArrayOfPoints[itempoints * 3 + 1] = p[1];
			pfArrayOfPoints[itempoints * 3 + 2] = p[2];
			itempoints++;
		}
	}
	//其他参数
	int numOfMovePos = 1;
	double RangeOfCtrlPoints[6];
	double doffset = m_FFDRadius * 2;
	RangeOfCtrlPoints[0] = m_adPolyDeformationPos[0] - doffset;
	RangeOfCtrlPoints[1] = m_adPolyDeformationPos[1] - doffset;
	RangeOfCtrlPoints[2] = m_adPolyDeformationPos[2] - doffset;
	RangeOfCtrlPoints[3] = m_adPolyDeformationPos[0] + doffset;
	RangeOfCtrlPoints[4] = m_adPolyDeformationPos[1] + doffset;
	RangeOfCtrlPoints[5] = m_adPolyDeformationPos[2] + doffset;
	int numOfCtrlPoints[3] = { 10, 10, 10 };
	//调用dll
	void* argv[6];
	argv[0] = pfArrayOfPoints;
	argv[1] = &itempoints;
	argv[2] = m_adPolyDeformationPos;
	argv[3] = &numOfMovePos;
	argv[4] = RangeOfCtrlPoints;
	argv[5] = numOfCtrlPoints;
	float re = FreeFormDeformation(6, argv);
	if (re == 1.0)
	{
		itempoints = 0;
		for (int ncrntFFDPolyData = 0; ncrntFFDPolyData < numOfFFDTemporaryData; ncrntFFDPolyData++)
		{
			structTemporaryPolyDataSet* FFDTemporaryData = plistFFDTemporaryData->at(ncrntFFDPolyData);
			ASPolyImageData* crntDataNode = (ASPolyImageData*)ASAbstractData::getDataNodeFromID(FFDTemporaryData->ID);
			if (crntDataNode == nullptr)
			{
				continue;
			}
			vtkPoints* pcrntPoints = crntDataNode->getPolyImageData()->GetPoints();
			int numOfPoints = pcrntPoints->GetNumberOfPoints();
			for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
			{
				pcrntPoints->SetPoint(ncrntPoint, &(pfArrayOfPoints[itempoints * 3]));
				itempoints++;
			}
			crntDataNode->getPolyImageData()->Modified();
		}
	}
	free(pfArrayOfPoints);
	return 0;
}