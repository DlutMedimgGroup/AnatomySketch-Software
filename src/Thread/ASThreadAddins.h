#pragma once
#include "ASThreadBase.h"

class ASThreadAddins : public ASThreadBase
{
	Q_OBJECT

public:
	ASThreadAddins(QObject *parent = 0);
	~ASThreadAddins();

	void SetArgc(const int c_argc);
	void SetArgv(void** argv);
	void SetDLL(const QString c_DLL);
	void SetPython(const QString c_Python);
	int GetInferenceType();

protected:
	void run();

private:

	int m_Inference = -1;
	int m_Argc;
	void** m_Argv;
	QString m_DLL;
	QString m_Python;

signals:

	void signalFinished(float RValue);
};
