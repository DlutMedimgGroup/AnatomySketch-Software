#include "ASThreadDICOMScanner.h"
#include <vtkDICOMItem.h>

ASThreadDICOMScanner::ASThreadDICOMScanner(QObject *parent)
	: ASThreadBase(parent)
{
	this->setTerminationEnabled();
}

ASThreadDICOMScanner::~ASThreadDICOMScanner()
{

}

void ASThreadDICOMScanner::SetScanPath(const QString c_ScanPath)
{
	m_ScanningPath = c_ScanPath;
}
void ASThreadDICOMScanner::SetDICOMDirectory(vtkDICOMDirectory* DICOMDirectory)
{
	m_DICOMDirectory = DICOMDirectory;
}
void ASThreadDICOMScanner::SetMutex(QMutex* Mutex)
{
	m_MutexForDICOMDirectory = Mutex;
}

void ASThreadDICOMScanner::run()
{
	std::string str = m_ScanningPath.toStdString();
	const char* ch = str.c_str();
	m_MutexForDICOMDirectory->lock();
	m_DICOMDirectory->SetDirectoryName(ch);
	m_DICOMDirectory->SetScanDepth(10);
	m_DICOMDirectory->Update();
	m_MutexForDICOMDirectory->unlock();
	emit signalScanningFinished();
	this->exit();
}
