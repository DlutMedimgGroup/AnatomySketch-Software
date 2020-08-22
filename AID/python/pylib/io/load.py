import SimpleITK as sitk
def ReadImage(path):
    '''
    输入：路径
    输出：图像对象，数组
    '''
    img = sitk.ReadImage(path)
    array = sitk.GetArrayFromImage(img)
    return img, array