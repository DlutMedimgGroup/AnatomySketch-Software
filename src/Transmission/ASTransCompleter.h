#pragma once
#include "ASTransmissionBase.h"
#include <QSettings>
#include <QColor>

typedef struct structCompleterItem
{
	QString name;
	int id;
	QColor color;
}structCompleterItem;

class ASTransCompleter : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransCompleter(QObject *parent = 0);
	~ASTransCompleter();

	// 全局对象
	static ASTransCompleter* GetSelfPointer();
	static QList<structCompleterItem*>* GetLandmarkList();
	static QList<structCompleterItem*>* GetStructureList();

private:
	// 唯一对象
	static ASTransCompleter* ms_SelfPointer;

	void InitConfiguration();
	// 内部函数
	void UpdateGroup(QSettings* settings, QString group, QList<structCompleterItem*>* plist); 
	
	QList<structCompleterItem*>* m_plistLandmarksItems;
	QList<structCompleterItem*>* m_plistStructuresItems;

signals:

};
