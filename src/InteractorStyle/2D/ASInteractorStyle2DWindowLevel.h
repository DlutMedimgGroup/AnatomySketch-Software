#pragma once

#include "ASInteractorStyle2DGeneral.h"
#include "ASArrayImageData.h"

// !!!弃用!!!

class ASInteractorStyle2DWindowLevel : public ASInteractorStyle2DGeneral
{
public:
	static ASInteractorStyle2DWindowLevel *New();
	vtkTypeMacro(ASInteractorStyle2DWindowLevel, ASInteractorStyle2DGeneral);

	// 重写左键拖动部分
	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();

private:
	ASInteractorStyle2DWindowLevel();
	~ASInteractorStyle2DWindowLevel();

	//窗位窗宽调整
	double m_LevelWindowInitial[2];
	double m_LevelWindowStartPosition[2];
	double m_LevelWindowCurrentPosition[2];
	// 当前顶层体数据
	ASArrayImageData* m_crntTopData;


};
