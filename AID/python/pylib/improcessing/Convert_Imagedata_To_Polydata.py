'''
Script name: 标签图转曲面
Author:@Mingrui
Created:2020/05/15
Last modified: 2020/05/15
Version:1
Description:使用Marching cube方法将标签图转换为曲面，并平滑
'''
import vtk
from pylib.utilities.Poly_Data import Poly_Data
from pylib.utilities.Image_Data import Image_Data
import pylib.improcessing.Threshold_Seg as Threshold_Seg

def Convert(image:Image_Data, threshold1, threshold2, smooth_iterations = 0):
    '''
    Input: 需转换的图像，两个阈值，平滑迭代次数
    Output: 转换得到的曲面数据
    Description: 使用Marching cube方法将标签图转换为曲面，并平滑。两个阈值间的灰度范围为前景，其余部分为背景
    '''
    label = Threshold_Seg.Threshold(image, threshold1, threshold2)

    Extractor = vtk.vtkMarchingCubes()
    Extractor.SetInputData(label.Get_vtkImageData())
    Extractor.SetValue(0, 1)
    Smoother = vtk.vtkSmoothPolyDataFilter()
    Smoother.SetInputConnection(Extractor.GetOutputPort())
    Smoother.SetNumberOfIterations(smooth_iterations)
    Smoother.SetRelaxationFactor(0.1)
    Smoother.FeatureEdgeSmoothingOff()
    Smoother.BoundarySmoothingOn()
    Smoother.Update()
    Normals = vtk.vtkPolyDataNormals()
    Normals.SetInputConnection(Smoother.GetOutputPort())
    Normals.SetFeatureAngle(60.0)
    Stripper = vtk.vtkStripper()
    Stripper.SetInputConnection(Normals.GetOutputPort())
    Stripper.Update()

    polydata = Poly_Data()
    polydata.Init_From_Vtk(Stripper.GetOutput())
    return polydata