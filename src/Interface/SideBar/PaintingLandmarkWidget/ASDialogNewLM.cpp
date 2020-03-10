#include "ASDialogNewLM.h"
#include "ASTransCompleter.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

ASDialogNewLM::ASDialogNewLM(QWidget *parent)
	: QDialog(parent)
{
	QLabel* label = new QLabel(tr("Please enter landmark name:"), this);
	m_LineEdit = new QLineEdit(this);
	m_ok = new QPushButton(tr("Confirm"), this);
	connect(m_ok, SIGNAL(clicked()), this, SLOT(slotOK()));
	m_cancel = new QPushButton(tr("Cancel"), this);
	connect(m_cancel, SIGNAL(clicked()), this, SLOT(close()));
	QHBoxLayout* layout3 = new QHBoxLayout();
	layout3->addStretch();
	layout3->addWidget(m_ok);
	layout3->addWidget(m_cancel);
	QVBoxLayout* mainlayout = new QVBoxLayout();
	mainlayout = new QVBoxLayout();
	mainlayout->addWidget(label);
	mainlayout->addWidget(m_LineEdit);
	mainlayout->addLayout(layout3);
	this->setLayout(mainlayout);

	QList<structCompleterItem*>* pList = ASTransCompleter::GetLandmarkList();
	QStringList StrList;
	for (int index = 0; index < pList->size(); index++)
	{
		StrList << pList->at(index)->name;
	}
	listmodel.setStringList(StrList);
	completer.setModel(&listmodel);
	completer.setCaseSensitivity(Qt::CaseInsensitive);
	m_LineEdit->setCompleter(&completer);
	EventFilterLM* filter = new EventFilterLM(m_LineEdit);
	completer.popup()->installEventFilter(filter);
	connect(filter, SIGNAL(EnterPress()), this, SLOT(slotOK()));
}

ASDialogNewLM::~ASDialogNewLM()
{
}

// È·¶¨
void ASDialogNewLM::slotOK()
{
	m_result = m_LineEdit->text();
	this->close();
	//QList<structCompleterItem*>* pList = ASTransCompleter::GetLandmarkList();
	//for (int index = 0; index < pList->size(); index++)
	//{
	//	if (pList->at(index)->name == m_LineEdit->text())
	//	{
	//		m_result = m_LineEdit->text();
	//		this->close();
	//	}
	//}
}
