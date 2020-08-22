'''
Script name: Contour_Data_Writer
Author:@Mingrui
Created:2020/05/14
Last modified: 2020/05/14
Version:1
Description: write Contour to ctr file. ctr is a file format of AnatomySketch. One ctr file can handle multiple Contour.
'''
import pylib.utilities.Contour_Data as Contour_Data
import numpy as np

def Write_Contour(contour_list, file_name):
    '''
    Input:
        contour_list: list, list of Contour to write
        file_name: str, full path of output file
    Output:
    Description:
        write Contour to ctr file. ctr is a file format of AnatomySketch. One ctr file can handle multiple Contour.
    '''
    file = open(file_name, 'w+', newline='\n')
    # 0 写入文件头
    file.write('#Anatomy Sketch Contour File\n')
    # 1 曲面轮廓数量
    file.write('#NUMBER OF DATA:\n')
    file.write(str(len(contour_list)) + '\n')
    # 2 DATALIST
    file.write('#DATA LIST DEFINITION:\n')
    for ncrntContourData, contour in enumerate(contour_list):
        # 2.1 DATA LIST NUMBER
        file.write('#DATA LIST NUMBER ' + str(ncrntContourData) + ' DEFINITION:\n')
        # 2.2 data_list_index
        file.write(str(ncrntContourData) + ' # data_list_index\n')
        # 2.3 data_name
        file.write(contour.Get_Property('Name') + ' # data_name\n')
        # 2.4 Color
        color = contour.Get_Property('Color')
        file.write(str(color[0]) + ' ' + str(color[1]) + ' ' + str(color[2]) + ' # r_color g_color b_color\n')
        # 2.5 Label
        file.write(str(contour.Get_Property('Label')) + ' # data_label\n')
        # 2.6 number_of_contours
        contour_lines = contour.Get_Contour_Lines()
        number_of_contours = len(contour_lines)
        file.write(str(number_of_contours) + ' # number_of_contours\n')

        for ncrntCurve, contour_line in enumerate(contour_lines):
            file.write('#CONTOUR LIST NUMBER ' + str(ncrntCurve) + ' DEFINITION:\n')
            # 2.6.1 contour_index
            file.write(str(ncrntCurve) + ' # contour_index\n')
            # 2.6.2 contour_ViewLabel
            view_label = contour_line.Get_Property('view_label')
            file.write(str(view_label) + ' # contour_ViewLabel\n')
            # 2.6.3 contour_isClosure
            if contour_line.Get_Closer():
                closer = 'true'
            else:
                closer = 'false'
            file.write(closer + ' # contour_isClosure\n')
            # 2.6.4 contour_dLayer
            ctrl_points = contour_line.Get_CtrlPoints()
            layer = ctrl_points[0, view_label]
            file.write(str(layer) + ' # contour_dLayer\n')
            # 2.6.5 Points
            file.write('#START_POINTS_POS\n')
            number_of_points = ctrl_points.shape[0]
            for i in range(number_of_points):
                file.write(str(ctrl_points[i, 0]) + ' ' + str(ctrl_points[i, 1]) + ' ' + str(ctrl_points[i, 2]) + '\n')
            file.write('#END_POINTS_POS\n')
    # 3 写入文件尾
    file.write('#\n')
    file.write('#END OF CONTOURS DEFINITION\n')
