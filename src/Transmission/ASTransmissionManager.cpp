#include "ASTransmissionManager.h"
#include "ASTransGlobalKeyBoard.h"
#include "ASTransAddDeleteData.h"
#include "ASTransDICOMLoader.h"
#include "ASTransSidebarGeneral.h"
#include "ASTransInteractorStyle.h"
#include "ASTransStatusBarGeneral.h"
#include "ASTransSeeds.h"
#include "ASTransContour.h"
#include "ASTransLandmark.h"
#include "ASTransGraphCut.h"
#include "ASTransPolyDeformation.h"
#include "ASTransGeneralInteractor.h"
#include "ASTransDataGeneral.h"
#include "ASTransAddins.h"
#include "ASTransROI.h"
#include "ASTransCompleter.h"
#include "ASTransTheme.h"
#include "ASTransSetting.h"
#include "ASTransLang.h"

ASTransmissionManager::ASTransmissionManager()
{
	// ×ÓÄ£¿éÊµÀý
	GlobalKeyBoard = new ASTransGlobalKeyBoard();
	AddDeleteData = new ASTransAddDeleteData();
	DICOMLoader = new ASTransDICOMLoader();
	SidebarGeneral = new ASTransSidebarGeneral();
	InteractorStyle = new ASTransInteractorStyle();
	StatusBarGeneral = new ASTransStatusBarGeneral();
	Seeds = new ASTransSeeds();
	Contour = new ASTransContour();
	Landmark = new ASTransLandmark();
	GraphCut = new ASTransGraphCut();
	PolyDeformation = new ASTransPolyDeformation();
	GeneralInteractor = new ASTransGeneralInteractor();
	DataGeneral = new ASTransDataGeneral();
	Segmentation = new ASTransAddins();
	ROI = new ASTransROI();
	Completer = new ASTransCompleter();
	Setting = new ASTransSetting();
	Theme = new ASTransTheme();
	Lang = new ASTransLang();
}

ASTransmissionManager::~ASTransmissionManager()
{

}
