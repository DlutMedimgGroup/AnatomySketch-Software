#include "ASLauncher.h"
#include "ASMainWidget.h"
#include "ASTransmissionManager.h"
#include "ASDataManager.h"
#include "ASVisualizationManager.h"
#include "ASInteractorStyleManager.h"
#include "ASTransAddDeleteData.h"
#include <QMutex>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QStringList>
#include <QFileInfo>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QMutex mutex;
	mutex.lock();
	QString text;
	switch (type)
	{
	case QtDebugMsg:
		text = QString("Debug:");
		break;
	case QtWarningMsg:
		text = QString("Warning:");
		break;
	case QtCriticalMsg:
		text = QString("Critical:");
		break;
	case QtFatalMsg:
		text = QString("Fatal:");
	}
	QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString current_date = QString("(%1)").arg(current_date_time);
	QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);
	QFile file("log.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream text_stream(&file);
	text_stream << message << "\r\n";
	file.flush();
	file.close();
	mutex.unlock();

	QCoreApplication::setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, true);
	QCoreApplication::setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents, true);
}

ASLauncher::ASLauncher(QObject *parent)
	: QObject(parent)
{	
	theTransmissionManager = new ASTransmissionManager;
	theMainWidget = new ASMainWidget; // 在theTransmissionManager之后，界面需要连接连接模块发送的信号
	theDataManager = new ASDataManager; // 在theTransmissionManager之后，界面需要连接连接模块发送的信号
	theVisualizationManager = new ASVisualizationManager; // 在theMainWidget实例之后，需要ASQVTKWidget指针
	theInteractorStyleManager = new ASInteractorStyleManager; // 在theMainWidget实例之后，需要ASQVTKWidget指针；在theVisualizationManager实例之后，需要Renderer指针
	theMainWidget->show();

	// 命令行传参打开文件
	QStringList argv = QCoreApplication::arguments();
	argv.removeFirst();
	QStringList seedFileNames;
	QStringList ctrFileNames;
	QStringList ldmFileNames;
	QStringList roiFileNames;
	for (int nit = 0; nit < argv.size();)
	{
		QFileInfo pFileInfo = QFileInfo(argv.at(nit));
		QString qstrSuffix = pFileInfo.suffix();
		if (strcmp(qstrSuffix.toStdString().c_str(), "seed") == 0)
		{
			seedFileNames.append(argv.at(nit));
			argv.removeAt(nit);
		}
		else if (strcmp(qstrSuffix.toStdString().c_str(), "ctr") == 0)
		{
			ctrFileNames.append(argv.at(nit));
			argv.removeAt(nit);
		}
		else if (strcmp(qstrSuffix.toStdString().c_str(), "ldm") == 0)
		{
			ldmFileNames.append(argv.at(nit));
			argv.removeAt(nit);
		}
		else if (strcmp(qstrSuffix.toStdString().c_str(), "roi") == 0)
		{
			roiFileNames.append(argv.at(nit));
			argv.removeAt(nit);
		}
		else
		{
			nit++;
		}
	}
	ASTransAddDeleteData::AddLoadWaitingArea(seedFileNames, 0);
	ASTransAddDeleteData::AddLoadWaitingArea(ctrFileNames, 1);
	ASTransAddDeleteData::AddLoadWaitingArea(ldmFileNames, 2);
	ASTransAddDeleteData::AddLoadWaitingArea(roiFileNames, 3);
	ASTransAddDeleteData::LoadDataFromDisk(argv);

	// 生成日志
	qInstallMessageHandler(outputMessage);//注册MessageHandler
	QFile file("log.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream text_stream(&file);
	text_stream << "" << "\r\n";
	text_stream << "" << "\r\n";
	text_stream << "" << "\r\n";
	text_stream << tr("------------ Start ------------") << "\r\n";
	QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
	QString current_date = QString("(%1)").arg(current_date_time);
	text_stream << tr("Start time：") << current_date << "\r\n";
	text_stream << "" << "\r\n";
	file.flush();
	file.close();
}

ASLauncher::~ASLauncher()
{

}

