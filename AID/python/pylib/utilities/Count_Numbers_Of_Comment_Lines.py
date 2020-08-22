'''
Script name: 注释统计
Author:@HanYe
Created:2019/05/24
Last modified: 2019/05/24
Version:2
Description:本程序可以统计当前文件夹内所有 Python 程序注释行数，并在控制台输出。
'''
import os


def Get_Python_Code_In_Folder(folder):
    '''
    Input: The path of code folder.
    Output: The path list which contain python code.
    Description: The purpose of this function is to get all the .py files in the folder.
    '''
    modify_path_list = []
    path_list = os.listdir(folder)
    for file_name in path_list:
        if '.py' in file_name:  # 去除非Python程序文件
            modify_path_list.append(os.path.join(folder, file_name))

    return modify_path_list


def Count_Numbers_Of_Comment(path_list):
    flag = 0
    for path in path_list:
        file = open(path, "r", encoding="utf8")
        for line in file.readlines():  # 逐行读文件
            if '#' in line:
                flag += 1  # 若该行存在'#'号，flag 标记自自增1
        file.close()
    print("注释行数：", c)
    return c


folder = r'C:\Users\hanye\Desktop\dd'  # 需要统计的文件目录，使用时修改此行即可
Path_List = getPythonCodeInFolder(folder)
Count_Lines = Count_Numbers_Of_Comment(Path_List)
