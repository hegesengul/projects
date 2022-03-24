import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import matplotlib.colors as mpclr
import numpy as np
from copy import deepcopy

# 9x9 laplacian of gaussian
LoG = np.array([[0,0,-3,-2,-2,-2,-3,0,0],[0,-2,-3,-5,-5,-5,-3,-2,0],[-3,-3,-5,-3,0,-3,-5,-3,-3],[-2,-5,-3,12,23,12,-3,-5,-2],[-2,-5,0,23,40,23,0,-5,-2],[-2,-5,-3,12,23,12,-3,-5,-2],[-3,-3,-5,-3,0,-3,-5,-3,-3],[0,-2,-3,-5,-5,-5,-3,-2,0],[0,0,-3,-2,-2,-2,-3,0,0]])


def fourier(img):
    return np.fft.fftshift(np.fft.fft2(img))

def inv_fourier(img):
    return np.real(np.fft.ifft2(np.fft.ifftshift(img)))

def show_freq_img(img):
    plt.imshow(20 * np.log10(abs(img) + 0.000000001) , cmap = 'gray')
    plt.show()

def show_spatial_img(img):
    plt.imshow(abs(img) , cmap = 'gray')
    plt.show()

def get_I_component(img):
    return np.mean(img, -1)

def get_lum(img):
    return img[...,0] * 0.299 + img[...,1] * 0.587 + img[...,2] * 0.114

def ideal_high_pass_filter(img , radius):
    res = deepcopy(img)
    
    centerx = len(img[0]) // 2
    centery = len(img) // 2

    for i in range(centery - radius , centery + radius + 1):
        for j in range(centerx - radius , centerx + radius + 1):
            if np.sqrt((i - centery)**2 +(j - centerx)**2) < radius:
                res[i][j] = complex(0)
    
    return res

def d(u , v , img):
    return (u/len(img) - 0.5)**2 + (v/len(img[0]) - 0.5)**2
    
def butterworth_high_pass_filter(img , radius , n):
    res = deepcopy(img)
    
    centerx = len(img[0]) // 2 - .5
    centery = len(img) // 2 - .5
    
    for i in range(len(res)):
        for j in range(len(res[0])):
            d = (i - centery)**2 + (j - centerx)**2

            if not d == 0:
                res[i][j] *= 1/(1 + (radius**2 / d)**n)
    
    return res

def apply_spatial_filter(img , filter):
    filter = deepcopy(filter)
    # Pad the filter with zeros to make it as big as img
    diff = (img.shape[0] - filter.shape[0], img.shape[1] - filter.shape[1]) 
    diff = (((diff[0] + 1) // 2 , diff[0] // 2) , ((diff[1] + 1) // 2 , diff[1] // 2))
    filter = np.pad(filter, diff , 'constant')

    # Convert both to frequency domain then multiply, convert back and return the result
    return np.fft.ifftshift(np.real(np.fft.ifft2(np.fft.fft2(img) * np.fft.fft2(filter))))

# Load Images and convert them to grayscale
B1 = get_lum(mpimg.imread("Images/B1.jpg"))
B2 = get_lum(mpimg.imread("Images/B2.jpg"))
B3 = get_lum(mpimg.imread("Images/B3.jpg"))


# For B1 and B2 , we apply the 9x9 laplacian of gaussian filter. For B3 we will use butterworth high pass filter
B1 = apply_spatial_filter(B1 , LoG)
B2 = apply_spatial_filter(B2 , LoG)
B3 = apply_spatial_filter(B3 , LoG)
#show_spatial_img(inv_fourier(butterworth_high_pass_filter(B1_fourier , 48 , 2)))#1
#show_spatial_img(inv_fourier(butterworth_high_pass_filter(B1_fourier , 100 , 5)))#2
#res= inv_fourier(butterworth_high_pass_filter(B1_fourier , 10 , 1))

# Save the results and exit
plt.imsave("B1_output.png" , B1 , vmin = 0 , vmax = 255 , cmap = 'gray')
plt.imsave("B2_output.png" , B2 , vmin = 0 , vmax = 255 , cmap = 'gray')
plt.imsave("B3_output.png" , B3 , vmin = 0 , vmax = 255 , cmap = 'gray')
