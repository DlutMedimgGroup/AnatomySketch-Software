import matplotlib.pyplot as plt

def draw_histogram(array, bins, xrange):
    """

    :param array: 图像数组
    :param xrange: 横坐标范围
    :param bins: 箱子数
    :return: null, 显示图片

    """
    array = array.flatten()
    plt.hist(array, bins=bins)
    plt.xlim(xrange[0], xrange[1])
    plt.xlabel('CT Value')
    plt.ylabel('Frequency')
    plt.title('Histogram')
    plt.show()
