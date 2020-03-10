#pragma once

#include <QString>

class ASSeedData;
class ASContourData;
class ASLandmarkData;
class ASROIData;

class ASCustomizeDataWriter
{
public:
	ASCustomizeDataWriter();
	~ASCustomizeDataWriter();

	void WriteSeedData(ASSeedData* DataNode, QString FilePath);
	void WriteContourData(ASContourData* DataNode, QString FilePath);
	void WriteLandmarkData(ASLandmarkData* DataNode, QString FilePath);
	void WriteROIData(ASROIData* DataNode, QString FilePath);

private:


};
