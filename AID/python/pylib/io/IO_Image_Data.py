'''
Script name: IO_image_Data
Author:@Mingrui
Created:2020/05/14
Last modified: 2020/05/14
Version:1
Description: Read and write 3D image data or poly data. The data format used is Image_Data and Poly_Data
'''
import SimpleITK as sitk
import vtk
from pylib.utilities.Image_Data import Image_Data
from pylib.utilities.Poly_Data import Poly_Data

def Read_Image_Data(path):
    '''
    Input:
        path: string, full path of data to import. Support mhd, nii, nii.gz, etc.
    Output: Image_Data
    Description: Import 3D image data in various formats (without DICOM).
    '''
    image = sitk.ReadImage(path)
    imgdata = Image_Data()
    imgdata.Init_From_Itk(image)
    return imgdata

def Write_Image_Data(img_data:Image_Data, path):
    '''
    Input:
        img_data: Image_Data, data to write.
        path: string, full path of data to write. Support mhd, nii, nii.gz, etc.
    Output:
    Description: Write 3D image data in various formats (without DICOM).
    '''
    itk_data = img_data.Get_itkImageData()
    sitk.WriteImage(itk_data, path)

def Read_DICOM(path):
    '''
    Input:
        path: string, full path of the folder where the DICOM sequence is located.
    Output: Image_Data
    Description: Import a DICOM data.
    '''
    reader = sitk.ImageSeriesReader()
    dicom_names = reader.GetGDCMSeriesFileNames(path)
    reader.SetFileNames(dicom_names)

    image = reader.Execute()
    imgdata = Image_Data()
    imgdata.Init_From_Itk(image)
    return imgdata

def Read_Poly_Data(path):
    '''
    Input:
        path: string, full path of data to import. Support stl, ply, obj, vtk.
    Output: Poly_Data
    Description: Import poly data in various formats.
    '''
    if path.endswith('.vtk'):
        reader = vtk.vtkPolyDataReader()
    elif path.endswith('.stl'):
        reader = vtk.vtkSTLReader()
    elif path.endswith('.ply'):
        reader = vtk.vtkPLYReader()
    elif path.endswith('.obj'):
        reader = vtk.vtkOBJReader()
    else:
        return 0

    reader.SetFileName(path)
    reader.Update()
    data = reader.GetOutput()
    if data:
        polydata = Poly_Data()
        polydata.Init_From_Vtk(data)
        return polydata
    return 0

def Write_Poly_Data(poly_data:Poly_Data, path):
    '''
    Input:
        poly_data: Poly_Data, data to write
        path: string, full path of data to write. Support stl, ply, obj, vtk.
    Output:
    Description: Write poly data in various formats.
    '''
    vtk_poly_data = poly_data.Get_vtkPolyData()
    if path.endswith('.vtk'):
        writer = vtk.vtkPolyDataWriter()
    elif path.endswith('.stl'):
        writer = vtk.vtkSTLWriter()
    elif path.endswith('.ply'):
        writer = vtk.vtkPLYWriter()
    elif path.endswith('.obj'):
        writer = vtk.vtkOBJWriter()
    else:
        return 0
    writer.SetInputData(vtk_poly_data)
    writer.SetFileName(path)
    writer.Write()