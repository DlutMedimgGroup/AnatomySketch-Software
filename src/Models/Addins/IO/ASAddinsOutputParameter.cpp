#include "ASAddinsOutputParameter.h"
#include "ASAddinsBase.h"
#include <QFileInfo>
#include <QTextStream>

ASAddinsOutputParameter::ASAddinsOutputParameter(ASAddinsBase* parent)
	: ASAddinsOutputNode(parent)
{
	m_Type = AddinOutput_Parameter;
	m_LineEdit = new QLineEdit();
	m_LineEdit->setFocusPolicy(Qt::NoFocus);
}

ASAddinsOutputParameter::~ASAddinsOutputParameter()
{

}

// 得到空指针，用于argc，s为描述
void* ASAddinsOutputParameter::GetVoidPoint(const QString s)
{
	if (s == "")
	{
		return &m_dPara;
	}
	else
	{
		return nullptr;
	}
}
void ASAddinsOutputParameter::GenerateFinished(const int InterfaceType)
{
	if (InterfaceType == InterfaceType_DLL)
	{
		m_LineEdit->setText(QString::number(m_dPara));
	}
	else if (InterfaceType == InterfaceType_Python)
	{
		QString path = _pgmptr;
		QFileInfo fi = QFileInfo(path);
		QString file_path = fi.absolutePath();
		file_path = file_path + "/Plugins/_datacacke";
		file_path = file_path + "/" + this->GetName() + ".txt";

		QFile file(file_path);
		if (!file.exists())
		{
			return; //数据文件不存在！
		}
		QStringList StringList;
		if (file.open(QIODevice::ReadOnly))
		{
			while (!file.atEnd())
				StringList << file.readLine();

			file.close();
		}
		m_LineEdit->setText(StringList.first());
	}
}
// QLineEdit
QLineEdit* ASAddinsOutputParameter::GetLineEdit()
{
	return m_LineEdit;
}