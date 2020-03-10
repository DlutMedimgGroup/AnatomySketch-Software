#pragma once

#include <vtkSmartPointer.h>
#include <vtkDICOMDirectory.h>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QList>
#include <QStackedWidget>
#include <QTextBrowser>

class ASSignalLinker;
class ASDataManager;

class ASDialogDICOMImporter : public QDialog
{
	Q_OBJECT

public:
	ASDialogDICOMImporter(QWidget *parent);
	~ASDialogDICOMImporter();

	// 检查是否为空，如果为空自动打开选路径对话框
	void CheckVoid();
	// 扫描完毕
	void DiskScanningFinished(vtkDICOMDirectory* DICOMDirectory);

protected:
	virtual void closeEvent(QCloseEvent * e);

private:

	//界面元素
	QPushButton*		m_PushButtonScanDirectory;		// 选择路径按钮
	QLineEdit*			m_LineEditScanDirectory;		// 显示当前路径
	QStackedWidget*		m_StackedWidget;				// 结果和输出窗口切换
	QTableWidget*		m_TableWidgetSeriesDisplay;		// 显示解析到的序列
	QTextBrowser*		m_TextBrowserOutput;			// 输出窗口
	QPushButton*		m_PushButtonCancel;				// 取消
	QPushButton*		m_PushButtonLoad;				// 导入
	//界面初始化
	void UIInit();
	// 扫描结果
	vtkSmartPointer<vtkDICOMDirectory> m_DICOMDirectory;

	// 是否正在扫描
	bool m_isScanning = false;

private slots:

	//选文件夹的按钮按下
	void slotScanDirectory();
	//取消的按钮按下
	void slotCancel();
	//导入的按钮按下
	void slotLoad();
};
