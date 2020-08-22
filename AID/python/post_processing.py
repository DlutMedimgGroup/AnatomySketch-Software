import os
import json
import numpy as np
import SimpleITK as sitk
import pylib.io.IO_Image_Data as IO_Image_Data
from pylib.utilities.Image_Data import Image_Data
import pylib.improcessing.Convert_Contour_To_Label as Conver_Contour_To_Label
import pylib.utilities.Contour_Data as Contour_Data
import pylib.io.Contour_Data_Writer as Contour_Data_Writer
import pylib.improcessing.Convert_Label_To_Contour as Convert_Label_To_Contour


# paths
root_path = os.path.abspath(os.path.join(os.path.dirname("__file__"), os.path.pardir))
network_output_path = os.path.join(root_path, 'network_output')
image_path = os.path.join(root_path, 'image')
label_output_path = os.path.join(root_path, 'network_output_label')
if not os.path.exists(label_output_path):
    os.makedirs(label_output_path)
label_output_contour = os.path.join(root_path, 'network_output_contour')
if not os.path.exists(label_output_contour):
    os.makedirs(label_output_contour)

targets = ['liver', 'leftkidney', 'rightkidney', 'spleen']


def get_data_full_name(img_id):
    full_path = os.path.join(image_path, img_id + '_image.nii.gz')
    return full_path


def index_to_word(x, dim, spacing, origin, direction, database):
    if direction == 2:
        spa = [spacing[1], spacing[2]]
        ori = [origin[1], origin[2]]
    elif direction == 1:
        spa = [spacing[0], spacing[2]]
        ori = [origin[0], origin[2]]
    elif direction == 0:
        spa = [spacing[0], spacing[1]]
        ori = [origin[0], origin[1]]
    if database != '1':
        s_x = 512 / dim[0]
        s_y = 512 / dim[1]
        x = np.array(x)/[s_x, s_y]
    y = np.array(x) * np.array(spa) + np.array(ori)
    return list(y)


def convert_slices_to_3d_label(img_id, slices):
    database = img_id[0]
    img_full_path = get_data_full_name(img_id)
    image = IO_Image_Data.Read_Image_Data(img_full_path)
    origin = image.Get_Origin()
    spacing = image.Get_Spacing()
    dim = image.Get_Dim()
    x_threshold = dim[0] / 2
    out_label = np.zeros(dim)

    for slice_dict in slices:
        slice_id = slice_dict['slice_id']
        labels = slice_dict['label']
        direction = int(slice_id[4]) - 1
        slice = int(slice_id[5:8])
        polys_2d = slice_dict['polys']
        for ii, poly_2d in enumerate(polys_2d):
            if not len(polys_2d) > 0:
                continue
            label = labels[ii]
            if label == 5 or label == 6:
                label = 1
            poly_2d = index_to_word(poly_2d, dim, spacing, origin, direction, database)
            if direction == 0:
                label_2d = Conver_Contour_To_Label.Poly2Label2D(poly_2d, dim[0:2], origin[0:2], spacing[0:2])
                x_center = np.nonzero(label_2d)[0]
                x_center = np.sum(x_center) / x_center.size
                if label == 1 and x_center > x_threshold:
                    label = 4
                elif label == 2 and x_center < x_threshold:
                    label = 3
                elif label == 3 and x_center > x_threshold:
                    label = 2
                elif label == 4 and x_center < x_threshold:
                    label = 1
                out_label[:, :, slice][label_2d != 0] = label
    # output
    out_label = out_label.astype(np.int)
    image = Image_Data()
    image.Init_From_Numpy_array(out_label, spacing, origin)
    img_out_path = os.path.join(label_output_path, img_id + '_network_output.nii.gz')
    IO_Image_Data.Write_Image_Data(image, img_out_path)
    print('convert to 3D label finished: ' + img_id + '_network_output.nii.gz')


def convert_to_3d_label():
    file_list = os.listdir(network_output_path)
    json_list = []
    img_list = []
    for json_file in file_list:
        if os.path.splitext(json_file)[-1] != '.json':
            continue
        json_path = os.path.join(network_output_path, json_file)
        with open(json_path, 'r') as load_f:
            load_dict = json.load(load_f)
            json_list.append(load_dict)
            if load_dict['img_id'] not in img_list:
                img_list.append(load_dict['img_id'])

    for img_id in img_list:
        slices = [ele for ele in json_list if ele['img_id'] == img_id]
        convert_slices_to_3d_label(img_id, slices)


def dim_2D_to_3D(pos_2d, slice, direction):
    if direction == 0:
        pos_3d = [slice, pos_2d[0], pos_2d[1]]
    elif direction == 1:
        pos_3d = [pos_2d[0], slice, pos_2d[1]]
    elif direction == 2:
        pos_3d = [pos_2d[0], pos_2d[1], slice]
    return pos_3d


def convert_label_to_contour(label_img: Image_Data, target, contour_list):
    array = label_img.Get_Data()
    shape = label_img.Get_Dim()
    origin = label_img.Get_Origin()
    spacing = label_img.Get_Spacing()
    for direction in range(3):
        for slice_index in range(shape[direction]):
            if direction == 0:
                slice_array = array[slice_index, :, :]
                origin_2d = [origin[1], origin[2]]
                spacing_2d = [spacing[1], spacing[2]]
            elif direction == 1:
                slice_array = array[:, slice_index, :]
                origin_2d = [origin[0], origin[2]]
                spacing_2d = [spacing[0], spacing[2]]
            elif direction == 2:
                slice_array = array[:, :, slice_index]
                origin_2d = [origin[0], origin[1]]
                spacing_2d = [spacing[0], spacing[1]]
            area = np.count_nonzero(slice_array)
            if area < 50:
                continue
            slice_array = slice_array.astype(np.uint8)
            if slice_array.min == slice_array.max:
                continue
            contour = Convert_Label_To_Contour.Label2Contour2D(slice_array, slice_array.shape, origin_2d, spacing_2d)
            contour_lines = contour.Get_Contour_Lines()

            for contour_line in contour_lines:
                num_points = contour_line.Get_Number_Of_CtrlPoints()
                points = contour_line.Get_CtrlPoints()
                if num_points < 15:
                    continue
                poly_2d = []
                for i in range(0, num_points, 10):
                    # poly_2d.append(list(contour[i, 0, ::-1]))
                    poly_2d.append(list(points[i, :]))
                slice_pos = (slice_index - 0.5) * spacing[direction] + origin[direction]
                # slice_pos = slice_index * spacing[direction] + origin[direction]
                poly_3d = [dim_2D_to_3D(x, slice_pos, direction) for x in poly_2d]
                poly_3d = np.array(poly_3d)
                # poly_3d = label_img.Get_Position_From_Index(poly_3d)
                # poly_3d = poly_3d[1:-1:5, :]
                contour_line = Contour_Data.Contour_Line(poly_3d, is_closer=True)
                contour_line.Set_Property('view_label', direction)
                contour_list[targets.index(target)].Append_Line(contour_line)


def convert_a_file(label_name):
    label_full_name = os.path.join(label_output_path, label_name)
    label_img = sitk.ReadImage(label_full_name)
    label_array = sitk.GetArrayFromImage(label_img)
    img_id = label_name[0:3]
    BMC_Radius = 8
    BMO_Radius = 8

    contour_liver = Contour_Data.Contour()
    contour_liver.Set_Property('Name', 'liver')
    contour_liver.Set_Property('Color', [31, 119, 180])
    contour_liver.Set_Property('Label', 1)
    contour_leftkidney = Contour_Data.Contour()
    contour_leftkidney.Set_Property('Name', 'leftkidney')
    contour_leftkidney.Set_Property('Color', [255, 127, 14])
    contour_leftkidney.Set_Property('Label', 2)
    contour_rightkidney = Contour_Data.Contour()
    contour_rightkidney.Set_Property('Name', 'rightkidney')
    contour_rightkidney.Set_Property('Color', [46, 160, 44])
    contour_rightkidney.Set_Property('Label', 3)
    contour_spleen = Contour_Data.Contour()
    contour_spleen.Set_Property('Name', 'spleen')
    contour_spleen.Set_Property('Color', [214, 40, 39])
    contour_spleen.Set_Property('Label', 4)
    contour_list = [contour_liver, contour_leftkidney, contour_rightkidney, contour_spleen]

    for ii, target in enumerate(targets):
        target_array = np.zeros(label_array.shape)
        target_array = target_array.astype(np.int)
        target_array[label_array == ii+1] = 255
        target_img = sitk.GetImageFromArray(target_array)
        target_img.SetOrigin(label_img.GetOrigin())
        target_img.SetSpacing(label_img.GetSpacing())
        # close
        BMC = sitk.BinaryMorphologicalClosingImageFilter()
        BMC.SetKernelType(sitk.sitkBall)
        BMC.SetKernelRadius(BMC_Radius)
        BMC.SetForegroundValue(255)
        close_out = BMC.Execute(target_img)
        # open
        BMO = sitk.BinaryMorphologicalOpeningImageFilter()
        BMO.SetKernelType(sitk.sitkBall)
        BMO.SetKernelRadius(BMO_Radius)
        BMO.SetForegroundValue(255)
        open_out = BMO.Execute(close_out)
        #
        conn = sitk.ConnectedComponentImageFilter()
        conn_out = conn.Execute(open_out)
        # image
        out_image = Image_Data()
        out_image.Init_From_Itk(conn_out)
        array = out_image.Get_Data()
        a = np.bincount(array.flatten())
        a[0] = 0
        a_list = list(a)
        t = a_list.index(max(a))
        array[array != t] = 0
        array[array == t] = 255
        out_image.Init_From_Numpy_array(array, out_image.Get_Spacing(), out_image.Get_Origin())

        convert_label_to_contour(out_image, target, contour_list)
    if len(contour_list) > 0:
        ctr_name = os.path.join(label_output_contour, str(img_id) + '.ctr')
        Contour_Data_Writer.Write_Contour(contour_list, ctr_name)
        print('convert to 3D contour finished: ' + str(img_id) + '.ctr')


def convert_to_3d_contour():
    label_file_list = os.listdir(label_output_path)
    for label_name in label_file_list:
        convert_a_file(label_name)


if __name__ == '__main__':
    convert_to_3d_label()
    convert_to_3d_contour()
