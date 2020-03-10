#include "ASContourData.h"
#include "ASTransContour.h"

int ASContourData::ms_nNumOfContourData = 0;

ASContourData::ASContourData()
{
	m_Datatype = DataType_ContourData;

	if (ASContourData::ms_nNumOfContourData == 0)
	{
		// 建立轮廓线显示管线
		ASTransContour::InitContourPipeline();
	}
	ASContourData::ms_nNumOfContourData += 1;

	m_listContours = new QList<structContour*>;
}

ASContourData::~ASContourData()
{
	ASContourData::ms_nNumOfContourData -= 1;

	while (m_listContours->size())
	{
		structContour* crntContour = m_listContours->first();
		m_listContours->removeFirst();
		while (crntContour->pListCurves->size())
		{
			structCurve* crntCurve = crntContour->pListCurves->first();
			crntContour->pListCurves->removeFirst();
			while (crntCurve->pvectorPoint.size())
			{
				structPoint* crntPoint = crntCurve->pvectorPoint.first();
				crntCurve->pvectorPoint.removeFirst();
				delete crntPoint;
			}
			delete crntCurve;
		}
		delete crntContour->pListCurves;
		delete crntContour;
	}
	delete m_listContours;
}

// 调用此函数来释放节点
void ASContourData::DeleteNode()
{
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASContourData::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_ContourData || ASMarkData::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASContourData::isTypeAbsolute(const int c_Type)
{
	if (c_Type == DataType_ContourData)
	{
		return true;
	}
	return false;
}
bool ASContourData::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_ContourData || ASMarkData::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}
// 新建轮廓线
int ASContourData::NewContour(const QString c_name, const int c_lable, const QColor c_color)
{
	int Label = c_lable;
	QColor crntColor = c_color;
	int numOfContours = m_listContours->size();
	if (Label == -1)
	{
		// 自动选择label和颜色
		bool f;
		do
		{
			Label++;
			f = false;
			for (int i = 0; i < numOfContours; i++)
			{
				if (m_listContours->at(i)->label == Label)
				{
					f = true;
					break;
				}
			}
		} while (f);
		crntColor.setRed(qrand() % 256);
		crntColor.setGreen(qrand() % 256);
		crntColor.setBlue(qrand() % 256);
	}

	for (int i = 0; i < numOfContours; i++)
	{
		if (m_listContours->at(i)->label == c_lable || m_listContours->at(i)->name == c_name)
		{
			return -1;
		}
	}
	structContour* crntContour = new structContour;
	crntContour->color = crntColor;
	crntContour->label = Label;
	crntContour->name = c_name;
	crntContour->contourID = GetNewContourID();
	crntContour->isShown = true;
	crntContour->pListCurves = new QList<structCurve *>;
	m_listContours->append(crntContour);
	return crntContour->contourID;
}
// 轮廓线数据数量
int ASContourData::GetNumOfContourData()
{
	return ASContourData::ms_nNumOfContourData;
}
// 标签值
int ASContourData::getLabel()
{
	return m_nLabel;
}
void ASContourData::setLabel(const int label)
{
	m_nLabel = label;
}
// 访问轮廓线
int ASContourData::GetNumberOfContour()
{
	if (m_listContours)
	{
		return m_listContours->size();
	}
	else
	{
		return 0;
	}
}
structContour* ASContourData::GetContourAt(const int c_Contour)
{
	return m_listContours->at(c_Contour);
}
structContour* ASContourData::GetContourFromID(const int c_ContourID)
{
	int numOfContour = m_listContours->size();
	for (int i = 0; i < numOfContour; i++)
	{
		if (m_listContours->at(i)->contourID == c_ContourID)
		{
			return m_listContours->at(i);
		}
	}
	return nullptr;
}
void ASContourData::AppandContour(structContour* NewContour)
{
	m_listContours->append(NewContour);
}
// 得到数据范围，double[6]
double* ASContourData::getDataExtent()
{
	return nullptr;
}

// 轮廓线操作
void ASContourData::AddContrlPointsTemp(double* pPosition, const int ViewLabel, 
	double* pSpacing, const int c_ContourID)
{
	//计算当前层的范围，对该范围内数据认为同层
	double layermin = pPosition[ViewLabel] - 0.5 * pSpacing[ViewLabel];
	double layermax = pPosition[ViewLabel] + 0.5 * pSpacing[ViewLabel];
	structContour* crntContour = this->GetContourFromID(c_ContourID);
	if (!crntContour)
	{
		return;
	}
	// 1 判断当前层面是否有未闭合的轮廓
	int unClosureContour = -1;
	// 遍历轮廓
	int numOfCurve = crntContour->pListCurves->size();
	for (int ncrntContour = 0; ncrntContour < numOfCurve; ncrntContour++)
	{
		double crntdLayer = crntContour->pListCurves->at(ncrntContour)->dLayer;
		if (crntdLayer >= layermin && crntdLayer < layermax)
		{
			// 是当前层的，判断是否闭合
			if (crntContour->pListCurves->at(ncrntContour)->isClosure == false)
			{
				unClosureContour = ncrntContour;
				break;
			}
		}
	}

	// 2 新建控制点
	structPoint* crntpoint = new structPoint;
	crntpoint->dX = pPosition[0];
	crntpoint->dY = pPosition[1];
	crntpoint->dZ = pPosition[2];

	if (unClosureContour == -1)
	{
		// 2.1 如果没有未闭合轮廓，则新建一个轮廓
		structCurve* newContour = new structCurve;
		newContour->dLayer = pPosition[ViewLabel];
		newContour->isClosure = false;
		newContour->ViewLabel = ViewLabel;
		newContour->pvectorPoint.append(crntpoint);
		newContour->haveTemporary = true;
		crntContour->pListCurves->append(newContour);
	}
	else
	{
		// 2.2 如果有未闭合轮廓，直接添加一个控制点
		if (crntContour->pListCurves->at(unClosureContour)->haveTemporary)
		{
			// 有临时控制点则删除原临时控制点
			delete crntContour->pListCurves->at(unClosureContour)->pvectorPoint.last();
			crntContour->pListCurves->at(unClosureContour)->pvectorPoint.removeLast();
		}
		crntContour->pListCurves->at(unClosureContour)->haveTemporary = true;
		crntContour->pListCurves->at(unClosureContour)->pvectorPoint.append(crntpoint);
	}
}
void ASContourData::AddContrlPoints(double* pPosition, const int ViewLabel, 
	double* pSpacing, const int c_ContourID)
{
	//计算当前层的范围，对该范围内数据认为同层
	double layermin = pPosition[ViewLabel] - 0.5 * pSpacing[ViewLabel];
	double layermax = pPosition[ViewLabel] + 0.5 * pSpacing[ViewLabel];
	structContour* crntContour = this->GetContourFromID(c_ContourID);
	if (!crntContour)
	{
		return;
	}
	// 1 判断当前层面是否有未闭合的轮廓
	int unClosureContour = -1;
	// 遍历轮廓
	int numOfCurve = crntContour->pListCurves->size();
	for (int ncrntContour = 0; ncrntContour < numOfCurve; ncrntContour++)
	{
		double crntdLayer = crntContour->pListCurves->at(ncrntContour)->dLayer;
		if (crntdLayer >= layermin && crntdLayer < layermax)
		{
			// 是当前层的，判断是否闭合
			if (crntContour->pListCurves->at(ncrntContour)->isClosure == false)
			{
				unClosureContour = ncrntContour;
				break;
			}
		}
	}
	if (unClosureContour == -1)
	{
		// 2.1 如果没有未闭合轮廓
		return;
	}
	else
	{
		// 2.2 如果有未闭合轮廓，直接添加一个控制点
		if (crntContour->pListCurves->at(unClosureContour)->haveTemporary)
		{
			structPoint* crntpoint = new structPoint;
			crntpoint->dX = pPosition[0];
			crntpoint->dY = pPosition[1];
			crntpoint->dZ = pPosition[2];
			crntContour->pListCurves->at(unClosureContour)->pvectorPoint.append(crntpoint);
		}
	}
}
void ASContourData::MoveContrlPoints(double* pPosition, const int c_ViewLabel, double* pSpacing, 
	const int c_ContourID,	const int c_IndexOfContour, const int c_IndexOfHandle)
{
	//计算当前层的范围，对该范围内数据认为同层
	double layermin = pPosition[c_ViewLabel] - 0.5 * pSpacing[c_ViewLabel];
	double layermax = pPosition[c_ViewLabel] + 0.5 * pSpacing[c_ViewLabel];
	structContour* crntContour;
	if (c_ContourID == -1)
	{
		int id = ASTransContour::GetCrntContourID();
		crntContour = this->GetContourFromID(id);
	} 
	else
	{
		crntContour = this->GetContourFromID(c_ContourID);
	}
	if (!crntContour)
	{
		return;
	}
	// 改变控制点坐标
	int numOfCurve = crntContour->pListCurves->size();
	if (c_IndexOfContour == -1)
	{
		int unClosureContour = -1;
		// 判断当前层面是否有未闭合的轮廓
		for (int ncrntContour = 0; ncrntContour < numOfCurve; ncrntContour++)
		{
			double crntdLayer = crntContour->pListCurves->at(ncrntContour)->dLayer;
			if (crntdLayer >= layermin && crntdLayer < layermax)
			{
				// 是当前层的，判断是否闭合
				if (crntContour->pListCurves->at(ncrntContour)->isClosure == false)
				{
					unClosureContour = ncrntContour;
					break;
				}
			}
		}
		// 找到当前层面未闭合轮廓的最后一个控制点
		if (unClosureContour == -1)
		{
			return;
		}
		structPoint* crntpoint = crntContour->pListCurves->at(unClosureContour)->pvectorPoint.last();
		crntpoint->dX = pPosition[0];
		crntpoint->dY = pPosition[1];
		crntpoint->dZ = pPosition[2];
	}
	else
	{
		// 判断控制点是否合法
		structCurve* crntCurve = crntContour->pListCurves->at(c_IndexOfContour);
		if (crntCurve->ViewLabel != c_ViewLabel)
		{
			return;
		}
		if (crntCurve->dLayer >= layermin && crntCurve->dLayer < layermax)
		{
			if (c_IndexOfHandle >= 0 && c_IndexOfHandle < crntCurve->pvectorPoint.size())
			{
				structPoint* crntpoint = crntCurve->pvectorPoint.at(c_IndexOfHandle);
				crntpoint->dX = pPosition[0];
				crntpoint->dY = pPosition[1];
				crntpoint->dZ = pPosition[2];
			}
		}
	}
}
void ASContourData::DeleteContrlPointsTemp(double* pPosition, const int c_ViewLabel, double* pSpacing,
	const int c_ContourID)
{
	//计算当前层的范围，对该范围内数据认为同层
	double layermin = pPosition[c_ViewLabel] - 0.5 * pSpacing[c_ViewLabel];
	double layermax = pPosition[c_ViewLabel] + 0.5 * pSpacing[c_ViewLabel];
	structContour* crntContour = this->GetContourFromID(c_ContourID);
	if (!crntContour)
	{
		return;
	}
	int unClosureContour = -1;
	// 判断当前层面是否有未闭合的轮廓
	int numOfCurve = crntContour->pListCurves->size();
	for (int ncrntContour = 0; ncrntContour < numOfCurve; ncrntContour++)
	{
		double crntdLayer = crntContour->pListCurves->at(ncrntContour)->dLayer;
		if (crntdLayer >= layermin && crntdLayer < layermax)
		{
			// 是当前层的，判断是否闭合
			if (crntContour->pListCurves->at(ncrntContour)->isClosure == false)
			{
				unClosureContour = ncrntContour;
				break;
			}
		}
	}
	// 找到当前层面未闭合轮廓的最后一个控制点
	if (unClosureContour == -1)
	{
		return;
	}
	structCurve* crntCurve = crntContour->pListCurves->at(unClosureContour);
	if (crntCurve->haveTemporary == false)
	{
		return;
	}
	crntCurve->haveTemporary = false;
	delete crntCurve->pvectorPoint.last();
	crntCurve->pvectorPoint.removeLast();
	if (crntCurve->pvectorPoint.size() == 0)
	{
		// 如果删除的是最后一个点
		crntContour->pListCurves->removeOne(crntCurve);
		delete crntCurve;
	}
}
void ASContourData::SetContourClosed(const int c_viewlabel, const int c_indexOfContour, 
	const int c_indexOfHandle, const bool c_Closed, const int c_ContourID)
{
	// 根据轮廓序号得到轮廓
	structContour* crntContour = this->GetContourFromID(c_ContourID);
	if (!crntContour)
	{
		return;
	}
	structCurve* crntCurve = crntContour->pListCurves->at(c_indexOfContour);
	if (crntCurve->ViewLabel == c_viewlabel && c_indexOfHandle == 0)
	{
		crntCurve->isClosure = c_Closed;
	}
}
void ASContourData::DeleteContrlPoints(const int c_ViewLabel, double* pSpacing, const int c_ContourID, 
	const int c_IndexOfContour, const int c_IndexOfHandle)
{
	// 根据轮廓序号得到轮廓
	structContour* crntContour;
	if (c_ContourID == -1)
	{
		int id = ASTransContour::GetCrntContourID();
		crntContour = this->GetContourFromID(id);
	}
	else
	{
		crntContour = this->GetContourFromID(c_ContourID);
	}
	if (!crntContour)
	{
		return;
	}
	structCurve* crntCurve = crntContour->pListCurves->at(c_IndexOfContour);
	if (crntCurve->ViewLabel == c_ViewLabel)
	{
		crntCurve->pvectorPoint.removeAt(c_IndexOfHandle);
	}
	if (crntCurve->pvectorPoint.size() == 0)
	{
		// 如果删除的是最后一个点
		crntContour->pListCurves->removeOne(crntCurve);
		delete crntCurve;
	}
}
void ASContourData::InsertContrlPoints(double* pPosition, const int c_ViewLabel, double* pSpacing, 
	const int c_ContourID, const int c_IndexOfContour, const int c_IndexOfHandle)
{
	//计算当前层的范围，对该范围内数据认为同层
	double layermin = pPosition[c_ViewLabel] - 0.5 * pSpacing[c_ViewLabel];
	double layermax = pPosition[c_ViewLabel] + 0.5 * pSpacing[c_ViewLabel];
	structContour* crntContour = this->GetContourFromID(c_ContourID);
	if (!crntContour)
	{
		return;
	}
	double position[3];
	position[0] = pPosition[0];
	position[1] = pPosition[1];
	position[2] = pPosition[2];

	structCurve* crntCurve = crntContour->pListCurves->at(c_IndexOfContour);
	if (crntCurve->ViewLabel != c_ViewLabel)
	{
		return;
	}
	position[c_ViewLabel] = crntCurve->dLayer;
	//插入控制点
	structPoint* newPoint = new structPoint;
	newPoint->dX = position[0];
	newPoint->dY = position[1];
	newPoint->dZ = position[2];

	crntCurve->pvectorPoint.insert(c_IndexOfHandle + 1, newPoint);
}
void ASContourData::DeleteContour(const int c_viewlabel, const int c_indexOfContour, 
	const int c_indexOfHandle, const int c_ContourID)
{
	// 根据轮廓序号得到轮廓
	structContour* crntContour = this->GetContourFromID(c_ContourID);
	if (!crntContour)
	{
		return;
	}
	structCurve* crntCurve = crntContour->pListCurves->at(c_indexOfContour);
	if (crntCurve->ViewLabel == c_viewlabel)
	{
		foreach(structPoint* crntPoint, crntCurve->pvectorPoint)
		{
			delete crntPoint;
		}
	}
	crntContour->pListCurves->removeOne(crntCurve);
	delete crntCurve;
}
void ASContourData::DeleteWholeContour(const int c_ContourID)
{
	structContour* crntContour = this->GetContourFromID(c_ContourID);
	if (!crntContour)
	{
		return;
	}
	m_listContours->removeOne(crntContour);
	while (crntContour->pListCurves->size())
	{
		structCurve* crntCurve = crntContour->pListCurves->first();
		crntContour->pListCurves->removeFirst();
		while (crntCurve->pvectorPoint.size())
		{
			structPoint* crntPoint = crntCurve->pvectorPoint.first();
			crntCurve->pvectorPoint.removeFirst();
			delete crntPoint;
		}
		delete crntCurve;
	}
	delete crntContour->pListCurves;
	delete crntContour;
}
// 得到一个新的不重复ID
int ASContourData::GetNewContourID()
{
	int newid = 0;
	bool f = false;
	int sz = m_listContours->size();
	do
	{
		f = false;
		for (int i = 0; i < sz; i++)
		{
			if (m_listContours->at(i)->contourID == newid)
			{
				newid++;
				f = true;
				break;
			}
		}
	} while (f);
	return newid;
}