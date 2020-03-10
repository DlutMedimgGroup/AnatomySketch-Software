#pragma once

#include <QItemDelegate>
#include <QObject>

class ASDataManagerDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ASDataManagerDelegate(QObject *parent = 0);
	void setSizeOffset(const int c_nOff);
	int getCrntH();

protected:

	virtual QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

private:
	int m_SizeOffset = 0;
	int m_crntH  = 17;
};
