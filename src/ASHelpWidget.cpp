#include "ASHelpWidget.h"
#include "ui_ASHelpWidget.h"
#include <QHBoxLayout>
#include <QFileInfo>

ASHelpWidget::ASHelpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ASHelpWidget)
{
    ui->setupUi(this);
	this->setObjectName("ASHelpWidget");
	this->setWindowState(Qt::WindowMaximized);
	setWindowTitle(tr("Help"));
	setWindowIcon(QIcon(":/Icon/ToolBar_Help"));

	QString path = QFileInfo("./README.html").absoluteFilePath();
	QFileInfo fileInfo(path);
	if (!fileInfo.isFile())
	{
		path = QFileInfo("./Debug/README.html").absoluteFilePath();
	}

	ui->webEngineView->load(QUrl::fromLocalFile(path));
	ui->webEngineView->show();
}

ASHelpWidget::~ASHelpWidget()
{
    delete ui;
}
