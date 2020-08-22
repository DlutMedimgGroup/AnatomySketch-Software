'''
Author Mingrui
Version 1.0
update date 2020-05-30
mod date record 2020-05-30 version 1.0
'''

import os
import numpy as np
import SimpleITK as sitk
import shutil

def Open_In_AS(image):
    '''
    display an itkImageData with AnatomySketch.exe. Before using this function,
    you need to set the path of AnatomySkech.exe
    Args:
        image(itkImageData): the input
    Input: itkImageData to display
    Output: nothing
    Description: display an itkImageData with AnatomySketch.exe
    '''
    executive_path = "D:/AnatomySketch/AnatomySketch-1.0/AnatomySketch-1.0/AnatomySketch.exe"
    path = os.path.dirname(os.path.abspath(__file__)) + '\\_cache_data'
    if os.path.exists(path):
        shutil.rmtree(path)
    os.mkdir(path)
    file_name = path + '\\image.nii'
    sitk.WriteImage(image, file_name)
    result = os.system(executive_path + " " + file_name)

# an example of using the function
if __name__ == '__main__':

    impath = 'G:/20_AC_CT_0002/9238_LiXin_Series0_CT.mhd'
    image = sitk.ReadImage(impath)
    Open_In_AS(image)