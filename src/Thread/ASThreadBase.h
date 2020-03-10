#pragma once
#include <QThread>

class ASThreadBase : public QThread
{
	Q_OBJECT

public:
	ASThreadBase(QObject *parent = 0);
	~ASThreadBase();



private:

};
