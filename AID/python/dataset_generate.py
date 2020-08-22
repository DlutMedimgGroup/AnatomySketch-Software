import os
import pylib.io.IO_Image_Data as IO_Image_Data
import pylib.improcessing.Convert_Polydata_To_Imagedata as Convert_Polydata_To_Imagedata
import numpy as np
from pylib.utilities.Image_Data import Image_Data
import json
import fill_hole
import cv2
from PIL import Image


# paths
root_path = os.path.abspath(os.path.join(os.path.dirname("__file__"), os.path.pardir))
image_path = os.path.join(root_path, 'image')
mesh_path = os.path.join(root_path, 'mesh')
label_path = os.path.join(root_path, 'label')
if not os.path.exists(label_path):
    os.makedirs(label_path)
slice_path = os.path.join(root_path, 'slice_dataset')
if not os.path.exists(slice_path):
    os.makedirs(slice_path)



def get_image_info(data_id):
    full_path = os.path.join(image_path, data_id + '_image.nii.gz')
    image = IO_Image_Data.Read_Image_Data(full_path)
    dim = image.Get_Dim()
    spacing = image.Get_Spacing()
    origin = image.Get_Origin()
    return dim, origin, spacing


def get_label_from_name(name):
    if 'liver' in name:
        return 1
    if 'leftkidney' in name:
        return 2
    if 'rightkidney' in name:
        return 3
    if 'spleen' in name:
        return 4


def convert_polys_to_label(data_id, poly_list):
    dim, origin, spacing = get_image_info(data_id)
    result_array = np.zeros(dim, dtype=float)
    for poly_name in poly_list:
        poly_full_name = os.path.join(mesh_path, poly_name)
        poly_data = IO_Image_Data.Read_Poly_Data(poly_full_name)
        label_image = Convert_Polydata_To_Imagedata.Convert(poly_data, dim, origin, spacing)
        label_array = label_image.Get_Data()
        label = get_label_from_name(poly_name)
        result_array[label_array != 0] = label
    result_image = Image_Data()
    result_image.Init_From_Numpy_array(result_array, spacing, origin)
    output_full_path = os.path.join(label_path, data_id + '_label.nii.gz')
    IO_Image_Data.Write_Image_Data(result_image, output_full_path)
    print('convert mesh to label: ' + data_id + ' finished.')


def convert_mesh_to_label():
    file_list = os.listdir(mesh_path)
    file_dict = {}
    for poly_name in file_list:
        data_id = poly_name[0:3]
        if data_id in file_dict:
            file_dict[data_id].append(poly_name)
        else:
            file_dict[data_id] = [poly_name]
    for data_id in file_dict:
        convert_polys_to_label(data_id, file_dict[data_id])


def get_slice(array, direction, index):
    if direction == 0:
        return array[index, :, :]
    if direction == 1:
        return array[:, index, :]
    if direction == 2:
        return array[:, :, index]


def get_slice_name(image_id, img_out_path, mask_out_path, direction, slice_index):
    slice_no = image_id + str(direction+1).zfill(2) + str(slice_index).zfill(3)
    slice_name = slice_no+'.jpg'
    slice_full_name = os.path.join(img_out_path, slice_name)
    mask_full_name = os.path.join(mask_out_path, slice_name)
    return slice_no, slice_name, slice_full_name, mask_full_name


def get_resize_shape(origin_shape, spacing, direction):
    [a, b] = origin_shape
    if direction == 0:
        pixel_shape = [spacing[1], spacing[0]]
    if direction == 1:
        pixel_shape = [spacing[2], spacing[0]]
    if direction == 2:
        pixel_shape = [spacing[2], spacing[1]]
    if a > b:
        b = b/pixel_shape[0]*pixel_shape[1]
        b = int(b * 512 / a)
        a = 512
    elif a < b:
        a = a/pixel_shape[1]*pixel_shape[0]
        a = int(a * 512 / b)
        b = 512
    return (b, a)


def save_slice(image_array, direction, slice_index, slice_full_name, resize_shape):
    if os.path.exists(slice_full_name):
        return 0, False
    img_slice = get_slice(image_array, direction, slice_index)

    img_slice = cv2.resize(img_slice, resize_shape, interpolation=cv2.INTER_CUBIC)
    img_image = Image.fromarray(img_slice)
    img_image = img_image.convert('RGB')
    img_image.save(slice_full_name)
    return img_slice, True


def generate_direction(direction, image, mask, annotations, images, img_out_path, mask_out_path, image_id, spacing):
    num_of_slices = image.shape[direction]
    for slice_index in range(num_of_slices):
        mask_slice = get_slice(mask, direction, slice_index)
        for target in [1, 2, 3, 4]:
            if target == 1:
                # liver
                mask_liver = np.zeros(image.shape)
                mask_liver[mask == 1] = 1
                liver_slices = np.nonzero(mask_liver)[0]
                liver_range = [liver_slices.min(), liver_slices.max()]
                range_v = liver_range[1] - liver_range[0]
                liver_threshold_1 = liver_range[0] + range_v / 3
                liver_threshold_2 = liver_range[0] + range_v / 3 * 2
            mask_target_slice = np.zeros(mask_slice.shape)
            mask_target_slice[mask_slice == target] = 255
            area = np.count_nonzero(mask_target_slice)
            if area < 5:
                continue
            mask_target_slice = mask_target_slice.astype('uint8')
            mask_target_slice = fill_hole.FillHole(mask_target_slice)
            resize_shape = get_resize_shape(mask_target_slice.shape, spacing, direction)
            mask_target_slice = cv2.resize(mask_target_slice, resize_shape, interpolation=cv2.INTER_NEAREST)

            slice_no, slice_name, slice_full_name, mask_full_name = get_slice_name(image_id, img_out_path,
                                                                                   mask_out_path, direction,
                                                                                   slice_index)
            img_slice, flag = save_slice(image, direction, slice_index, slice_full_name, resize_shape)
            print('slice_No:' + slice_no)

            current_img_dict = {'file_name': slice_name, 'id': int(slice_no)}
            images[slice_no] = current_img_dict
            binary, contours, hierarchy = cv2.findContours(mask_target_slice.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

            if flag:
                mask_slice_ = mask_slice.copy()
                mask_slice_ = mask_slice_ * 60
                mask_slice_ = cv2.resize(mask_slice_, resize_shape, interpolation=cv2.INTER_NEAREST)
                mask_image = Image.fromarray(mask_slice_)
                mask_image = mask_image.convert('RGB')
                mask_image.save(mask_full_name)
                # show_contour_with_image(img_slice, contours[0])

            contour_list = []
            num_of_contours = len(contours)
            for i in range(num_of_contours):
                contour = contours[i]
                contour = contour.ravel().tolist()
                contour_list.append(contour.copy())
            current_ann_dict = {'segmentation': contour_list, 'type': direction*4+target}
            if target == 1:
                # liver
                if slice_index > liver_threshold_1 and slice_index <= liver_threshold_2:
                    current_ann_dict['type'] = 5
                elif slice_index > liver_threshold_2:
                    current_ann_dict['type'] = 6
            if not slice_no in annotations.keys():
                annotations[slice_no] = []
            annotations[slice_no].append(current_ann_dict.copy())


def generate_instance(image, mask, annotations, images, img_out_path, mask_out_path, image_id):
    data_type = int(image_id[0])
    image_array = image.Get_Data_With_F_Order()
    if data_type == 1:
        window_level = [400, 20]  # CT
    elif data_type == 2:
        max_pixel = image_array.max()
        window_level = [max_pixel / 3 * 2, max_pixel / 3]  # MR T1
    elif data_type == 3:
        max_pixel = image_array.max()
        window_level = [max_pixel / 4 * 2, max_pixel / 4]  # MR T2
    image_array = image.Get_Data_With_F_Order(window_level=window_level)
    mask_array = mask.Get_Data_With_F_Order()
    for direction in range(1):
        generate_direction(direction, image_array, mask_array, annotations, images,
                           img_out_path, mask_out_path, image_id, image.Get_Spacing())


def convert_label_to_slice_dataset():
    img_out_path = os.path.join(slice_path, 'img')
    mask_out_path = os.path.join(slice_path, 'mask')
    json_path = os.path.join(slice_path, 'annotations')
    if not os.path.isdir(img_out_path):
        os.makedirs(img_out_path)
    if not os.path.isdir(mask_out_path):
        os.makedirs(mask_out_path)
    if not os.path.isdir(json_path):
        os.makedirs(json_path)
    json_path = os.path.join(json_path, 'MyCT_train_instance.json')

    annotations = {}
    images = {}
    image_list = os.listdir(image_path)
    for image_name in image_list:
        image_id = image_name[0:3]
        image_full_name = os.path.join(image_path, image_name)
        mask_name = image_name[0:3]+'_label.nii.gz'
        mask_full_name = os.path.join(label_path, mask_name)
        image = IO_Image_Data.Read_Image_Data(image_full_name)
        mask = IO_Image_Data.Read_Image_Data(mask_full_name)
        generate_instance(image, mask, annotations, images, img_out_path, mask_out_path, image_id)
    # write json
    json_dict = {'images': images, 'annotations': annotations}
    with open(json_path, "w") as f:
        json.dump(json_dict, f)



if __name__ == '__main__':
    convert_mesh_to_label()
    convert_label_to_slice_dataset()
