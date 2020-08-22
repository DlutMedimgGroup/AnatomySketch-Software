from pylib.utilities.Image_Data import Image_Data
import vtk
import numpy as np

def Rasample(input_img:Image_Data, target_dim, method):
    ori_spacing = np.array(input_img.Get_Spacing())
    ori_dim = np.array(input_img.Get_Dim())
    target_spacing = ori_spacing / target_dim * ori_dim

    input_vtk_image = input_img.Get_vtkImageData()
    resample_filter = vtk.vtkImageResample()
    resample_filter.SetInputData(input_vtk_image)
    resample_filter.SetOutputSpacing(target_spacing)
    if method == 'Linear':
        resample_filter.SetInterpolationModeToLinear()
    elif method == 'Cubic':
        resample_filter.SetInterpolationModeToCubic()
    else:
        resample_filter.SetInterpolationModeToNearestNeighbor()
    resample_filter.Update()
    
    output_image_data = Image_Data()
    output_image_data.Init_From_Vtk(resample_filter.GetOutput())
    return output_image_data
    