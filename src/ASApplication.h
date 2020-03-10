#pragma once

#include <QApplication>
#include <QObject>

class ASApplication : public QApplication
{
	Q_OBJECT
public:
     ASApplication(int&argc,char **argv);
     ~ASApplication();

     bool notify(QObject*, QEvent *);

	 // 在ASLauncher生成之后才能连接信号槽
	 void connectsns();

private:
	QWidget* mainWindow;
	QTranslator *zhTranslator;

public slots:

	// 换肤的槽
	void slotQSSChange(bool checked);
	// 换语言的槽
	void slotLangChange(bool checked);

};
