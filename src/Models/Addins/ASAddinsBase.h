#pragma once

#include <QString>
#include <QStringList>
#include <QList>
#include <QObject>
#include <QMessageBox>

class ASAddinsInputBase;
class ASAddinsOutputBase;
class ASAddinsIOBase;
class ASThreadAddins;
class ASOperation;

#define AddinType_ImgProcess	0
#define AddinType_MeshProcess	1
#define AddinType_Segmentation	2

#define InterfaceType_DLL		0
#define InterfaceType_Python	1

typedef struct structArgEle
{
	ASAddinsIOBase* IOEle;
	QString Discri;
}structArgEle;

class ASAddinsBase : public QObject
{
	Q_OBJECT
	
public:
	ASAddinsBase(ASOperation* host, QObject *parent = 0);
	~ASAddinsBase();

	// Type
	int GetType();
	// Name
	void SetName(const QString c_name);
	QString GetName();
	// Model type
	void SetAddinType(const int c_type);
	int GetAddinType();
	// Interface type
	void SetInterfaceType(const int c_type);
	int GetInterfaceType();
	// Host
	ASOperation* GetHost();
	// DLL
	void SetDLL(const QString c_name);
	QString GetDLL();
	// Python
	void SetPython(const QString c_name);
	QString GetPython();
	// Description
	void SetDescription(const QString c_description);
	QString GetDescription();
	// Input
	void AddInput(const int c_type, const QStringList c_value);
	void AddInput(const QString c_typename, const QStringList c_value);
	int GetNumOfInput();
	ASAddinsInputBase* GetInputAt(const int c_index);
	// Output
	void AddOutput(const int c_type, const QString c_name);
	void AddOutput(const QString c_typename, const QString c_name);
	int GetNumOfOutput();
	ASAddinsOutputBase* GetOutputAt(const int c_index);
	// interface. type: 0, argv; 1, PYEntrence; 2, PYExport
	void AppendInterface(const QString c_s, const int c_type);

	// ‘ÀÀ„
	virtual void Generate();

protected:

	int m_Type;
	QString m_Name;
	int m_AddinType = 0; // AddinType_ImgProcess
	int m_InterfaceType = -1;	// 0, DLL; 1, python
	QString m_DLL;
	QString m_Python;
	QString m_Description;
	ASOperation* m_Host;
	
	QList<ASAddinsInputBase*>* m_listInputs;
	QList<ASAddinsOutputBase*>* m_listOutputs;
	QList<structArgEle*>* m_listArgv;
	QList<structArgEle*>* m_listPYEntrence;
	QList<structArgEle*>* m_listPYExport;

private:
	ASThreadAddins* m_ThreadAddin = nullptr;

	QMessageBox* m_MessageBox = nullptr;

private slots:

	void slotThreadFinished(float RValue);
	void slotCancel(QAbstractButton* b);
};
