'''
Script name: 阈值分割
Author:@Mingrui
Created:2020/05/16
Last modified: 2020/05/16
Version:1
Description:阈值分割
'''
from pylib.utilities.Image_Data import Image_Data
import numpy as np
import copy

def Threshold(img:Image_Data, threshold1, threshold2, foreground = 1, background = 0):
    '''
    Input: 需分割的图像，两个阈值，输出标签图中前景和背景的标签值
    Output: 分割结果图像
    Description: 阈值分割。灰度在[threshold1, threshold2]范围内的部分为前景，其余部分为背景
    '''
    output = copy.deepcopy(img)
    oridata = img.Get_Data()
    outdata = output.Get_Data()
    index = (oridata >= threshold1) & (oridata <= threshold2)
    outdata[index] = foreground
    outdata[~index] = background
    return output