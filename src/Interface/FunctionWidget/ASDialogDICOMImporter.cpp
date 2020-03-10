#include "ASDialogDICOMImporter.h"
#include "ASTransDICOMLoader.h"
#include "ASTransSetting.h"
#include <vtkDICOMItem.h>
#include <vtkDICOMValue.h>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QStringList>
#include <QRegExp>
#include <QByteArray>
#include <QHeaderView>
#include <QDir>
#include <QTextCodec>
#include <QHeaderView>
#include <QSettings>

ASDialogDICOMImporter::ASDialogDICOMImporter(QWidget *parent)
	: QDialog(parent)
{
	this->resize(QSize(800, 600));
	UIInit();

	//信号槽
	connect(m_PushButtonScanDirectory, SIGNAL(clicked()), this, SLOT(slotScanDirectory()));
	connect(m_PushButtonCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
	connect(m_PushButtonLoad, SIGNAL(clicked()), this, SLOT(slotLoad()));
}

ASDialogDICOMImporter::~ASDialogDICOMImporter()
{
}

// 检查是否为空，如果为空自动打开选路径对话框
void ASDialogDICOMImporter::CheckVoid()
{
	if (m_TableWidgetSeriesDisplay->rowCount() == 0)
	{
		this->slotScanDirectory();
	}
}
// 扫描完毕
void ASDialogDICOMImporter::DiskScanningFinished(vtkDICOMDirectory* DICOMDirectory)
{
	this->m_DICOMDirectory = DICOMDirectory;
	// 切换为序列列表
	m_StackedWidget->setCurrentWidget(m_TableWidgetSeriesDisplay);
	// 清空列表
	while (m_TableWidgetSeriesDisplay->rowCount())
	{
		m_TableWidgetSeriesDisplay->removeRow(0);
	}
	// 填充列表
	m_TableWidgetSeriesDisplay->setRowCount(DICOMDirectory->GetNumberOfSeries());
	// Iterate through all of the studies that are present.
	int NumOfStudy = DICOMDirectory->GetNumberOfStudies();
	for (int crntStudy = 0; crntStudy < NumOfStudy; crntStudy++)
	{
		// Get information related to the patient study
		vtkDICOMItem patient = DICOMDirectory->GetPatientRecordForStudy(crntStudy);
		vtkDICOMItem study = DICOMDirectory->GetStudyRecord(crntStudy);
		// Iterate through all of the series in this study.
		int j1 = DICOMDirectory->GetFirstSeriesForStudy(crntStudy);
		int j2 = DICOMDirectory->GetLastSeriesForStudy(crntStudy);
		for (int cuntSeries = j1; cuntSeries <= j2; cuntSeries++)
		{
			vtkDICOMItem series = DICOMDirectory->GetSeriesRecord(cuntSeries);
			QTableWidgetItem *item0 = new QTableWidgetItem;
			item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			item0->setCheckState(Qt::Checked);
			m_TableWidgetSeriesDisplay->setItem(cuntSeries, 0, item0);
			QTableWidgetItem* item1 = new QTableWidgetItem(series.GetAttributeValue(DC::SeriesNumber).GetCharData());
			item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			item1->setFlags(item1->flags() & (~Qt::ItemIsEditable)); // 设置单元格不可编辑
			item1->setFlags(item1->flags() & (~Qt::ItemIsSelectable)); // 设置单元格不可选择
			m_TableWidgetSeriesDisplay->setItem(cuntSeries, 1, item1);
			QTableWidgetItem* item2 = new QTableWidgetItem(series.GetAttributeValue(DC::Modality).GetCharData());
			item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			item2->setFlags(item2->flags() & (~Qt::ItemIsEditable)); // 设置单元格不可编辑
			item2->setFlags(item2->flags() & (~Qt::ItemIsSelectable)); // 设置单元格不可选择
			m_TableWidgetSeriesDisplay->setItem(cuntSeries, 2, item2);
			QTableWidgetItem* item3 = new QTableWidgetItem(series.GetAttributeValue(DC::SeriesDate).GetCharData());
			item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			item3->setFlags(item3->flags() & (~Qt::ItemIsEditable)); // 设置单元格不可编辑
			item3->setFlags(item3->flags() & (~Qt::ItemIsSelectable)); // 设置单元格不可选择
			m_TableWidgetSeriesDisplay->setItem(cuntSeries, 3, item3);
			QTableWidgetItem* item4 = new QTableWidgetItem(study.GetAttributeValue(DC::StudyID).GetCharData());
			item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			item4->setFlags(item4->flags() & (~Qt::ItemIsEditable)); // 设置单元格不可编辑
			item4->setFlags(item4->flags() & (~Qt::ItemIsSelectable)); // 设置单元格不可选择
			m_TableWidgetSeriesDisplay->setItem(cuntSeries, 4, item4);
			QTableWidgetItem* item5 = new QTableWidgetItem(patient.GetAttributeValue(DC::PatientName).GetCharData());
			item5->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			item5->setFlags(item5->flags() & (~Qt::ItemIsEditable)); // 设置单元格不可编辑
			item5->setFlags(item5->flags() & (~Qt::ItemIsSelectable)); // 设置单元格不可选择
			m_TableWidgetSeriesDisplay->setItem(cuntSeries, 5, item5);
			QTableWidgetItem* item6 = new QTableWidgetItem(patient.GetAttributeValue(DC::PatientID).GetCharData());
			item6->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			item6->setFlags(item6->flags() & (~Qt::ItemIsEditable)); // 设置单元格不可编辑
			item6->setFlags(item6->flags() & (~Qt::ItemIsSelectable)); // 设置单元格不可选择
			m_TableWidgetSeriesDisplay->setItem(cuntSeries, 6, item6);
		}
	}
	m_PushButtonLoad->setEnabled(true);
	m_isScanning = false;
}

void ASDialogDICOMImporter::closeEvent(QCloseEvent * e)
{
	// 如果正在扫描，取消扫描
	if (m_isScanning)
	{
		ASTransDICOMLoader::ScanningDiskStopManualy();

		// 切换为序列列表
		m_StackedWidget->setCurrentWidget(m_TableWidgetSeriesDisplay);
		// 清空列表
		while (m_TableWidgetSeriesDisplay->rowCount())
		{
			m_TableWidgetSeriesDisplay->removeRow(0);
		}
		m_LineEditScanDirectory->clear();
		m_PushButtonLoad->setEnabled(true);
		m_isScanning = false;
	}
	QDialog::closeEvent(e);
}

//界面初始化
void ASDialogDICOMImporter::UIInit()
{
	// 1
	QHBoxLayout* HBoxLayout1 = new QHBoxLayout();
	m_PushButtonScanDirectory = new QPushButton(this);
	m_LineEditScanDirectory = new QLineEdit(this);
	HBoxLayout1->addWidget(m_PushButtonScanDirectory);
	HBoxLayout1->addWidget(m_LineEditScanDirectory);
	m_PushButtonScanDirectory->setText("Scan Directory");
	m_PushButtonScanDirectory->setMinimumWidth(100);
	m_LineEditScanDirectory->setFocusPolicy(Qt::NoFocus);//设置它不能获得焦点，达到不可编辑的效果

	// 2
	m_StackedWidget = new QStackedWidget(this);
	m_TableWidgetSeriesDisplay = new QTableWidget(this);
	m_TextBrowserOutput = new QTextBrowser(this);
	m_StackedWidget->addWidget(m_TableWidgetSeriesDisplay);
	m_StackedWidget->addWidget(m_TextBrowserOutput);
	m_StackedWidget->setCurrentWidget(m_TableWidgetSeriesDisplay);

	// 3
	QHBoxLayout* HBoxLayout3 = new QHBoxLayout();
	m_PushButtonCancel = new QPushButton(this);
	m_PushButtonLoad = new QPushButton(this);
	HBoxLayout3->addStretch();
	HBoxLayout3->addWidget(m_PushButtonCancel);
	HBoxLayout3->addWidget(m_PushButtonLoad);
	m_PushButtonCancel->setText(tr("Cancel Scanning"));
	m_PushButtonCancel->setEnabled(false);
	m_PushButtonLoad->setText(tr("Import Data"));

	// 0
	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addLayout(HBoxLayout1);
	mainLayout->addWidget(m_StackedWidget);
	mainLayout->addLayout(HBoxLayout3);
	this->setLayout(mainLayout);

	//表格相关设置
	QStringList header;
	header.append(" ");
	header.append("SeriesNumber");
	header.append("Modality");
	header.append("SeriesDate");
	header.append("StudyID");
	header.append("PatientsName");
	header.append("PatientID");
	m_TableWidgetSeriesDisplay->verticalHeader()->hide();
	m_TableWidgetSeriesDisplay->setColumnCount(7);
	m_TableWidgetSeriesDisplay->setColumnWidth(0, 25);
	m_TableWidgetSeriesDisplay->setColumnWidth(1, 122);
	m_TableWidgetSeriesDisplay->setColumnWidth(2, 122);
	m_TableWidgetSeriesDisplay->setColumnWidth(3, 122);
	m_TableWidgetSeriesDisplay->setColumnWidth(4, 122);
	m_TableWidgetSeriesDisplay->setColumnWidth(5, 122);
	m_TableWidgetSeriesDisplay->setColumnWidth(6, 122);
	m_TableWidgetSeriesDisplay->setHorizontalHeaderLabels(header);
	m_TableWidgetSeriesDisplay->setShowGrid(false);
	m_TableWidgetSeriesDisplay->setFrameShape(QFrame::NoFrame); //设置无边框
	m_TableWidgetSeriesDisplay->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	m_TableWidgetSeriesDisplay->setFocusPolicy(Qt::NoFocus);
}

//选文件夹的按钮按下
void ASDialogDICOMImporter::slotScanDirectory()
{
	QString lastPath = ASTransSetting::GetSetting()->value("LastFilePath").toString();
	//打开文件标准对话框
	QString OperatingDirectoryName = QFileDialog::getExistingDirectory(
		this,						// 指定父类
		"Directory",				// 文件对话框标题栏名称
		lastPath);					// 指定打开文件目录
	//判断是否打开文件
	if (OperatingDirectoryName.size() == 0)
	{
		return;
	}
	QFileInfo fi(OperatingDirectoryName);
	//ASTransSetting::GetSetting()->setValue("LastFilePath", QVariant(fi.path()));
	ASTransSetting::GetSetting()->setValue("LastFilePath", QVariant(OperatingDirectoryName));
	m_LineEditScanDirectory->setText(OperatingDirectoryName);

	// 切换为输出窗口
	m_StackedWidget->setCurrentWidget(m_TextBrowserOutput);
	m_TextBrowserOutput->clear();
	m_TextBrowserOutput->append(tr(" - Starting scanning disk..."));
	m_PushButtonLoad->setEnabled(false);
	m_PushButtonCancel->setEnabled(true);
	m_isScanning = true;
	// 扫面硬盘
	ASTransDICOMLoader::ScanDisk(OperatingDirectoryName);
}

//取消的按钮按下
void ASDialogDICOMImporter::slotCancel()
{
	// 如果正在扫描，取消扫描
	if (m_isScanning)
	{
		ASTransDICOMLoader::ScanningDiskStopManualy();

		// 切换为序列列表
		m_StackedWidget->setCurrentWidget(m_TableWidgetSeriesDisplay);
		// 清空列表
		while (m_TableWidgetSeriesDisplay->rowCount())
		{
			m_TableWidgetSeriesDisplay->removeRow(0);
		}
		m_LineEditScanDirectory->clear();
		m_PushButtonLoad->setEnabled(true);
		m_PushButtonCancel->setEnabled(false);
		m_isScanning = false;
	}	
}
//导入的按钮按下
void ASDialogDICOMImporter::slotLoad()
{
	// 检查checkbox，是否需要读入数据
	QList<int>* listSeriesList = new QList<int>; // 需要读入的序列列表
	int numOfSeries = m_TableWidgetSeriesDisplay->rowCount();
	for (int crntSeries = 0; crntSeries < numOfSeries; crntSeries++)
	{
		if (m_TableWidgetSeriesDisplay->item(crntSeries, 0)->checkState() == Qt::Checked)
		{
			// 需要读入该序列
			listSeriesList->append(crntSeries);
		}		
	}
	// 读入数据
	if (ASTransDICOMLoader::LoadDICOMData(this->m_DICOMDirectory, listSeriesList))
		this->hide();
}