#pragma once

#include "ASInteractorStyle2DGeneral.h"

class ASInteractorStyle2DROICube : public ASInteractorStyle2DGeneral
{
public:
	static ASInteractorStyle2DROICube *New();
	vtkTypeMacro(ASInteractorStyle2DROICube, ASInteractorStyle2DGeneral);

	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnKeyPress();
	virtual void OnKeyRelease();

private:
	ASInteractorStyle2DROICube();
	~ASInteractorStyle2DROICube();

	// °´¼ü×´Ì¬×´Ì¬
	bool m_isKey_w_down = false;
	bool m_isKey_a_down = false;
	bool m_isKey_s_down = false;
	bool m_isKey_d_down = false;
	// ·½ÏòÓ³Éä
	int DirectionMapping[3][4] = { { 2, 3, 4, 5 } ,{ 0, 1, 4, 5 } ,{ 0, 1, 3, 2 } };

	bool m_isNormal = true;
};
