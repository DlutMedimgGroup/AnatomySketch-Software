'''
Script name: Cut_Polydata_To_Contour
Author:@Mingrui
Created:2020/05/14
Last modified: 2020/05/14
Version:1
Description: 通过平面将曲面截为轮廓线
'''
import vtk
from pylib.utilities.Poly_Data import Poly_Data
from vtk.numpy_interface import dataset_adapter as dsa
import numpy as np
from pylib.utilities.Contour_Data import Contour

def Cut_Polydata_To_Contour(poly, pos, direction):
    '''
    Input:
        poly: Poly_Data, poly to cut
        pos: 截面经过的点
        direction: str, 截面的方向 (sagittal, coronal, axial)
    Output: Image_Data类型的图像
    Description: 读入mnd, nii等多种格式的医学影像
    '''
    if direction == 'sagittal':
        normal = [1, 0, 0]
    elif direction == 'coronal':
        normal = [0, 1, 0]
    elif direction == 'axial':
        normal = [0, 0, 1]
    else:
        return -1

    plane = vtk.vtkPlane()
    plane.SetOrigin(pos)
    plane.SetNormal(normal)
    cutter = vtk.vtkCutter()
    cutter.SetCutFunction(plane)
    cutter.SetInputData(poly.poly_data)
    cutter.Update()

    contour_data = Contour()
    contour_data.Init_From_Vtk(cutter.GetOutput())

    return contour_data
