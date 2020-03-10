#include "ASDataManagerDelegate.h"


ASDataManagerDelegate::ASDataManagerDelegate(QObject *parent)
	: QItemDelegate(parent)
{


}

void ASDataManagerDelegate::setSizeOffset(const int c_nOff)
{
	m_SizeOffset += c_nOff;
	if (m_SizeOffset < 0)
	{
		m_SizeOffset = 0;
	}
	int h = 17;
	int s = h / 10 + 1;
	h += m_SizeOffset * s;
	m_crntH = h;
}
int ASDataManagerDelegate::getCrntH()
{
	return m_crntH;
}

QSize ASDataManagerDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QSize sz = QItemDelegate::sizeHint(option, index);
	int h = sz.height();
	int s = h / 10 + 1;
	h += m_SizeOffset * s;
	if (h < 5) h = 5;
	sz.setHeight(h);
	return sz;
}