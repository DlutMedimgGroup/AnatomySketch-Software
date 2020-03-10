#include "ASLauncher.h"
#include "ASApplication.h"
#include "ASTransTheme.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QSharedMemory>
#include <QSettings>
#include <QProcess>
#include <iostream>

int main(int argc, char *argv[])
{
	//#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
	//	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	//#endif 

	ASApplication *a = new ASApplication(argc, argv);
	QTextCodec *codec = QTextCodec::codecForName("System");
	QTextCodec::setCodecForLocale(codec);

	ASLauncher TheLauncher;
	bool default_theme = ASTransTheme::GetTheme();
	a->connectsns();
	a->slotQSSChange(default_theme);
	int returncode = a->exec();
	if (returncode == -1)
	{
		delete a;
		ASApplication *a = new ASApplication(argc, argv);
		QProcess* proc = new QProcess();
		proc->start(QCoreApplication::applicationFilePath());
	}
	return returncode;
}
