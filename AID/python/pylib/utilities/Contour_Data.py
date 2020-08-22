'''
Script name: Contour_Line
Author:@Mingrui
Created:2020/05/14
Last modified: 2020/08/05
Version:1
Description: Contour and Contour_Line are used together to represent the contour data.
'''
import vtk
import numpy as np
import copy

class Contour_Line:
    '''
    Description:
        This class is used to represent a contour line. Contour contains one or more Contour_Line. ContourLine is
        essentially a series of control points in order.
    '''
    def __init__(self, ctrl_points, is_closer = True):
        '''
        Input:
            ctrl_points: numpy array (3*n), position of control points
            is_closer (Optional): is contour line closer or not
        Output:
        Description: Constructor
        '''
        self._ctrl_points = copy.deepcopy(ctrl_points)
        self._is_closer = is_closer
        self._property = {}

    def Set_Property(self, key, value):
        '''
        Input:
            key: string
            value: any type
        Output:
        Description:
            Set properties. The type of properties is uniquely identified by "key", so the key is required when
            accessing the properties.
        '''
        self._property[key] = value

    def Get_Property(self, key):
        '''
        Input:
            key: string
        Output:
            value of property
        Description:
            Get properties. The type of properties is uniquely identified by "key", so the key is required when
            accessing the properties.
        '''
        return self._property[key]
    
    def Get_Closer(self):
        '''
        Input:
        Output: bool, closer or not
        Description: Get whether the contour line is closed.
        '''
        return self._is_closer
    
    def Get_CtrlPoints(self):
        '''
        Input:
        Output: numpy array (3*n), position of contorl points
        Description: Get position of contorl points
        '''
        return self._ctrl_points

    def Get_Number_Of_CtrlPoints(self):
        '''
        Input:
        Output: Number of contorl points
        Description: Get number of contorl points
        '''
        return self._ctrl_points.shape[0]

    def Get_Interpolated_CtrlPoints(self, num_of_points):
        '''
        Input:
            num_of_points: Number of contorl points after interpolation
        Output: numpy array (3*n), position of interpolated contorl points
        Description: Get a specific number of control points through interpolation
        '''
        Points = vtk.vtkPoints()
        for i in range(self._ctrl_points.shape[0]):
            Points.InsertPoint(i, self._ctrl_points[i][0], self._ctrl_points[i][1], self._ctrl_points[i][2])
        Points.Modified()

        ParametricSpline = vtk.vtkParametricSpline()
        ParametricSpline.SetPoints(Points)
        if self._is_closer:
            ParametricSpline.ClosedOn()
        else:
            ParametricSpline.ClosedOff()
        ParametricSpline.Modified()

        ParametricFunctionSource = vtk.vtkParametricFunctionSource()
        ParametricFunctionSource.SetParametricFunction(ParametricSpline)
        ParametricFunctionSource.SetUResolution(num_of_points)
        ParametricFunctionSource.Update()
        points = np.array(ParametricFunctionSource.GetOutput().GetPoints().GetData())
        return points

class Contour:
    '''
    Description:
        This class is used to represent a contour data. Contour contains one or more Contour_Line. Contour is
        essentially the sum of ContourLine and contains related attributes.
    '''
    def __init__(self):
        '''
        Input:
        Output:
        Description: Constructor. Need to be initialized afterwards.
        '''
        self._contour_lines = []
        self._num_of_lines = 0
        self._property = {}
        

    def Get_Contour_Lines(self):
        '''
        Input:
        Output: a list of Contour_Line
        Description: Get all Contour_Line of this contour data
        '''
        return self._contour_lines

    def Append_Line(self, line):
        '''
        Input: Contour_Line, the contour line to append.
        Output:
        Description: Append a new Contour_Line to this contour data.
        '''
        self._contour_lines.append(line)
        self._num_of_lines = len(self._contour_lines)
        
    def Set_Property(self, key, value):
        '''
        Input:
            key: string
            value: any type
        Output:
        Description:
            Set properties. The type of properties is uniquely identified by "key", so the key is required when
            accessing the properties.
        '''
        self._property[key] = value
    
    def Get_Property(self, key):
        '''
        Input:
            key: string
        Output:
            value of property
        Description:
            Get properties. The type of properties is uniquely identified by "key", so the key is required when
            accessing the properties.
        '''
        return self._property[key]


    def Init_From_Vtk(self, poly):
        '''
        Input: vtkPolyData
        Output:
        Description: Init this contour data with a vtkPolyData.
        '''
        num_of_cells = poly.GetNumberOfCells()
        if num_of_cells == 0:
            return False
        points = np.array(poly.GetPoints().GetData())
        lines = np.array(poly.GetLines().GetData())
        lines_array = np.empty([num_of_cells, 2], dtype=int)
        lines_array[:, 0] = lines[1::3]
        lines_array[:, 1] = lines[2::3]
        points_sequence = np.empty([num_of_cells, 3])

        # The vertices in vtkPointData are unordered. We need to find the order of vertices in lines, whitch
        # represents the order of points.
        p = [0, 0]
        end = lines_array[p[0], p[1]]
        iter = 0
        for i in range(num_of_cells):
            points_sequence[iter, :] = points[lines_array[p[0], p[1]], :]
            new_id = lines_array[p[0], 1 - p[1]]

            lines_array[p[0], :] = [-1, -1]
            p = np.where(lines_array == new_id)
            if len(p[0]) == 0:
                new_id = end
            if len(p[0]) > 1:
                p = (p[0][0], p[1][0])

            if new_id == end:
                points_sequence = points_sequence[0:iter, :]
                contour_line = Contour_Line(points_sequence, True)
                self.Append_Line(contour_line)
                rest = np.where(lines_array != -1)
                if len(rest[0]) > 0:
                    p = [rest[0][0], rest[1][0]]
                    end = lines_array[p[0], p[1]]
                    iter = 0
                    points_sequence = np.empty([num_of_cells, 3])
            else:
                iter += 1
        return True
