import numpy as np
import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import pywt
import sys
import os
from PIL import Image





def rgb2gray(rgb):
    return np.dot(rgb[..., :3], [0.2989, 0.5870, 0.1140])


def save_image(image, filename='foo.png'):
    fig = plt.figure(frameon=False)
    fig.set_size_inches(image.shape[1] / 100, image.shape[0] / 100)
    ax = plt.Axes(fig, [0., 0., 1., 1.])
    ax.set_axis_off()
    fig.add_axes(ax)
    ax.imshow(image, cmap='gray', aspect='auto')

    plt.savefig(filename)


def compress(image_path, threshold_wavelet=0.05, threshold_run_len=0.001):
    # threshold_wavelet is percentage of threshing image. For %95 it is 0.05
    # threshold_run_len is threshold for Enhanced Run length encoding
    # is_gray represent whether image is grayscale or not.
    image = mpimg.imread(image_path)
    if len(image.shape) == 3:
        image = rgb2gray(image)
    picture = Image.open(image_path)
    picture = picture.convert('L')
    picture.save("Compressed_" + str(10)+".jpeg", "JPEG", optimize=True, quality=10)
    picture.save("Compressed_" + str(30)+".jpeg", "JPEG", optimize=True, quality=30)
    picture.save("Compressed_" + str(50)+".jpeg", "JPEG", optimize=True, quality=50)

    #  Multi-level of wavelet
    n = 4
    # Type of wavelet (Haar wavelet)
    w = 'haar'
    coefficients = pywt.wavedec2(image, wavelet=w, level=n)

    coefficients_arr, coefficients_slices = pywt.coeffs_to_array(coefficients)

    coefficients_sort = np.sort(np.abs(coefficients_arr.reshape(-1)))
    # Finding threshold from percentage for if threshold_wavelet = 0.05 , we thresh %95 of image
    thresh = coefficients_sort[int(np.floor((1 - threshold_wavelet) * len(coefficients_sort)))]
    indices = np.abs(coefficients_arr) > thresh
    # Threshold small indices
    coefficients_filter = coefficients_arr * indices
    run_len = []
    m, n = coefficients_filter.shape
    k = coefficients_filter[0][0]
    c = 0
    # Enhanced Run length encoding
    for i in range(m):
        for j in range(n):
            if abs(k - coefficients_filter[i, j]) < threshold_run_len:
                c += 1
            else:
                run_len.append((k, c))
                k = coefficients_filter[i, j]
                c = 1
    run_len.append((k, c))

    return run_len, coefficients_slices, m, n


def decompress(image_path, compress_image):
    # Send image path to compare image.
    # Type of wavelet (Haar wavelet)
    w = 'haar'
    m, n = (compress_image[2], compress_image[3])
    encode, slices = compress_image[0], compress_image[1]

    image = mpimg.imread(image_path)
    if len(image.shape) == 3:
        image = rgb2gray(image)
        img_size = sys.getsizeof(image[0][0]) * m * n * 3
    else:
        img_size = sys.getsizeof(image[0][0]) * m * n

    image_s = os.stat(image_path).st_size
    c10_s = os.path.getsize('Compressed_10.jpeg')
    c30_s = os.stat('Compressed_30.jpeg').st_size
    c50_s = os.stat('Compressed_50.jpeg').st_size
    c10 = mpimg.imread('Compressed_10.jpeg')
    c30 = mpimg.imread('Compressed_30.jpeg')
    c50 = mpimg.imread('Compressed_50.jpeg')

    comp_size = (sys.getsizeof(encode[0][0]) + sys.getsizeof(encode[0][1])) * len(encode) + sys.getsizeof(
        slices) + sys.getsizeof(m) * 2
    print('Size Ratio:', img_size / comp_size)
    print('Size Ratio of JPEG Quality 10: ', image_s / c10_s)
    print('Size Ratio of JPEG Quality 30: ', image_s / c30_s)
    print('Size Ratio of JPEG Quality 50: ', image_s / c50_s)

    d_coff = []
    # Decode the Enhanced Run length encoding
    for rl in encode:
        run = rl[0]
        length = rl[1]
        for i in range(length):
            d_coff.append(run)

    d_coff = np.reshape(d_coff, (m, -1))
    d_image = pywt.array_to_coeffs(d_coff, slices, output_format='wavedec2')
    decompress_image = pywt.waverec2(d_image, wavelet=w)

    mse = np.square(np.subtract(image, decompress_image)).mean()
    mse_10 = np.square(np.subtract(image, c10/255)).mean()
    mse_30 = np.square(np.subtract(image, c30/255)).mean()
    mse_50 = np.square(np.subtract(image, c50/255)).mean()
    print('Mean Square Error:', mse)
    print('Mean Square Error of JPEG Quality 10:', mse_10)
    print('Mean Square Error of JPEG Quality 30:', mse_30)
    print('Mean Square Error of JPEG Quality 50:', mse_50)

    return decompress_image

# Simple run for compress and decompress
# comp = compress('C3_g.png', 0.05, 0.001)
# decomp = decompress('C3_g.png', comp)
# save_image(decomp, 'filename.png')
