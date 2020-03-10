#include "AboutWidget.h"
#include "ui_AboutWidget.h"
#include <QIcon>
#include <QPixmap>

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutWidget)
{
    ui->setupUi(this);

	this->setObjectName("AboutWidget");
	setWindowTitle(tr("About"));
	setWindowIcon(QIcon(":/Icon/ToolBar_About"));

	QPixmap pixmap(":/Image/DUT_MEDIMG_LOG");
	ui->label_logo->setPixmap(pixmap);
	ui->label_logo->setScaledContents(true);
	ui->label_logo->show();
}

AboutWidget::~AboutWidget()
{
    delete ui;
}
