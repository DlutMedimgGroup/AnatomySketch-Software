'''
Script name: 2D标签图转轮廓线
Author:@Mingrui
Created:2020/05/15
Last modified: 2020/05/15
Version:1
Description: convert 2D label to Contour
'''
import vtkmodules.all as vtk
import numpy as np
from vtkmodules.util.numpy_support import numpy_to_vtk
from pylib.utilities.Contour_Data import Contour

def Label2Contour2D(array, dim, origin, spacing):
    '''
    Input:
        array: numpy array, label to convert
        dim: dimention of input label
        origin: origin of input label
        spacing: spacing of input label
    Output: Contour
    Description: convert 2D label to Contour
    '''
    array = array.astype(np.int)
    vtk_data = numpy_to_vtk(array.ravel('F'), array_type=vtk.VTK_INT)
    image = vtk.vtkImageData()
    image.SetDimensions([dim[0], dim[1], 1])
    image.SetSpacing([spacing[0], spacing[1], 0])
    image.SetOrigin([origin[0], origin[1], 0])
    image.GetPointData().SetScalars(vtk_data)

    filer = vtk.vtkMarchingSquares()
    filer.SetInputData(image)
    filer.SetValue(0, 255)
    filer.Update()
    poly = filer.GetOutput()
    
    contour = Contour()
    b = contour.Init_From_Vtk(poly)
    return contour

