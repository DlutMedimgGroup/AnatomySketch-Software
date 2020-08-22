import SimpleITK as sitk


def WriteImage(array, path, origin = (0,0,0), spacing = [1,1,1]):
    '''
    输入：数组
    输出：文件
    '''
    image = sitk.GetImageFromArray(array)
    image.SetOrigin(origin)
    image.SetSpacing(spacing)
    sitk.WriteImage(image, path)