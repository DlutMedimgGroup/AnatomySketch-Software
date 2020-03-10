#include "ASApplication.h"
#include "ASTransGlobalKeyBoard.h"
#include "ASTransTheme.h"
#include "ASTransLang.h"
#include "ASTransSetting.h"
#include <QKeyEvent>
#include <QMetaObject>
#include <QSettings>
#include <QTranslator>
#include <QProcess>
#include <QMessageBox>

ASApplication::ASApplication(int &argc,char **argv)
	: QApplication(argc,argv)
{
	QSettings* Setting = new QSettings("./Configurations/Setting.ini", QSettings::IniFormat);
	bool default_lang = Setting->value("DefaultLang").toBool();
	zhTranslator = new QTranslator(this);
	zhTranslator->load(":/QSSfile/zh");
	if (default_lang)
		qApp->installTranslator(zhTranslator);
}

ASApplication::~ASApplication()
{
}

bool ASApplication::notify(QObject *obj, QEvent *e)
{
	if (e->type() == QEvent::KeyPress)
	{
		if (obj->inherits("QWidgetWindow"))
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
			ASTransGlobalKeyBoard::GlobalKeyBoard(keyEvent);
		}
	}

    return QApplication::notify(obj, e);
}

// 在亮暗主题切换
void ASApplication::slotQSSChange(bool checked)
{
	bool default_theme = ASTransTheme::GetTheme();

	// 动态载入主题文件
	QString path = ":/QSSfile/default_light";
	if (checked)
		path = ":/QSSfile/dark_theme";;
	QFile qssfile(path);
	qssfile.open(QFile::ReadOnly);
	QString qss;
	qss = qssfile.readAll();
	this->setStyleSheet(qss);

	// 写入主题选择到配置文件中
	ASTransSetting::GetSetting()->setValue("DefaultTheme", checked);
	ASTransTheme::SetTheme(checked);
}

// 在中英切换
void ASApplication::slotLangChange(bool checked)
{
	QMessageBox *msgBox;
	msgBox = new QMessageBox(tr("Change language?"),
		tr("Changing language requires to restart software, all the working progress will be discarded, continue? "),
		QMessageBox::Warning,
		QMessageBox::Ok | QMessageBox::Default,		
		QMessageBox::Cancel | QMessageBox::Escape,	
		0);														

	if (msgBox->exec() == QMessageBox::Ok)
	{

		bool default_lang = ASTransLang::GetLang();

		// 写入语言选择到配置文件中
		ASTransSetting::GetSetting()->setValue("DefaultLang", checked);
		ASTransLang::SetLang(checked);

		QCoreApplication::exit(-1);
	}
}

void ASApplication::connectsns()
{
	connect(ASTransTheme::GetSelfPointer(), SIGNAL(signalQSSChange(bool)), this, SLOT(slotQSSChange(bool)));
	connect(ASTransLang::GetSelfPointer(), SIGNAL(signalLanguageChange(bool)), this, SLOT(slotLangChange(bool)));
}
