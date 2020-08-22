'''
Script name: Image_Data
Author:@Mingrui
Created:2020/05/14
Last modified: 2020/08/05
Version:1
Description: Image_Data Class, whitch is used to represent 3D image data.
'''
import numpy as np
import SimpleITK as sitk
import vtk
import copy

class Image_Data:
    '''
    Description: This class is used to represent 3D image data
    '''
    def __init__(self):
        '''
        Input:
        Output:
        Description: Constructor. Need to be initialized afterwards.
        '''
        self._spacing = np.array([1., 1., 1.])
        self._origin = np.array([0., 0., 0.])
        self._dim = np.array([0., 0., 0.])
        self._data = np.array(0)

    def Get_Spacing(self):
        '''
        Input:
        Output: A list of spacing.
        Description: Get spacing of image data.
        '''
        return list(self._spacing)
    
    def Get_Origin(self):
        '''
        Input:
        Output: A list of origin.
        Description: Get origin of image data.
        '''
        return list(self._origin)
    
    def Get_Dim(self):
        '''
        Input:
        Output: A list of dimension.
        Description: Get dimension of image data.
        '''
        return list(self._dim)
    
    def Get_Data(self, window_level = [0, 0]):
        '''
        Input: window_level (Optional)
        Output: numpy array (C type, row-major)
        Description: Get numpy array of image data (C type, row-major).
        '''
        data = copy.deepcopy(self._data)
        if window_level != [0, 0]:
            data = (data - window_level[1]) * 256 / window_level[0] + 128
            data[data < 0] = 0
            data[data > 255] = 255  
        return data
    
    def Init_From_Numpy_array(self, array, spacing = [1, 1, 1], origin = [0, 0, 0]):
        '''
        Input:
            array: numpy array, image data array
            spacing: list, the spacing of image data
            dim: list, the dimention of image data
            origin: list, the origin of image data
        Output:
        Description: Init with a numpy array.
        '''
        self._spacing = spacing.copy()
        self._origin = origin.copy()
        self._dim = array.shape
        self._data = array.copy()
        
    def Get_Data_With_F_Order(self, window_level = [0, 0]):
        '''
        Input: window_level (Optional)
        Output: numpy array (F type, column-major)
        Description: Get numpy array of image data (F type, column-major).
        '''
        data = copy.deepcopy(self._data)
        if window_level != [0, 0]:
            data = data - window_level[1]
            a = 256 / window_level[0]
            data = data * a
            data = data + 128
            data[data < 0] = 0
            data[data > 255] = 255            
        data = data.ravel('C')
        data = np.reshape(data, self._dim[::-1], order='F')
        return data

    def Init_From_Itk(self, img):
        '''
        Input:
            img: itkImageData
        Output:
        Description: Init with an itkImageData.
        '''
        self._spacing = np.array(img.GetSpacing())
        self._origin = np.array(img.GetOrigin())
        self._dim = np.array(img.GetSize())
        self._data = sitk.GetArrayFromImage(img)
        self._data = self._data.ravel('F')
        self._data = np.reshape(self._data, self._dim, order='C')

    def Get_itkImageData(self):
        '''
        Input:
        Output:
            image: itkImageData
        Description: Convert to itkImageData and return.
        '''
        data = self._data.ravel('C')
        data = np.reshape(data, self._dim[::-1], order='F')
        
        image = sitk.GetImageFromArray(data)
        image.SetOrigin(self._origin)
        image.SetSpacing(self._spacing)
        return image

    def Init_From_Vtk(self, vtk_data):
        '''
        Input:
            vtk_data: vtkImageData
        Output:
        Description: Init with an vtkImageData.
        '''
        from vtk.util.numpy_support import vtk_to_numpy

        self._spacing = np.array(vtk_data.GetSpacing())
        self._dim = np.array(vtk_data.GetDimensions())
        self._origin = np.array(vtk_data.GetOrigin())
        self._data = vtk_to_numpy(vtk_data.GetPointData().GetScalars())
        self._data = self._data.reshape(self._dim[2], self._dim[1], self._dim[0])
        self._data = self._data.transpose(2, 1, 0)
        return

    def Get_vtkImageData(self):
        '''
        Input:
        Output:
            image_data: vtkImageData
        Description: Convert to vtkImageData and return.
        '''
        from vtkmodules.util.numpy_support import numpy_to_vtk

        vtk_data = numpy_to_vtk(self._data.ravel('F'), array_type=vtk.VTK_FLOAT)
        image_data = vtk.vtkImageData()
        image_data.SetDimensions(self._dim)
        image_data.SetSpacing(self._spacing)
        image_data.SetOrigin(self._origin)
        image_data.GetPointData().SetScalars(vtk_data)
        return image_data

    def Get_Position_From_Index(self, index):
        '''
        Input:
            index: list or numpy array (3 or 3*n, n is number of points), Pixel coordinates
        Output:
            pos: numpy array (3 or 3*n), Physical coordinates
        Description: Convert pixel coordinates to physical coordinates according to the parameters of this image data.
        '''
        index = np.array(index)
        pos = (index + 0.5) * self._spacing + self._origin
        return pos

    def Get_Index_From_Position(self, pos):
        '''
        Input:
            index: list or numpy array (3 or 3*n, n is number of points), Physical coordinates
        Output:
            pos: numpy array (3 or 3*n), Pixel coordinates
        Description: Convert physical coordinates to pixel coordinates according to the parameters of this image data.
        '''
        pos = np.array(pos)
        index = np.floor((pos - self._origin) / self._spacing)
        index = index.astype(np.int)
        return index
