#include "ASPolyImageData.h"
#include <QFileInfo>
#include <QColor>

ASPolyImageData::ASPolyImageData()
{
	m_Datatype = DataType_PolyImageData;
}

ASPolyImageData::~ASPolyImageData()
{
}

// 调用此函数来释放节点
void ASPolyImageData::DeleteNode()
{
	QMutexLocker Locker(m_Mutex);
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASPolyImageData::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_PolyImageData || ASImageData::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASPolyImageData::isTypeAbsolute(const int c_Type)
{
	QMutexLocker Locker(m_Mutex);
	if (c_Type == DataType_PolyImageData)
	{
		return true;
	}
	return false;
}
bool ASPolyImageData::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_PolyImageData || ASImageData::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}

// 得到图像数据
QColor ASPolyImageData::getColor()
{
	QMutexLocker Locker(m_Mutex);
	return m_Color;
}
// 设置颜色， 0~255
void ASPolyImageData::setColor(const QColor c_Color)
{
	QMutexLocker Locker(m_Mutex);
	m_Color = c_Color;
}
int ASPolyImageData::getOpacity()
{
	QMutexLocker Locker(m_Mutex);
	return m_nOpacity;
}
void ASPolyImageData::setOpacity(const int opacity)
{
	QMutexLocker Locker(m_Mutex);
	m_nOpacity = opacity;
}
vtkPolyData* ASPolyImageData::getPolyImageData()
{
	QMutexLocker Locker(m_Mutex);
	return m_PolyData;
}
void ASPolyImageData::setPolyData(vtkPolyData* pPolyData)
{
	QMutexLocker Locker(m_Mutex);
	if (m_PolyData == nullptr)
	{
		m_PolyData = vtkSmartPointer<vtkPolyData>::New();
	}	
	m_PolyData->SetPoints(pPolyData->GetPoints());
	m_PolyData->SetPolys(pPolyData->GetPolys());
	//m_PolyData->DeepCopy(pPolyData);
}
// 读入数据后的数据初始化
void ASPolyImageData::DataInitAfterLoad()
{
	this->setShown(true);
	this->setBeingProcess(false);
	QMutexLocker Locker(m_Mutex);
	this->m_nOpacity = 100;
	m_PolyData->GetBounds(m_adDataExtent);

	// 2 初始颜色，先进行名字比配，若成功则初始为特点颜色，否则随机上色。
	if (m_FileName.contains("all"))
	{
		// 总曲面 白色
		m_Color.setRgb(255, 255, 255);

	}
	else if (m_FileName.contains("heart") || m_FileName.contains("Heart"))
	{
		// 心脏 
		m_Color.setRgb(250, 50, 50);
	}
	else if (m_FileName.contains("liver") || m_FileName.contains("Liver"))
	{
		// 肝脏
		m_Color.setRgb(50, 255, 50);
	}
	else if (m_FileName.contains("lkidney"))
	{
		// 左肾
		m_Color.setRgb(255, 128, 0);
	}
	else if (m_FileName.contains("lungs"))
	{
		// 肺
		m_Color.setRgb(255, 255, 50);
	}
	else if (m_FileName.contains("rkidney"))
	{
		// 右肾
		m_Color.setRgb(50, 255, 255);
	}
	else if (m_FileName.contains("skeleton") || m_FileName.contains("Skeleton"))
	{
		// 骨骼
		m_Color.setRgb(90, 90, 255);
	}
	else if (m_FileName.contains("skin") || m_FileName.contains("Skin"))
	{
		// 皮肤
		m_Color.setRgb(255, 255, 255);
	}
	else if (m_FileName.contains("spleen") || m_FileName.contains("Spleen"))
	{
		// 脾
		m_Color.setRgb(255, 50, 255);
	}
	else
	{
		// 默认白色
		m_Color.setRgb(255, 255, 255);
	}
}

//// 曲面形变模块
//void ASPolyImageData::setPolyDeformationChoosen(const bool isChoosen)
//{
//	m_isPolyDeformationChoosen = isChoosen;
//}
//bool ASPolyImageData::isPolyDeformationChoosen()
//{
//	return m_isPolyDeformationChoosen;
//}