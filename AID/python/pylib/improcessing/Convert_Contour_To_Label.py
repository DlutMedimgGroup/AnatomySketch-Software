'''
Script name: Convert_Contour_To_Label
Author:@Mingrui
Created:2020/05/14
Last modified: 2020/05/14
Version:1
Description: Convert a 2D contour to 2D label
'''
import vtkmodules.all as vtk
import numpy
from vtkmodules.util import numpy_support

def Poly2Label2D(poly, dim, origin, spacing):
    '''
    Input:
        poly: numpy array (n*2), poly to convert
        dim: dimention of target label
        origin: origin of target label
        spacing: spacing of target label
    Output: numpy array, result 2D label
    Description: Convert a 2D contour to 2D label
    '''
    # 1 构建poly
    NumOfPoints = len(poly)
    Points = vtk.vtkPoints()
    polydata = vtk.vtkPolyData()
    Points.Reset()
    i = 0
    while i < NumOfPoints:
        Points.InsertPoint(i, (poly[i][0], poly[i][1], 0))
        i += 1
    polydata.Modified()
    lines = vtk.vtkCellArray()
    lines.InsertNextCell(NumOfPoints+1)
    i = 0
    while i < NumOfPoints:
        lines.InsertCellPoint(i)
        i += 1
    lines.InsertCellPoint(0)
    polydata.SetPoints(Points)
    polydata.SetLines(lines)
    polydata.Modified()

    # 2 建立空白图像
    Image = vtk.vtkImageData()
    Image.SetDimensions(dim[0], dim[1], 1)
    Image.SetOrigin(origin[0], origin[1], 0)
    Image.SetSpacing(spacing[0], spacing[1], 0)
    Image.AllocateScalars(vtk.VTK_INT, 1)
    count = Image.GetNumberOfPoints()
    # Fill every entry of the image data with "0"
    for i in range(count):
        Image.GetPointData().GetScalars().SetTuple1(i, 0)

    # 3 LinearExtrusionFilter
    extrudeFilter = vtk.vtkLinearExtrusionFilter()
    extrudeFilter.SetInputData(polydata)
    extrudeFilter.SetScaleFactor(1)
    extrudeFilter.SetExtrusionTypeToNormalExtrusion()
    extrudeFilter.SetVector( 0, 0, 1)

    # 4 poly沿z轴平移-0.5
    m1 = vtk.vtkMatrix4x4()
    m1.SetElement(2, 3, -0.5)
    linearTransform = vtk.vtkMatrixToLinearTransform()
    linearTransform.GetMatrix().DeepCopy(m1)
    transformPolyData = vtk.vtkTransformPolyDataFilter()
    transformPolyData.SetInputConnection(extrudeFilter.GetOutputPort())
    transformPolyData.SetTransform(linearTransform)

    # 5 转换
    dataToStencil = vtk.vtkPolyDataToImageStencil()
    dataToStencil.SetInputConnection(transformPolyData.GetOutputPort())
    dataToStencil.SetOutputSpacing(spacing[0], spacing[1], 0)
    dataToStencil.SetOutputOrigin(origin[0], origin[1], 0)
    stencil = vtk.vtkImageStencil()
    stencil.SetStencilConnection(dataToStencil.GetOutputPort())
    stencil.SetInputData(Image)
    stencil.ReverseStencilOn()
    stencil.SetBackgroundValue(1)
    stencil.Update()

    # 6 vtk 2 numpy
    dataObj = stencil.GetOutput()
    vtkArray = dataObj.GetPointData().GetArray(0)
    numpyarray = numpy_support.vtk_to_numpy(vtkArray)
    Output = numpy.reshape(numpyarray, dim, order='F')

    return Output