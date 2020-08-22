'''
Script name: Convert_Polydata_To_Imagedata
Author:@Mingrui
Created:2020/05/14
Last modified: 2020/05/14
Version:1
Description: 将曲面转标签图
'''
import vtk
import numpy as np
from vtkmodules.util.numpy_support import numpy_to_vtk
from pylib.utilities.Poly_Data import Poly_Data
from pylib.utilities.Image_Data import Image_Data


def Convert(poly:Poly_Data, dim, origin, spacing):
    '''
    Input:
        poly: Poly_Data, poly to convert
        dim: dimention of target label
        origin: origin of target label
        spacing: spacing of target label
    Output: Image_Data, output label
    Description: Convert poly to 3D label.
    '''
    polydata = poly.Get_vtkPolyData()
    # ImageData
    array = np.zeros(dim, dtype=float)
    vtk_data = numpy_to_vtk(array.ravel('F'), array_type=vtk.VTK_FLOAT)
    vtkImage = vtk.vtkImageData()
    vtkImage.SetDimensions(dim)
    vtkImage.SetOrigin(origin)
    vtkImage.SetSpacing(spacing)
    vtkImage.GetPointData().SetScalars(vtk_data)
    # vtkFillHolesFilter
    FillHolesFilter = vtk.vtkFillHolesFilter()
    FillHolesFilter.SetInputData(polydata)
    FillHolesFilter.SetHoleSize((dim[0]*spacing[0] + dim[1]*spacing[1] + dim[2]*spacing[2])/6)
    FillHolesFilter.Update()
    # PolyDataNormals
    PolyDataNormals = vtk.vtkPolyDataNormals()
    PolyDataNormals.SetInputConnection(FillHolesFilter.GetOutputPort())
    PolyDataNormals.ConsistencyOn()
    PolyDataNormals.Update()
    # PolyDataToImageStencil
    DataToStencil = vtk.vtkPolyDataToImageStencil()
    DataToStencil.SetInputConnection(PolyDataNormals.GetOutputPort())
    DataToStencil.SetOutputSpacing(spacing)
    DataToStencil.SetOutputOrigin(origin)
    # ImageStencil
    Stencil = vtk.vtkImageStencil()
    Stencil.SetStencilConnection(DataToStencil.GetOutputPort())
    Stencil.SetInputData(vtkImage)
    Stencil.ReverseStencilOn()
    Stencil.SetBackgroundValue(1)
    Stencil.Update()
    # Output
    Result = Stencil.GetOutput()
    image = Image_Data()
    image.Init_From_Vtk(Result)
    return image