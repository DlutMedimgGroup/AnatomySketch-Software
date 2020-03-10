#include "ASDataNodeItemWidget.h"
#include "ASTransGeneralInteractor.h"
#include "ASAbstractData.h"
#include "ASOperation.h"
#include "ASAddinsBase.h"
#include "ASTransTheme.h"
#include <QHBoxLayout>

ASDataNodeItemWidget::ASDataNodeItemWidget(QWidget *parent)
	: ASAbstructItemWidget(parent)
{
	m_ItemType = ItemType_DataNode;

	QHBoxLayout* MainLayout	= new QHBoxLayout(this);
	MainLayout->setContentsMargins(0, 0, 0, 0);	//±ß½ç¾àÀë£¬×óÉÏÓÒÏÂ
	//MainLayout->addWidget(m_pCheckbox);
	MainLayout->addWidget(m_pLabelForIcon);
	MainLayout->addWidget(m_pLabelForName);
	MainLayout->addStretch();
	m_pLabelForIcon->setFixedWidth(18);
}

ASDataNodeItemWidget::~ASDataNodeItemWidget()
{
}

// ItemType
bool ASDataNodeItemWidget::IsMarkEle()
{
	return false;
}

void ASDataNodeItemWidget::SetChecked(bool checked)
{
	m_Checked = checked;
	this->UpdateIcon();
}

//void ASDataNodeItemWidget::slotCheckboxStateChanged(int state)
//{
//	bool crntState;
//	if (state == 0)
//	{
//		crntState = false;
//	}
//	else
//	{
//		crntState = true;
//	}
//	ASTransGeneralInteractor::SetDateShown(this->GetID(), crntState);
//}
void ASDataNodeItemWidget::slotIconClicked()
{
	m_Checked = !m_Checked;
	this->UpdateIcon();
	ASTransGeneralInteractor::SetDateShown(this->GetID(), m_Checked);
}
void ASDataNodeItemWidget::UpdateIcon()
{
	ASAbstractData* crntNode = ASAbstractData::getDataNodeFromID(m_ID);
	bool theme = ASTransTheme::GetTheme();
	if (m_Checked)
	{
		if (theme)
		{
			switch (crntNode->getDataType())
			{
			case DataType_ArrayImageData:
				m_Icon = QPixmap(":/DataListIcons/mesh_dark_checked");
				break;
			case DataType_DICOMData:
				m_Icon = QPixmap(":/DataListIcons/mesh_dark_checked");
				break;
			case DataType_PolyImageData:
				m_Icon = QPixmap(":/DataListIcons/label_dark_checked");
				break;
			case DataType_DICOMRTData:
				m_Icon = QPixmap(":/DataListIcons/TypeOfData_DICOMRT");
				break;
			case DataType_SeedData:
				m_Icon = QPixmap(":/DataListIcons/seed_dark_checked");
				break;
			case DataType_ContourData:
				m_Icon = QPixmap(":/DataListIcons/contour_dark_checked");
				break;
			case DataType_LandmarkData:
				m_Icon = QPixmap(":/DataListIcons/landmark_dark_checked");
				break;
			case DataType_ROI:
				m_Icon = QPixmap(":/DataListIcons/boundingbox_dark_checked");
				break;
			case DataType_Annotation:
				m_Icon = QPixmap(":/DataListIcons/A_dark_checked");
				break;
			case DataType_Operation:
			{
				ASOperation* crntOpera = static_cast<ASOperation*>(crntNode);
				int addintype = crntOpera->GetAddinType();
				switch (addintype)
				{
				case AddinType_ImgProcess:
					m_Icon = QPixmap(":/DataListIcons/P_dark_checked");
					break;
				case AddinType_MeshProcess:
					m_Icon = QPixmap(":/DataListIcons/M_dark_checked");
					break;
				case AddinType_Segmentation:
					m_Icon = QPixmap(":/DataListIcons/S_dark_checked");
					break;
				default:
					break;
				}
			}
			default:
				break;
			}
		}
		else
		{
			switch (crntNode->getDataType())
			{
			case DataType_ArrayImageData:
				m_Icon = QPixmap(":/DataListIcons/mesh_light_checked");
				break;
			case DataType_DICOMData:
				m_Icon = QPixmap(":/DataListIcons/mesh_light_checked");
				break;
			case DataType_PolyImageData:
				m_Icon = QPixmap(":/DataListIcons/label_light_checked");
				break;
			case DataType_DICOMRTData:
				m_Icon = QPixmap(":/DataListIcons/TypeOfData_DICOMRT");
				break;
			case DataType_SeedData:
				m_Icon = QPixmap(":/DataListIcons/seed_light_checked");
				break;
			case DataType_ContourData:
				m_Icon = QPixmap(":/DataListIcons/contour_light_checked");
				break;
			case DataType_LandmarkData:
				m_Icon = QPixmap(":/DataListIcons/landmark_light_checked");
				break;
			case DataType_ROI:
				m_Icon = QPixmap(":/DataListIcons/boundingbox_light_checked");
				break;
			case DataType_Annotation:
				m_Icon = QPixmap(":/DataListIcons/A_light_checked");
				break;
			case DataType_Operation:
			{
				ASOperation* crntOpera = static_cast<ASOperation*>(crntNode);
				int addintype = crntOpera->GetAddinType();
				switch (addintype)
				{
				case AddinType_ImgProcess:
					m_Icon = QPixmap(":/DataListIcons/P_light_checked");
					break;
				case AddinType_MeshProcess:
					m_Icon = QPixmap(":/DataListIcons/M_light_checked");
					break;
				case AddinType_Segmentation:
					m_Icon = QPixmap(":/DataListIcons/S_light_checked");
					break;
				default:
					break;
				}
			}
			default:
				break;
			}
		}

	}
	else
	{
		if (theme)
		{
			switch (crntNode->getDataType())
			{
			case DataType_ArrayImageData:
				m_Icon = QPixmap(":/DataListIcons/mesh_dark_normal");
				break;
			case DataType_DICOMData:
				m_Icon = QPixmap(":/DataListIcons/mesh_dark_normal");
				break;
			case DataType_PolyImageData:
				m_Icon = QPixmap(":/DataListIcons/label_dark_normal");
				break;
			case DataType_DICOMRTData:
				m_Icon = QPixmap(":/DataListIcons/TypeOfData_DICOMRT");
				break;
			case DataType_SeedData:
				m_Icon = QPixmap(":/DataListIcons/seed_dark_normal");
				break;
			case DataType_ContourData:
				m_Icon = QPixmap(":/DataListIcons/contour_dark_normal");
				break;
			case DataType_LandmarkData:
				m_Icon = QPixmap(":/DataListIcons/landmark_dark_normal");
				break;
			case DataType_ROI:
				m_Icon = QPixmap(":/DataListIcons/boundingbox_dark_normal");
				break;
			case DataType_Annotation:
				m_Icon = QPixmap(":/DataListIcons/A_dark_normal");
				break;
			case DataType_Operation:
			{
				ASOperation* crntOpera = static_cast<ASOperation*>(crntNode);
				int addintype = crntOpera->GetAddinType();
				switch (addintype)
				{
				case AddinType_ImgProcess:
					m_Icon = QPixmap(":/DataListIcons/P_dark_normal");
					break;
				case AddinType_MeshProcess:
					m_Icon = QPixmap(":/DataListIcons/M_dark_normal");
					break;
				case AddinType_Segmentation:
					m_Icon = QPixmap(":/DataListIcons/S_dark_normal");
					break;
				default:
					break;
				}
			}
			default:
				break;
			}
		}
		else
		{
			switch (crntNode->getDataType())
			{
			case DataType_ArrayImageData:
				m_Icon = QPixmap(":/DataListIcons/mesh_light_normal");
				break;
			case DataType_DICOMData:
				m_Icon = QPixmap(":/DataListIcons/mesh_light_normal");
				break;
			case DataType_PolyImageData:
				m_Icon = QPixmap(":/DataListIcons/label_light_normal");
				break;
			case DataType_DICOMRTData:
				m_Icon = QPixmap(":/DataListIcons/TypeOfData_DICOMRT");
				break;
			case DataType_SeedData:
				m_Icon = QPixmap(":/DataListIcons/seed_light_normal");
				break;
			case DataType_ContourData:
				m_Icon = QPixmap(":/DataListIcons/contour_light_normal");
				break;
			case DataType_LandmarkData:
				m_Icon = QPixmap(":/DataListIcons/landmark_light_normal");
				break;
			case DataType_ROI:
				m_Icon = QPixmap(":/DataListIcons/boundingbox_light_normal");
				break;
			case DataType_Annotation:
				m_Icon = QPixmap(":/DataListIcons/A_light_normal");
				break;
			case DataType_Operation:
			{
				ASOperation* crntOpera = static_cast<ASOperation*>(crntNode);
				int addintype = crntOpera->GetAddinType();
				switch (addintype)
				{
				case AddinType_ImgProcess:
					m_Icon = QPixmap(":/DataListIcons/P_light_normal");
					break;
				case AddinType_MeshProcess:
					m_Icon = QPixmap(":/DataListIcons/M_light_normal");
					break;
				case AddinType_Segmentation:
					m_Icon = QPixmap(":/DataListIcons/S_light_normal");
					break;
				default:
					break;
				}
			}
			default:
				break;
			}
		}
	}
	QPixmap fitpixmap = m_Icon.scaled(QSize(m_nH, m_nH), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	m_pLabelForIcon->setPixmap(fitpixmap);
	m_pLabelForIcon->show();
}