#pragma once
#include <QObject>

class ASTransGlobalKeyBoard;
class ASTransAddDeleteData;
class ASTransDICOMLoader;
class ASTransSidebarGeneral;
class ASTransInteractorStyle;
class ASTransStatusBarGeneral;
class ASTransSeeds;
class ASTransContour;
class ASTransLandmark;
class ASTransGraphCut;
class ASTransPolyDeformation;
class ASTransGeneralInteractor;
class ASTransDataGeneral;
class ASTransAddins;
class ASTransROI;
class ASTransCompleter;
class ASTransAddins;
class ASTransTheme;
class ASTransLang;
class ASTransSetting;

class ASTransmissionManager
{
public:
	ASTransmissionManager();
	~ASTransmissionManager();

private:

	// всдё©И
	ASTransGlobalKeyBoard* GlobalKeyBoard;
	ASTransAddDeleteData* AddDeleteData;
	ASTransDICOMLoader* DICOMLoader;
	ASTransSidebarGeneral* SidebarGeneral;
	ASTransInteractorStyle* InteractorStyle;
	ASTransStatusBarGeneral* StatusBarGeneral;
	ASTransSeeds* Seeds;
	ASTransContour* Contour;
	ASTransLandmark* Landmark;
	ASTransGraphCut* GraphCut;
	ASTransPolyDeformation* PolyDeformation;
	ASTransGeneralInteractor* GeneralInteractor;
	ASTransDataGeneral* DataGeneral;
	ASTransAddins* Segmentation;
	ASTransROI* ROI;
	ASTransCompleter* Completer;
	ASTransAddins* Addins;
	ASTransTheme* Theme;
	ASTransSetting* Setting;
	ASTransLang* Lang;
};
