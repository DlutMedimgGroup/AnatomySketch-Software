#include "ASDataNodeRClickMenu.h"
#include "ASDataManagerAction.h"
#include "ASAbstractData.h"
#include "ASArrayImageData.h"
#include "ASPolyImageData.h"
#include "ASTransAddDeleteData.h"
#include "ASTransGeneralInteractor.h"
#include "ASTransDataGeneral.h"
#include "ASTransAddins.h"
#include "ASTransROI.h"
#include "ASTransContour.h"
#include "ASTransSeeds.h"
#include "ASTransLandmark.h"
#include "ASColorMap.h"
#include "ASAddinsBase.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QColorDialog>

ASDataNodeRClickMenu::ASDataNodeRClickMenu(QObject *parent)
	: QObject(parent)
{
	this->RightClickInit();
}

ASDataNodeRClickMenu::~ASDataNodeRClickMenu()
{

}

void ASDataNodeRClickMenu::Generate(const QPoint c_pos, ASAbstractData* DataNode)
{
	m_pGeneratingDataNode = DataNode;
	for (int i = 0; i < 2; i++)
	{
		m_RADataType[i]->setChecked(false);
	}
	for (int i = 0; i < 14; i++)
	{
		m_RAColorMap[i]->setChecked(false);
	}

	QMenu* crntMenu = new QMenu();
	if (DataNode->isTypeInherited(DataType_ArrayImageData))	//体数据
	{
		ASArrayImageData* crntDataNode = (ASArrayImageData*)DataNode;
		// 添加菜单
		//if (crntDataNode->getFather()->getDataNodeID() < 0)
		//{
		//	crntMenu->addAction(m_RACuttentData);
		//}
		crntMenu->addAction(m_RASave);
		crntMenu->addAction(m_RARemove);
		//crntMenu->addAction(m_RARemoveAll);
		crntMenu->addAction(m_RAReinit);
		crntMenu->addAction(m_RAMoveto);
		//crntMenu->addAction(m_RANewContour);
		//crntMenu->addAction(m_RANewSeeds);
		//crntMenu->addAction(m_RANewROI);
		//crntMenu->addAction(m_RANewLandmark);
		crntMenu->addAction(m_RANewAnnotation);
		crntMenu->addAction(m_RANewImgProcess);
		crntMenu->addAction(m_RANewSeg);
		crntMenu->addMenu(m_RMDataType);
		crntMenu->addMenu(m_RMColorMap);
		m_RADataType[crntDataNode->getArrayImageDataType()]->setChecked(true);
		m_RAColorMap[crntDataNode->getColorMap()]->setChecked(true);
		// 不透明度滑动条
		m_RSliderOpacity->setValue(crntDataNode->getOpacity());
		crntMenu->addAction(m_RWAOpacity);
	}
	else if (DataNode->isTypeInherited(DataType_PolyImageData))
	{
		ASPolyImageData* crntDataNode = (ASPolyImageData*)DataNode;
		// 添加菜单
		crntMenu->addAction(m_RASave);
		crntMenu->addAction(m_RARemove);
		//crntMenu->addAction(m_RARemoveAll);
		crntMenu->addAction(m_RAReinit);
		crntMenu->addAction(m_RAMoveto);
		crntMenu->addAction(m_RANewMeshProcess);
		// 不透明度滑动条
		m_RSliderOpacity->setValue(crntDataNode->getOpacity());
		crntMenu->addAction(m_RWAOpacity);
		// 面数据颜色选择
		QString str = "background-color:rgb(" + QString::number(crntDataNode->getColor().red()) + "," +
			QString::number(crntDataNode->getColor().green()) + "," + QString::number(crntDataNode->getColor().blue()) + ");";
		m_RButtonColor->setStyleSheet(str);
		crntMenu->addAction(m_RWAColor);
	}
	else if (DataNode->isTypeInherited(DataType_SeedData))
	{
		// 添加菜单
		crntMenu->addAction(m_RANewSeeds);
		crntMenu->addAction(m_RASave);
		crntMenu->addAction(m_RARemove);
		//crntMenu->addAction(m_RARemoveAll);
		crntMenu->addAction(m_RAMoveto);
	}
	else if (DataNode->isTypeInherited(DataType_ContourData))
	{
		// 添加菜单
		//crntMenu->addAction(m_RANewContour);
		crntMenu->addAction(m_RASave);
		crntMenu->addAction(m_RARemove);
		//crntMenu->addAction(m_RARemoveAll);
		crntMenu->addAction(m_RAMoveto);
		crntMenu->addMenu(m_RMContourInterStyle);
		int i = ASTransContour::GetInterfaceModel();
		switch (i)
		{
		case 0:
			m_RAContourIS_Mouse->setChecked(true);
			m_RAContourIS_MouseSerial->setChecked(false);
			m_RAContourIS_Stylus->setChecked(false);
			break;
		case 1:
			m_RAContourIS_Mouse->setChecked(false);
			m_RAContourIS_MouseSerial->setChecked(true);
			m_RAContourIS_Stylus->setChecked(false);
			break;
		case 2:
			m_RAContourIS_Mouse->setChecked(false);
			m_RAContourIS_MouseSerial->setChecked(false);
			m_RAContourIS_Stylus->setChecked(true);
			break;
		default:
			break;
		}
		
	}
	else if (DataNode->isTypeInherited(DataType_LandmarkData))
	{
		// 添加菜单
		crntMenu->addAction(m_RASave);
		crntMenu->addAction(m_RARemove);
		//crntMenu->addAction(m_RARemoveAll);
		crntMenu->addAction(m_RAMoveto);
	}
	else if (DataNode->isTypeInherited(DataType_ROI))
	{
		// 添加菜单
		crntMenu->addAction(m_RANewROI);
		crntMenu->addAction(m_RASave);
		crntMenu->addAction(m_RARemove);
		//crntMenu->addAction(m_RARemoveAll);
		crntMenu->addAction(m_RAMoveto);
	}
	else if (DataNode->isTypeInherited(DataType_Annotation))
	{
		crntMenu->addAction(m_RARemove);
		//crntMenu->addAction(m_RARemoveAll);
		crntMenu->addAction(m_RANewContour);
		crntMenu->addAction(m_RANewSeeds);
		crntMenu->addAction(m_RANewROI);
		crntMenu->addAction(m_RANewLandmark);
	}
	else if (DataNode->isTypeInherited(DataType_Operation))
	{
		crntMenu->addAction(m_RARemove);
		//crntMenu->addAction(m_RARemoveAll);
	}
	// 4 弹出右键菜单
	ASDataManagerAction* selectedItem = (ASDataManagerAction*)crntMenu->exec(c_pos);
	delete crntMenu;
	
	if (selectedItem)
	{
		enumActionInContextMenu action = selectedItem->getAction();
		RightClickMenuAction(DataNode, action);
	}
	m_pGeneratingDataNode = nullptr;
}

// 右键菜单初始化
void ASDataNodeRClickMenu::RightClickInit()
{
	// 通用菜单
	m_RASave = new ASDataManagerAction(tr("Save"), this, EMActSave);
	m_RARemove = new ASDataManagerAction(tr("Remove"), this, EMActRemove);
	m_RARemoveAll = new ASDataManagerAction(tr("Remove all"), this, EMActRemoveAll);
	m_RAReinit = new ASDataManagerAction(tr("Reset"), this, EMActReinit);
	m_RAMoveto = new ASDataManagerAction(tr("Move to"), this, EMActMoveto);
	m_RANewAnnotation = new ASDataManagerAction(tr("New Annotation"), this, EMActNewAnnotation);
	m_RANewSeg = new ASDataManagerAction(tr("New Segmentation"), this, EMActNewSeg);
	m_RANewImgProcess = new ASDataManagerAction(tr("New Image Processing"), this, EMActNewImgProcess);
	m_RANewMeshProcess = new ASDataManagerAction(tr("New Mesh Processing"), this, EMActNewMeshProcess);
	m_RACuttentData = new ASDataManagerAction(tr("Set to current data"), this, EMActCurrentData);
	// 新建数据
	m_RANewContour = new ASDataManagerAction(tr("New Contour"), this, EMActNewContour);
	m_RANewSeeds = new ASDataManagerAction(tr("New Seeds"), this, EMActNewSeed);
	m_RANewROI = new ASDataManagerAction(tr("New Bounding Box"), this, EMActNewROI);
	m_RANewLandmark = new ASDataManagerAction(tr("New Landmark"), this, EMActNewLandmark);
	// 体数据类型
	m_RMDataType = new QMenu(tr("Data Type"));
	m_RADataType[0] = new ASDataManagerAction(tr("Greyscale Map"), this, EMActDataType_GrayScale);
	m_RADataType[1] = new ASDataManagerAction(tr("Label Map"), this, EMActDataType_LabelMap);
	for (int i = 0; i < 2; i++)
	{
		m_RMDataType->addAction(m_RADataType[i]);
		m_RADataType[i]->setCheckable(true);
	}
	m_RMDataType->addAction(m_RADataType[0]);
	m_RMDataType->addAction(m_RADataType[1]);
	// 轮廓线交互方式
	m_RMContourInterStyle = new QMenu(tr("Interactor Style"));
	m_RAContourIS_Mouse = new ASDataManagerAction(tr("Use mouse to add control point"), 
		this, EMActCntrIS_Mouse);
	m_RAContourIS_MouseSerial = new ASDataManagerAction(tr("Use mouse to add serial points"),
		this, EMActCntrIS_MouseSerial);
	m_RAContourIS_Stylus = new ASDataManagerAction(tr("Use stylus to add serial points"),
		this, EMActCntrIS_Stylus);
	m_RMContourInterStyle->addAction(m_RAContourIS_Mouse);
	m_RMContourInterStyle->addAction(m_RAContourIS_MouseSerial);
	m_RMContourInterStyle->addAction(m_RAContourIS_Stylus);
	m_RAContourIS_Mouse->setCheckable(true);
	m_RAContourIS_MouseSerial->setCheckable(true);
	m_RAContourIS_Stylus->setCheckable(true);
	// 体数据颜色表
	m_RMColorMap = new QMenu(tr("Color Map"));
	m_RAColorMap[0] = new ASDataManagerAction(tr("Grayscale"), this, EMActColorMap_Gray);
	m_RAColorMap[1] = new ASDataManagerAction(tr("Multilabel"), this, EMActColorMap_MultiLabel);
	m_RAColorMap[2] = new ASDataManagerAction(tr("Mask"), this, EMActColorMap_Mask);
	m_RAColorMap[3] = new ASDataManagerAction(tr("Cold"), this, EMActColorMap_Cold);
	m_RAColorMap[4] = new ASDataManagerAction(tr("Hot"), this, EMActColorMap_Hot);
	m_RAColorMap[5] = new ASDataManagerAction(tr("Cool"), this, EMActColorMap_Cool);
	m_RAColorMap[6] = new ASDataManagerAction(tr("Bronson"), this, EMActColorMap_Bronson);
	m_RAColorMap[7] = new ASDataManagerAction(tr("Split_BBR"), this, EMActColorMap_SplitBBR);
	m_RAColorMap[8] = new ASDataManagerAction(tr("UCLA"), this, EMActColorMap_UCLA);
	m_RAColorMap[9] = new ASDataManagerAction(tr("Red"), this, EMActColorMap_Red);
	m_RAColorMap[10] = new ASDataManagerAction(tr("Green"), this, EMActColorMap_Green);
	m_RAColorMap[11] = new ASDataManagerAction(tr("Blue"), this, EMActColorMap_Blue);
	m_RAColorMap[12] = new ASDataManagerAction(tr("HeartVR"), this, EMActColorMap_HeartVR);
	m_RAColorMap[13] = new ASDataManagerAction(tr("HeadVR"), this, EMActColorMap_HeadVR);
	for (int i = 0; i < 14; i++)
	{
		m_RMColorMap->addAction(m_RAColorMap[i]);
		m_RAColorMap[i]->setCheckable(true);
	}
	// 不透明度滑动条
	QWidget* pWidgetOpacity = new QWidget();
	m_RSliderOpacity = new QSlider(Qt::Horizontal, pWidgetOpacity);
	m_RWAOpacity = new QWidgetAction(this);
	m_RWAOpacity->setDefaultWidget(pWidgetOpacity);
	m_RSliderOpacity->setMaximum(100);
	m_RSliderOpacity->setMinimum(0);
	QHBoxLayout* pLayoutOpacity = new QHBoxLayout(pWidgetOpacity);
	QLabel* pLabelOpacity = new QLabel(pWidgetOpacity);
	pLabelOpacity->setText(tr("Opacity: "));
	pLabelOpacity->setFixedWidth(50);
	pLayoutOpacity->addWidget(pLabelOpacity);
	pLayoutOpacity->addWidget(m_RSliderOpacity);
	pWidgetOpacity->setLayout(pLayoutOpacity);
	connect(m_RSliderOpacity, SIGNAL(valueChanged(int)), this, SLOT(slotOpacityPolyContextMenu(int)));
	// 颜色选择
	QWidget* pWidgetColor = new QWidget();
	m_RWAColor = new QWidgetAction(this);
	m_RWAColor->setDefaultWidget(pWidgetColor);
	m_RButtonColor = new QPushButton(pWidgetColor);
	QLabel* pLabelColor = new QLabel(pWidgetColor);
	pLabelColor->setText(tr("Color: "));
	pLabelColor->setFixedWidth(50);
	QHBoxLayout* pLayoutColor = new QHBoxLayout(pWidgetColor);
	pLayoutColor->addWidget(pLabelColor);
	pLayoutColor->addWidget(m_RButtonColor);
	pWidgetColor->setLayout(pLayoutColor);
	connect(m_RButtonColor, SIGNAL(clicked()), this, SLOT(slotColorPolyContextMenu()));
}
// 右键菜单动作
void ASDataNodeRClickMenu::RightClickMenuAction(ASAbstractData* DataNode, enumActionInContextMenu action)
{
	switch (action)
	{
	case EMActSave:
		ASTransAddDeleteData::SaveOneDataToDisk(DataNode);
		break;
	case EMActRemove:
		ASTransAddDeleteData::RemoveDataNode(DataNode);
		break;
	case EMActRemoveAll:
		ASTransAddDeleteData::RemoveAllDataNode();
		break;
	case EMActReinit:
		ASTransGeneralInteractor::ReinitDataNode(DataNode);
		break;
	case EMActMoveto:
		ASTransAddDeleteData::MoveDataNode(DataNode);
		break;
	case EMActNewAnnotation:
		ASTransDataGeneral::NewAnnotationData(DataNode);
		break;
	case EMActNewSeg:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
		ASTransAddins::NewAddinOperation(DataNode, AddinType_Segmentation);
		break;
	case EMActNewImgProcess:
		ASTransAddins::NewAddinOperation(DataNode, AddinType_ImgProcess);
		break;
	case EMActNewMeshProcess:
		ASTransAddins::NewAddinOperation(DataNode, AddinType_MeshProcess);
		break;
	case EMActProperty:
		break;
	case EMActCurrentData:
		ASTransDataGeneral::SetGeneratingData(DataNode);
		break;
	case EMActNewContour:
		ASTransContour::NewContourData(DataNode);
		break;
	case EMActNewSeed:
		ASTransSeeds::NewSeedsData(DataNode);
		break;
	case EMActNewROI:
		ASTransROI::NewROIData(DataNode);
		break;
	case EMActNewLandmark:
		ASTransLandmark::NewLandmarkData(DataNode);
		break;
	case EMActDataType_GrayScale:
		if (DataNode->isTypeInherited(DataType_ArrayImageData))
		{
			((ASArrayImageData*)DataNode)->setArrayImageDataType(EMGrayscaleImage);
		}
		break;
	case EMActDataType_LabelMap:
		if (DataNode->isTypeInherited(DataType_ArrayImageData))
		{
			((ASArrayImageData*)DataNode)->setArrayImageDataType(EMMultiLabelImage);
		}
		break;
	case EMActColorMap_Gray:
	case EMActColorMap_MultiLabel:
	case EMActColorMap_Mask:
	case EMActColorMap_Cold:
	case EMActColorMap_Hot:
	case EMActColorMap_Cool:
	case EMActColorMap_Bronson:
	case EMActColorMap_SplitBBR:
	case EMActColorMap_UCLA:
	case EMActColorMap_Red:
	case EMActColorMap_Green:
	case EMActColorMap_Blue:
	case EMActColorMap_HeartVR:
	case EMActColorMap_HeadVR:
		if (DataNode->isTypeInherited(DataType_ArrayImageData))
		{
			int colormap = getColorMapFromAction(action);
			if (colormap != -1)
			{
				ASTransGeneralInteractor::SetArrayDataColorMap((ASArrayImageData*)DataNode, colormap);
			}
		}
		break;
	case EMActCntrIS_Mouse:
		ASTransContour::SetInterfaceModel(0);
		break;
	case EMActCntrIS_MouseSerial:
		ASTransContour::SetInterfaceModel(1);
		break;
	case EMActCntrIS_Stylus:
		ASTransContour::SetInterfaceModel(2);
		break;
	}
	
}
// 由右键菜单动作得到颜色表序号
int ASDataNodeRClickMenu::getColorMapFromAction(enumActionInContextMenu actor)
{
	switch (actor)
	{
	case EMActColorMap_Gray:
		return ColorMap_Gray;
		break;
	case EMActColorMap_MultiLabel:
		return ColorMap_Multilabel;
		break;
	case EMActColorMap_Mask:
		return ColorMap_Mask;
		break;
	case EMActColorMap_Cold:
		return ColorMap_Cold;
		break;
	case EMActColorMap_Hot:
		return ColorMap_Hot;
		break;
	case EMActColorMap_Cool:
		return ColorMap_Cool;
		break;
	case EMActColorMap_Bronson:
		return ColorMap_Bronson;
		break;
	case EMActColorMap_SplitBBR:
		return ColorMap_Split_BBR;
		break;
	case EMActColorMap_UCLA:
		return ColorMap_UCLA;
		break;
	case EMActColorMap_Red:
		return ColorMap_Red;
		break;
	case EMActColorMap_Green:
		return ColorMap_Green;
		break;
	case EMActColorMap_Blue:
		return ColorMap_Blue;
		break;
	case EMActColorMap_HeartVR:
		return ColorMap_HeartVR;
		break;
	case EMActColorMap_HeadVR:
		return ColorMap_HeadVR;
		break;
	default:
		return -1;
		break;
	}
}

// 右键菜单面数据颜色和不透明度
void ASDataNodeRClickMenu::slotOpacityPolyContextMenu(int value)
{
	if (m_pGeneratingDataNode)
	{
		ASTransGeneralInteractor::SetPolyDataOpacity(m_pGeneratingDataNode, value);
	}	
}
void ASDataNodeRClickMenu::slotColorPolyContextMenu()
{
	QColor OriginColor = ((ASPolyImageData*)m_pGeneratingDataNode)->getColor();
	QColor NewColor = QColorDialog::getColor(OriginColor, nullptr, tr("Select the color of curved surface"));
	if (OriginColor != NewColor)
	{
		if (m_pGeneratingDataNode->isTypeInherited(DataType_PolyImageData))
		{
			ASTransGeneralInteractor::SetPolyDataColor((ASPolyImageData*)m_pGeneratingDataNode, NewColor);
		}
	}
}