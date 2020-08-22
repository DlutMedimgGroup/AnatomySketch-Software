'''
Script name: Poly_Data
Author:@Mingrui
Created:2020/05/14
Last modified: 2020/08/05
Version:1
Description: Poly_Data Class, whitch is used to represent poly data.
'''
import vtk

class Poly_Data:
    '''
    Description: This class is used to represent poly data.
    '''
    def __init__(self):
        '''
        Input:
        Output:
        Description: Constructor. Need to be initialized afterwards.
        '''
        self._poly_data = vtk.vtkPolyData()

    def Get_vtkPolyData(self):
        '''
        Input:
        Output: vtkPolyData
        Description: Convert to vtkPolyData and return.
        '''
        return self._poly_data

    def Init_From_Vtk(self, data):
        '''
        Input: vtkPolyData
        Output:
        Description: Init with an vtkPolyData.
        '''
        self._poly_data = data