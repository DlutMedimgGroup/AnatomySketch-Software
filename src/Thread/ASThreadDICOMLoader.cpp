#include "ASThreadDICOMLoader.h"
#include "ASDICOMData.h"
#include <vtkDICOMReader.h>
#include <vtkDICOMApplyRescale.h>
#include <vtkDICOMMetaData.h>
#include <vtkMath.h>
#include <vtkImageReslice.h>
#include <vtkDICOMTagPath.h>

ASThreadDICOMLoader::ASThreadDICOMLoader(QObject *parent)
	: ASThreadBase(parent)
{
	this->setTerminationEnabled();
}

ASThreadDICOMLoader::~ASThreadDICOMLoader()
{

}
void ASThreadDICOMLoader::SetDICOMDirSeries(vtkDICOMDirectory* dir, const int c_Series)
{
	m_DICOMDirectory = dir;
	m_Series = c_Series;
}

void ASThreadDICOMLoader::run()
{
	ASDICOMData* NewData = nullptr;

	int numOfStudy = m_DICOMDirectory->GetNumberOfStudies();
	for (int crntStudy = 0; crntStudy < numOfStudy; crntStudy++)
	{
		int j1 = m_DICOMDirectory->GetFirstSeriesForStudy(crntStudy);
		int j2 = m_DICOMDirectory->GetLastSeriesForStudy(crntStudy);
		if (m_Series >= j1 && m_Series <= j2)
		{
			NewData = new ASDICOMData();

			// 1 使用DICOMReader读入序列
			vtkStringArray *sortedFiles = m_DICOMDirectory->GetFileNamesForSeries(m_Series);
			vtkSmartPointer<vtkDICOMReader> DICOMReader = vtkSmartPointer<vtkDICOMReader>::New();
			DICOMReader->SetFileNames(sortedFiles);
			DICOMReader->SetMemoryRowOrderToFileNative();
			DICOMReader->AutoRescaleOff();
			/*
			http://dgobbi.github.io/vtk-dicom/doc/api/image_reader.html
			If accurate pixel values are required (which is the case in most medical applications), it is recommended that AutoRescaleOff() is used.
			The rescaling of the pixel values to "real" values should be done with the vtkDICOMApplyRescale filter, which will produce pixel values
			as "double" (or, optionally, as "float") after applying the RescaleSlope and the RescaleIntercept for each slice.
			*/
			DICOMReader->TimeAsVectorOn();
			/*
			http://dgobbi.github.io/vtk-dicom/doc/api/image_reader.html
			When a time dimension is present, things become interesting. The default behavior of the reader is to store adjacent time
			points in adjacent vtkImageData slices. This works well when the images are to be displayed slice-by-slice. It is, however,
			inappropriate if the vtkImageData is to be displayed as a multi-planar reformat or as a volume. For this reason, the vtkDICOMReader
			has a method called TimeAsVectorOn() that will cause the reader to treat each voxel as a time vector. In other words, if the
			DICOM data has 10 individual time slots, then the vtkImageData will have 10 components per voxel (or 30 components in the
			case of RGB data). By selecting a specific component or range of components when displaying the data, one can display a specific point in time.
			*/
			DICOMReader->Update();
			unsigned long errorcode = DICOMReader->GetErrorCode();
			if (errorcode != 0)
			{
				// 该序列读取失败
				emit signalDataLoadFinished(nullptr, -1);
				return;
			}
			vtkSmartPointer<vtkDICOMApplyRescale> rescale = vtkSmartPointer<vtkDICOMApplyRescale>::New();
			rescale->SetInputConnection(DICOMReader->GetOutputPort());
			rescale->SetOutputScalarTypeToFloat();
			rescale->Update();

			// 2 得到变换矩阵
			vtkSmartPointer<vtkMatrix4x4> TransformMatrix;
			vtkSmartPointer<vtkDICOMMetaData> DICOMMetaData = vtkSmartPointer<vtkDICOMMetaData>::New();
			DICOMMetaData = DICOMReader->GetMetaData();
			vtkDICOMValue pv = DICOMMetaData->Get(0, 0, DC::ImagePositionPatient);
			vtkDICOMValue ov = DICOMMetaData->Get(0, 0, DC::ImageOrientationPatient);
			if (pv.IsValid() && ov.IsValid())
			{
				TransformMatrix = DICOMReader->GetPatientMatrix();
			}
			else
			{
				pv = DICOMMetaData->GetAttributeValue(vtkDICOMTagPath(DC::DetectorInformationSequence, 0, DC::ImagePositionPatient));
				ov = DICOMMetaData->GetAttributeValue(vtkDICOMTagPath(DC::DetectorInformationSequence, 0, DC::ImageOrientationPatient));
				if (pv.IsValid() && ov.IsValid())
				{
					int rows = DICOMMetaData->Get(0, DC::Rows).AsInt();

					std::vector<double> points;
					double centroid[3] = { 0.0, 0.0, 0.0 };
					double orient[6] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0 };
					double point[3] = { 0.0, 0.0, 0.0 };
					double normal[3] = { 0.0, 0.0, 1.0 };
					pv.GetValues(point, 3);
					ov.GetValues(orient, 6);
					vtkMath::Cross(&orient[0], &orient[3], normal);
					if (vtkMath::Normalize(normal) < 1e-10)
					{
						orient[0] = 1.0; orient[1] = 0.0; orient[2] = 0.0;
						orient[3] = 0.0; orient[4] = 1.0; orient[5] = 0.0;
						normal[0] = 0.0; normal[1] = 0.0; normal[2] = 1.0;
					}
					// re-orthogonalize x vector (improve precision)
					vtkMath::Cross(&orient[3], normal, &orient[0]);
					vtkMath::Normalize(&orient[0]);
					vtkMath::Normalize(&orient[3]);

					if (DICOMReader->GetMemoryRowOrder() == vtkDICOMReader::BottomUp)
					{
						// calculate position of point at lower left			
						double yspacing = DICOMReader->GetDataSpacing()[1];
						point[0] = point[0] + orient[3] * yspacing*(rows - 1);
						point[1] = point[1] + orient[4] * yspacing*(rows - 1);
						point[2] = point[2] + orient[5] * yspacing*(rows - 1);

						orient[3] = -orient[3];
						orient[4] = -orient[4];
						orient[5] = -orient[5];
					}

					size_t ip = points.size();
					points.resize(ip + 3);
					for (int ii = 0; ii < 3; ii++)
					{
						centroid[ii] += point[ii];
						points[ip + ii] = point[ii];
					}

					// compute Z vector by fitting points to a line
					double vector[3] = { normal[0], normal[1], normal[2] };
					size_t nPoints = points.size() / 3;

					centroid[0] /= nPoints;
					centroid[1] /= nPoints;
					centroid[2] /= nPoints;

					// use Jacobi to compute line of best fit
					double storage[18] = {};
					double *A[3] = { &storage[0], &storage[3], &storage[6] };
					double *E[3] = { &storage[9], &storage[12], &storage[15] };
					for (size_t iPoint = 0; iPoint < nPoints; iPoint++)
					{
						for (int ii = 0; ii < 3; ii++)
						{
							for (int jj = 0; jj < 3; jj++)
							{
								A[ii][jj] = ((points[3 * iPoint + ii] - centroid[ii]) *
									(points[3 * iPoint + jj] - centroid[jj]));
							}
						}
					}
					double eigenvalues[3];
					vtkMath::Jacobi(A, eigenvalues, E);

					// only use eigenvector if the points fit a line very precisely
					if (eigenvalues[1] * eigenvalues[1] + eigenvalues[2] * eigenvalues[2] <
						1e-6*eigenvalues[0] * eigenvalues[0])
					{
						// create the vector, dot(vector,normal) should be unity
						double vdn = E[0][0] * normal[0] + E[1][0] * normal[1] + E[2][0] * normal[2];
						if (vdn > 0)
						{
							vector[0] = E[0][0] / vdn;
							vector[1] = E[1][0] / vdn;
							vector[2] = E[2][0] / vdn;

							// check difference between this vector and the normal
							double vcn[3];
							vtkMath::Cross(vector, normal, vcn);
							if (vtkMath::Norm(vcn) < 1e-4)
							{
								// use normal (more precise) if they are the same
								vector[0] = normal[0];
								vector[1] = normal[1];
								vector[2] = normal[2];
							}
						}
					}

					// build the patient matrix
					double pm[16];
					pm[0] = orient[0]; pm[1] = orient[3]; pm[2] = vector[0]; pm[3] = point[0];
					pm[4] = orient[1]; pm[5] = orient[4]; pm[6] = vector[1]; pm[7] = point[1];
					pm[8] = orient[2]; pm[9] = orient[5]; pm[10] = vector[2]; pm[11] = point[2];
					pm[12] = 0.0; pm[13] = 0.0; pm[14] = 0.0; pm[15] = 1.0;

					TransformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
					TransformMatrix->DeepCopy(pm);
				}
			}

			// 3 得到经过变换矩阵变换之后的最终图像，保存到数据中
			vtkSmartPointer<vtkImageReslice> ImageReslice = vtkSmartPointer<vtkImageReslice>::New();
			ImageReslice->SetInputData(rescale->GetOutput());
			ImageReslice->SetInformationInput(rescale->GetOutput());
			vtkSmartPointer<vtkMatrix4x4> transResult = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkMatrix4x4::Invert(TransformMatrix, transResult);
			ImageReslice->SetResliceAxes(transResult);
			ImageReslice->SetInterpolationModeToLinear();
			ImageReslice->Update();
			NewData->setArrayImageData(ImageReslice->GetOutput());

			// 4 序列信息
			NewData->SetPatientItem(m_DICOMDirectory->GetPatientRecordForStudy(crntStudy));
			NewData->SetStudyItem(m_DICOMDirectory->GetStudyRecord(crntStudy));
			NewData->SetSeriesItem(m_DICOMDirectory->GetSeriesRecord(m_Series));

			// 5 其他属性
			// 5.1 生成文件名
			const char* pStudyDescription = NewData->GetStudyItem().GetAttributeValue(DC::StudyDescription).GetCharData();
			const char* pSeriesDescription = NewData->GetSeriesItem().GetAttributeValue(DC::SeriesDescription).GetCharData();
			QString dataname = "";
			if (pStudyDescription == NULL)
			{
				dataname += "no study description";
			}
			else
			{
				dataname += pStudyDescription;
			}
			dataname.append("/");
			if (pSeriesDescription == NULL)
			{
				dataname += "no series description";
			}
			else
			{
				dataname += pSeriesDescription;
			}
			// 5.2 设置属性
			NewData->setName(dataname);
			NewData->setLayer(ASArrayImageData::getNumOfArrayImageData());
			NewData->setVoidNode(false);
			NewData->DataInitAfterLoad();
		}
	}
	emit signalDataLoadFinished(NewData, DataType_DICOMData);
}
