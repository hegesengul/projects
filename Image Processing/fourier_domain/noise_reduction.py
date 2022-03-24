mport matplotlib.pyplot as plt
import matplotlib.image as mpimg
import matplotlib.colors as mpclr
import numpy as np

from copy import deepcopy


def fourier(img):
    return np.fft.fftshift(np.fft.fft2(img))

def inv_fourier(img):
    return np.fft.ifft2(np.fft.ifftshift(img))

def show_freq_img(img):
    plt.imshow(20 * np.log10(abs(img) + 0.000000001) , cmap= 'gray')
    plt.show()

def show_spatial_img(img):
    plt.imshow(abs(img) , cmap = 'gray')
    plt.show()

def ideal_threshold_filter(img , threshold):
    res = deepcopy(img)

    for i in range(len(res)):
        for j in range(len(res[0])):
            if abs(res[i][j]) < threshold:
                res[i][j] = complex(0)
            else:
                res[i][j] = complex(1 , 1)
    
    return res

def ideal_band_reject_filter(img , rmin , rmax):
    res = deepcopy(img)

    centery = len(img) // 2
    centerx = len(img[0]) // 2

    for i in range(centery - rmax , centery + rmax + 1):
        for j in range(centerx - rmax , centerx + rmax + 1):
            if rmin <= np.sqrt((i - centery)**2 + (j - centerx)**2) <= rmax :
                res[i][j] = complex(0 , 0)
    
    return res

def ideal_band_pass_filter(img , rmin , rmax):
    res = deepcopy(img)

    centery = len(img) // 2
    centerx = len(img[0]) // 2

    for i in range(len(img)):
        for j in range(len(img[0])):
            if not (rmin <= np.sqrt((i - centery)**2 + (j - centerx)**2) <= rmax):
                res[i][j] = complex(0 , 0)
    
    return res

def clear_vertical(img , dist , width):
    res = deepcopy(img)

    centerx = len(res[0]) // 2

    for i in range(len(res)):
        for j in range(centerx - dist - width , centerx - dist + width + 1):
            res[i][j] = complex(0)
        for j in range(centerx + dist - width , centerx + dist + width + 1):
            res[i][j] = complex(0)
    
    return res

def take_out_sides(img , width):
    res = deepcopy(img)

    centerx = len(res[0]) // 2

    for i in range(len(res)):
        for j in range(centerx - width):
            res[i][j] = complex(0)

    for i in range(len(res)):
        for j in range(centerx + width + 1 , len(res[0])):
            res[i][j] = complex(0)

    return res

def take_out_mid(img , width):
    res = deepcopy(img)

    centerx = len(res[0]) // 2

    for i in range(len(res)):
        for j in range(centerx - width , centerx + width + 1):
            res[i][j] = complex(0)

    return res

def d(u , v , img):
    return np.sqrt((u/len(img) - 0.5)**2 + (v/len(img[0]) - 0.5)**2)

def apply_spatial_filter(img , filter):
    filter = deepcopy(filter)
    # Pad the filter with zeros to make it as big as img
    diff = (img.shape[0] - filter.shape[0], img.shape[1] - filter.shape[1]) 
    diff = (((diff[0] + 1) // 2 , diff[0] // 2) , ((diff[1] + 1) // 2 , diff[1] // 2))
    filter = np.pad(filter, diff , 'constant')

    # Convert both to frequency domain then multiply, convert back and return the result
    return np.fft.ifftshift(np.real(np.fft.ifft2(np.fft.fft2(img) * np.fft.fft2(filter))))

def remove_square(img , xmin , xmax , ymin , ymax):
    res = deepcopy(img)

    for i in range(ymin , ymax + 1):
        for j in range(xmin , xmax + 1):
            res[i][j] = complex(0)
    
    return res

def split_and_clip(img , x_low , x_up , y_low , y_up):
    res = deepcopy(img)

    stepx = (x_up - x_low) // 100
    stepy = (y_up - y_low) // 100   

    while x_low < x_up and y_low < y_up:
        av = abs(res[y_low:y_low + stepy,x_low:x_low + stepx]).mean()

        res[y_low:y_low + stepy,x_low:x_low + stepx] = np.where(abs(abs(res[y_low:y_low + stepy,x_low:x_low + stepx]) - av) > 50 , complex(0) , res[y_low:y_low + stepy,x_low:x_low + stepx])

        x_low += stepx
        y_low += stepy

    return res 

def split_and_clip_(img , x_low , x_up , y_low , y_up):
    res = deepcopy(img)

    stepx = (x_up - x_low) // 20
    stepy = (y_up - y_low) // 20   

    if stepy > 0:
        while x_low < x_up and y_low < y_up:
            res[y_low:y_low + stepy,x_low:x_low + stepx] = complex(0)
            
            x_low += stepx
            y_low += stepy
    elif stepy < 0:
        while x_low < x_up and y_low > y_up:
            res[y_low + stepy:y_low,x_low:x_low + stepx] = complex(0)
            
            x_low += stepx
            y_low += stepy
    return res

def erase_lines(img):
    res = deepcopy(img)

    res[538:546 , :] = complex(0)
    res[595:603 , :] = complex(0)
    res[: , 878:886] = complex(0)
    res[: , 971:979] = complex(0)

    return res
#################################################################################################################
A1 = mpimg.imread("Images/A1.png")

plt.imsave("A1_denoised.png" , A1 , cmap = 'gray')


#################################################################################################################
A2 = fourier(mpimg.imread("Images/A2.png"))

A2_noise = ideal_band_reject_filter(ideal_band_reject_filter(ideal_threshold_filter(A2 , 35) , 0 ,65) , 85 , 280)


for i in range(len(A2)):
    for j in range(len(A2[0])):
        if A2_noise[i][j] == complex(1 , 1):
            A2[i][j] = complex(0)

#show_spatial_img(inv_fourier(A2))
plt.imsave("A2_denoised.png" , abs(inv_fourier(A2)) , cmap = 'gray')
#################################################################################################################

A3 = mpimg.imread("Images/A3.png")

red   = erase_lines(fourier(A3[...,0]))
green = erase_lines(fourier(A3[...,1]))
blue  = erase_lines(fourier(A3[...,2]))

red = split_and_clip_(red , 0 , 860 , 0 ,535)
red = split_and_clip_(red , 996 , len(A3[0]) - 1, 613 , len(A3) - 1)

green = split_and_clip_(green , 310  , 863  , 949 , 620)
green = split_and_clip_(green , 1008 , 1540 , 519 , 188)

blue = split_and_clip_(blue , 310  , 863  , 949 , 620)
blue = split_and_clip_(blue , 1008 , 1540 , 519 , 188)

red   = abs(inv_fourier(red))
blue  = abs(inv_fourier(blue))
green = abs(inv_fourier(green))

red   = np.where(red   > 1 , 1 , red  ) 
green = np.where(green > 1 , 1 , green) 
blue  = np.where(blue  > 1 , 1 , blue ) 

plt.imsave("A3_denoised.png" , np.dstack((red,green,blue)))
#################################################################################################################
