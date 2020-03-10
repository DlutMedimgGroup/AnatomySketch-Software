#pragma once

#include "ASClicklabel.h"
#include <QWidget>
#include <QStandardItem>
#include <QCheckBox>
#include <QLabel>
#include <QPixmap>

#define ItemType_Abstract	0
#define ItemType_DataNode	1
#define ItemType_MarkEle	2
#define ItemType_Contour	3
#define ItemType_Seed		4
#define ItemType_Landmark	5
#define ItemType_ROI		6

class ASAbstructItemWidget : public QWidget
{
	Q_OBJECT

public:
	ASAbstructItemWidget(QWidget *parent);
	~ASAbstructItemWidget();

	// ItemType
	int GetItemType();
	virtual bool IsMarkEle();
	// ListWidgetItem
	QStandardItem* GetListWidgetItem();
	void SetListWidgetItem(QStandardItem* item);
	// ID
	void SetID(int ID);
	int GetID();
	virtual int getMarkEleID();
	// Checked
	virtual void SetChecked(bool checked);
	bool GetChecked();
	// Font
	void SetSizeHWithUpdate(const int c_nH);
	void SetBold(const bool c_b);
	void SetBoldWithUpdate(const bool c_b);	
	// Name
	void SetName(QString name);
	QString GetName();
	// Icon
	void SetIcon(QPixmap icon);	
	
protected:

	int m_ItemType = -1;
	//QCheckBox*	m_pCheckbox;
	QLabel*		m_pLabelForName;
	ASClicklabel* m_pLabelForIcon;
	QPixmap		m_Icon;
	bool		m_isBold = false;
	int			m_nH = 17;
	// 对应数据节点的ID
	int	m_ID = 0;
	bool m_Checked = true;

private:	

	QStandardItem* m_ListWidgetItem;

protected slots:

	virtual void slotCheckboxStateChanged(int state);
	virtual void slotIconClicked();
};
