#pragma once

#include <QWidget>
#include <QGridLayout>

//class ASSignalLinker;
class ASDiplayFrame;
class ASQVTKWidget;

//操作栏布局标号
#define OpeBarLayoutLabel_Default	0		//操作栏布局标号,默认布局
#define OpeBarLayoutLabel_Sagittal	1		//操作栏布局标号,矢状面，Sagittal
#define OpeBarLayoutLabel_Coronal	2		//操作栏布局标号,冠状面，Coronal
#define OpeBarLayoutLabel_Axial		3		//操作栏布局标号,横截面，Axial
#define OpeBarLayoutLabel_3D		4		//操作栏布局标号,三维视图，3D

class ASOperationBar : public QWidget
{
Q_OBJECT

public:
	ASOperationBar(QWidget* parent);
	~ASOperationBar();

	// 静态函数，得到四个QVTKWidget的指针
	static void GetQVTKWidget(ASQVTKWidget** pQVTKWidget);

protected:
	// 绘制边框
	void paintEvent(QPaintEvent* e);

private:

	//ASSignalLinker* SignalLinker;	//软件中通用信号连接器

	//界面初始化，包括五种窗口布局
	void GUIInit();
	// 静态函数对应到唯一对象中的函数
	void fGetQVTKWidget(ASQVTKWidget** pQVTKWidget);
	// 唯一对象的指针
	static ASOperationBar* ms_pOperationBar;

	QGridLayout* m_OperationbarLayout;
	ASDiplayFrame* m_apDisplayFrame[4];

	int m_LayoutMode = OpeBarLayoutLabel_Default;

	void setLayoutMode(int mode);

private slots:

	void slotButtonLayoutClicked(int viewlabel);
	void slotCrossShairClicked(int viewlabel);

public slots:
	void slotBackgroundColorChange(bool checked);

};
