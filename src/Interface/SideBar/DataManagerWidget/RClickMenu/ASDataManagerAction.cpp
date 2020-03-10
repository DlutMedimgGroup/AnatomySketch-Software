#include "ASDataManagerAction.h"

ASDataManagerAction::ASDataManagerAction(QObject * parent, enumActionInContextMenu action)
	: QAction(parent)
{
	m_Action = action;
}
ASDataManagerAction::ASDataManagerAction(const QString & text, QObject * parent, enumActionInContextMenu action)
	: QAction(text, parent)
{
	m_Action = action;
}
ASDataManagerAction::ASDataManagerAction(const QIcon & icon, const QString & text, QObject * parent, enumActionInContextMenu action)
	: QAction(icon, text, parent)
{
	m_Action = action;
}

ASDataManagerAction::~ASDataManagerAction()
{
}

enumActionInContextMenu ASDataManagerAction::getAction()
{
	return m_Action;
}