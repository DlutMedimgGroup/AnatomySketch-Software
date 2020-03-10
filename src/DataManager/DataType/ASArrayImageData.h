#pragma once

#include "ASImageData.h"
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
#include <vtkImageData.h>

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

#define ColorMode_Grayscale	0
#define ColorMode_RGB		1

class ASArrayImageData : public ASImageData
{
public:
	ASArrayImageData();
	~ASArrayImageData();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);

	// 得到图像信息
	int getColorMap();
	void setColorMap(const int colormap); // 设置颜色表， 宏定义:ColorMap_
	int getLayer();
	void setLayer(const int c_Layer);
	// 窗位窗宽
	double* getWindowLevel();
	void setWindowLevel(); // 无参数表示重置为默认值
	void setWindowLevel(const double window, const double level);
	double* getImageExtent();
	// 不透明度
	int getOpacity();
	void setOpacity(const int opacity);
	// 图像数据
	vtkImageData* getArrayImageData(); // 拷贝另一个数据的图像
	void copyArrayImageDataFrom(const int c_ID);
	// 灰度图和标签图
	enumArrayImageDataType getArrayImageDataType();
	void setArrayImageDataType(const enumArrayImageDataType type);
	// 色彩模式
	void SetColorMode(const int c_mode);
	int GetColorMode();

	// 设置数据
	void setArrayImageData(vtkImageData* ImageData);
	// 读入数据后的数据初始化
	void DataInitAfterLoad();


	// 虚函数
	// 1 数据读取
	//virtual int LoadData(QString filename);
	// 2 设置是否显示，重写该虚函数是因为要统计当前显示的体数据的数量
	virtual void setShown(const bool shown);

	// ---------------------- 静态函数 ----------------------
	// 得到当前已经打开体数据数量
	static int getNumOfArrayImageData();
	// 得到当前已经显示体数据数量
	static int getNumberOfArrayImageDataShown();
	// 得到当前顶层显示的数据，没有返回nullptr
	static ASArrayImageData* getTopLayerDisplayingArrayImageData();

private:

	static int ms_nNumOfArrayImageData;				// 当前打开的体数据的数量
	static int ms_nNumberOfArrayImageDataShown;		// 当前显示的体数据的数量

protected:

	int			m_ColorMode;			// 色彩模式。宏定义:ColorMode_
	int			m_nColorMap;			// 宏定义:ColorMap_
	int			m_nLayer;				// 在显示时的层级关系，连续不重复，大层在上面
	double		m_adWindowLevel[2];		// [0]是color window [1]是color level
	int	m_nOpacity = 100; // 不透明度,0~100
	enumArrayImageDataType			m_ArrayImageDataType;	// 图像类型。0：图像文件；1：标签图文件
	vtkSmartPointer<vtkMatrix4x4>	m_pTransform;			// 图像变换矩阵
	vtkSmartPointer<vtkImageData>	m_pImageData;			// 储存体数据

	// 判断数据m_pImageData是灰度图还是标签图，更新m_ArrayImageDataType和m_nColorMap，读取数据时使用
	void checkDataTypeAndColorMap();
	// 计算数据范围，更新m_adImageExtent，读取数据时使用
	void checkDataExtent();
	// 判断数据m_pImageData是否为CT图像，更新m_adWindowLevel，读取数据时使用
	void checkDataIsCTOrNot();

	// 静态成员和成员函数
	

};
