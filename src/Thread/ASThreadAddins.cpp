#include "ASThreadAddins.h"
#include <Windows.h>
#include <iostream>
#include <QDebug>
#include <QProcess>
#include "stdlib.h"
#include <QFileInfo>

ASThreadAddins::ASThreadAddins(QObject *parent)
	: ASThreadBase(parent)
{

}

ASThreadAddins::~ASThreadAddins()
{

}

void ASThreadAddins::SetArgc(const int c_argc)
{
	m_Argc = c_argc;
}
void ASThreadAddins::SetArgv(void** argv)
{
	m_Argv = argv;
}
void ASThreadAddins::SetDLL(const QString c_DLL)
{
	m_DLL = c_DLL;
	m_Inference = 0;
}
void ASThreadAddins::SetPython(const QString c_Python)
{
	m_Python = c_Python;
	m_Inference = 1;
}
int ASThreadAddins::GetInferenceType()
{
	return m_Inference;
}

void ASThreadAddins::run()
{
	if (m_Inference < 0)
	{
		return;
	}
	else if (m_Inference == 0)
	{
		// DLL
		typedef float(*pAdd)(int argc, void* argv[]);
		QString s = ".\\Plugins\\" + m_DLL + ".dll";
		QByteArray ba = s.toLocal8Bit();
		HINSTANCE hDLL = LoadLibrary(s.toStdString().c_str());
		if (hDLL)
		{
			pAdd pFun = (pAdd)GetProcAddress(hDLL, "ASAddinEntrance"); // 获取DLL中需要调用的函数的地址
			float f = pFun(m_Argc, m_Argv);
			emit signalFinished(f);
		}
	}
	else if (m_Inference == 1)
	{
		// Python		
		QString path = _pgmptr;
		QFileInfo fi = QFileInfo(path);
		QString file_path = fi.absolutePath();
		file_path = file_path + "/Plugins/" + m_Python + ".py";
		QString commend = "python " + file_path;

		QProcess process(this);
		process.setProgram("cmd");
		QStringList argument;
		argument << "/c" << commend;
		//argument << commend;
		process.setArguments(argument);
		process.start();
		process.waitForStarted(); //等待程序启动
		process.waitForFinished();//等待程序关闭
		QString temp = QString::fromLocal8Bit(process.readAllStandardOutput()); //程序输出信息

		emit signalFinished(1);
	}
}
