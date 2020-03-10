#include "ASTransCompleter.h"
#include <QCoreApplication>
#include <QDir>
#include <QVariant>
#include <QString>

ASTransCompleter* ASTransCompleter::ms_SelfPointer = nullptr;

ASTransCompleter::ASTransCompleter(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_SelfPointer = this;
	m_plistLandmarksItems = new QList<structCompleterItem *>;
	m_plistStructuresItems = new QList<structCompleterItem *>;
	this->InitConfiguration();
}

ASTransCompleter::~ASTransCompleter()
{

}

// 全局对象
ASTransCompleter* ASTransCompleter::GetSelfPointer()
{
	return ms_SelfPointer;
}

QList<structCompleterItem*>* ASTransCompleter::GetLandmarkList()
{
	return ms_SelfPointer->m_plistLandmarksItems;
}
QList<structCompleterItem*>* ASTransCompleter::GetStructureList()
{
	return ms_SelfPointer->m_plistStructuresItems;
}

void ASTransCompleter::InitConfiguration()
{
	QString ConfPath = QCoreApplication::applicationDirPath() + "/Configurations";
	QString filename = ConfPath + "/StructureNameLibrary.ini";
	QSettings* configIniRead = new QSettings(filename, QSettings::IniFormat);
	this->UpdateGroup(configIniRead, "Landmark", m_plistLandmarksItems);
	this->UpdateGroup(configIniRead, "Structure", m_plistStructuresItems);
}
void ASTransCompleter::UpdateGroup(QSettings* settings, QString group, 
	QList<structCompleterItem*>* plist)
{
	// 清空
	for (int item = 0; item < plist->size(); item++)
	{
		delete plist->at(item);
	}
	plist->clear();
	// 添加
	settings->beginGroup(group);
	QStringList crntKeys = settings->allKeys();
	int size = crntKeys.size();
	for (int index = 0; index < size; index++)
	{
		QStringList Value = settings->value(crntKeys.at(index)).toStringList();
		if (Value.size() != 4 && Value.size() != 5)
		{
			QString s = "error: Loading " + group + "/" + crntKeys.at(index) +
				" in StructureNameLibrary.ini";
			qDebug(qPrintable(s));
			continue;
		}
		else
		{
			structCompleterItem* crntItem = new structCompleterItem;
			crntItem->name = crntKeys.at(index);
			crntItem->id = Value.at(0).toInt();
			crntItem->color.setRed(Value.at(1).toInt());
			crntItem->color.setGreen(Value.at(2).toInt());
			crntItem->color.setBlue(Value.at(3).toInt());
			if (Value.size() == 5)
			{
				crntItem->color.setAlpha(Value.at(4).toInt());
			}
			plist->append(crntItem);
		}
	}
	settings->endGroup();
}