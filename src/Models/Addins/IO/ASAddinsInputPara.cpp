#include "ASAddinsInputPara.h"
#include <QTextCodec>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

ASAddinsInputPara::ASAddinsInputPara(ASAddinsBase* parent)
	: ASAddinsInputBase(parent)
{
	m_Type = AddinInput_Parameter;
	m_DouSpinBox = new QDoubleSpinBox();
	m_DouSpinBox->setMaximum(999999999);
	m_DouSpinBox->setMinimum(-999999999);
}

ASAddinsInputPara::~ASAddinsInputPara()
{
	m_DouSpinBox->deleteLater();
}

// Value
void ASAddinsInputPara::SetValue(const QStringList c_value)
{
	QStringList crntSL = c_value.first().split("&");
	m_Name = crntSL.first();
	m_DouSpinBox->setValue(crntSL.last().toDouble());
}
// QDoubleSpinBox
QDoubleSpinBox* ASAddinsInputPara::GetDouSpinBox()
{
	return m_DouSpinBox;
}
// 得到空指针，用于argc，s为描述
void* ASAddinsInputPara::GetVoidPoint(const QString s)
{
	if (s == "")
	{
		m_dPara = m_DouSpinBox->value();
		return &m_dPara;
	}
	else
	{
		return nullptr;
	}
}
// 保存输入到Python程序中的临时文件.1成功，-1失败
int ASAddinsInputPara::PythonInputGenerate()
{
	m_dPara = m_DouSpinBox->value();
	QString path = _pgmptr;
	QFileInfo fi = QFileInfo(path);
	QString file_path = fi.absolutePath();
	file_path = file_path + "/Plugins/_datacacke";
	QDir dir(file_path);
	if (dir.exists() == false)
	{
		dir.mkpath(file_path);
	}
	file_path = file_path + "/" + this->GetName() + ".txt";
	QFile data(file_path);
	if (data.open(QFile::WriteOnly | QIODevice::Truncate))
	{
		QTextStream out(&data);
		out << QString::number(m_dPara);
		return 1;
	}
	else
	{
		return -1;
	}	
}
