#pragma once
#include <QString>

class ASAddinsBase;

#define AddinInput_Base			100
#define AddinInput_Node			101
#define AddinInput_ArrayImage	102
#define AddinInput_PolyImage	103
#define AddinInput_Mask			104
#define AddinInput_Landmark		105
#define AddinInput_Contour		106
#define AddinInput_Seed			107
#define AddinInput_ROI			108
#define AddinInput_Parameter	109
#define AddinInput_Switch		110

#define AddinOutput_Base		200
#define AddinOutput_Node		201
#define AddinOutput_ArrayImage	202
#define AddinOutput_PolyImage	203
#define AddinOutput_Parameter	204

class ASAddinsIOBase
{
public:
	ASAddinsIOBase(ASAddinsBase* parent);
	~ASAddinsIOBase();

	// Type
	int GetType();
	virtual bool isInput();
	// Name
	void SetName(const QString c_name);
	QString GetName();
	// 得到空指针，用于argc，s为描述
	virtual void* GetVoidPoint(const QString s);
	// 保存输入到Python程序中的临时文件.1成功，-1失败
	virtual int PythonInputGenerate();
	// 运算后释放数据
	virtual void FreeMemory();

protected:

	int m_Type;
	QString m_Name;

	ASAddinsBase* m_Parent = nullptr;

private:


};
