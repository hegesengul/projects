import numpy as np
import cv2
from copy import deepcopy
# Run one by one to find real results, when running same time results gets mix.
# I determine images parameter for you in end of code.


def segmentation_function(image, k, out_s, in_s, out_color, in_color, thresh_type, low_t, high_t):
    pixel_vals = image.reshape((-1, 3))
    # Convert to float type
    pixel_vals = np.float32(pixel_vals)
    # the below line of code defines the criteria for the algorithm to stop running,
    # which will happen is 10 iterations are run or the epsilon (which is the required accuracy)
    # becomes 85%
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 0.85)
    # then perform k-means clustering wit h number of clusters defined as 3
    # also random centres are initally chosed for k-means clustering
    retval, labels, centers = cv2.kmeans(pixel_vals, k, None, criteria, 10, cv2.KMEANS_RANDOM_CENTERS)

    # Reshaping the image into a 2D array of pixels and 3 color values (RGB)
    canny_image = np.copy(image).reshape((-1, 3))

    # Adjusting segments of image
    for cluster in out_s:
        canny_image[labels.flatten() == cluster] = out_color
    for cluster in in_s:
        canny_image[labels.flatten() == cluster] = in_color
    canny_image = cv2.Canny(canny_image, 100, 200).reshape(image.shape)

    imgray = cv2.cvtColor(canny_image, cv2.COLOR_BGR2GRAY)
    imgray = cv2.medianBlur(imgray, 13)

    if thresh_type:
        _, thresh = cv2.threshold(imgray, 50, 255, 0)
    else:
        _, thresh = cv2.threshold(imgray, 50, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)

    #  cv2.findContours uses topological structural analysis of digitized binary images by border following
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    index = 0
    a = []
    # Area thresholding
    if high_t > 0:
        for contour in contours:
            if index > 0:
                area = cv2.contourArea(contour)
                if low_t < area < high_t:
                    a.append(contours[index])
            index = index + 1
    else:
        for contour in contours:
            if index > 0:
                area = cv2.contourArea(contour)
                if low_t < area:
                    a.append(contours[index])
            index = index + 1

    contour_image = np.copy(image)
    # Drawing contours
    cv2.drawContours(contour_image, a, -1, (0, 0, 255), 3)

    return contour_image


b1 = cv2.imread('Images/B1.jpg')
b2 = cv2.imread('Images/B2.jpg')
b3 = cv2.imread('Images/B3.jpg')
b4 = cv2.imread('Images/B4.jpg')
b5 = cv2.imread('Images/B5.jpg')

# Run one by one to find real results, when running same time results gets mix.

# cv2.imwrite("B1_output_algo1.png", segmentation_function(b1, 12, [], [0, 4, 10], [], [0, 0, 255], False, 80000, -1))
# cv2.imwrite("B2_output_algo1.png", segmentation_function(b2, 10, [], [2,3], [], [0, 0, 255], False, 2600, 181000))
cv2.imwrite("B3_output_algo1.png", segmentation_function(b3, 4, [3, 7], [], [0, 0, 0], [], True, 54000, -1))
# cv2.imwrite("B4_output_algo1.png", segmentation_function(b4, 10, [4, 5, 8, 9], [], [255, 0, 0], [], True, 600, -1))
# cv2.imwrite("B5_output_algo1.png", segmentation_function(b5, 10, [4, 5], [], [255, 0, 0], [], True, 80000, -1))


#########################################################################################################################

def segmentation_function2(name , threshold_green , threshold_blue , op_it , cl_it , er_it , size):
    img = cv2.imread('Images/' + name + ".jpg")
    
    image = deepcopy(img)
    
    img = img.astype(float)

    img[img[:,:,1] > (img[:,:,0] + img[:,:,2]) * threshold_green] = np.uint8(np.array([0,0,0]))

    img[(img[:,:,0] > (img[:,:,2] + img[:,:,1]) * threshold_blue) & (img[:,:,0] + img[:,:,2] + img[:,:,1] > 230)] = np.array([0.0,0.0,0.0])

    img = np.mean(img , -1)
    
    img[img > 15] = np.uint8(np.array([255]))

    img = img.astype(np.uint8)

    img = cv2.morphologyEx(img, cv2.MORPH_OPEN, cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(size,size)) , iterations= op_it)
    img = cv2.morphologyEx(img, cv2.MORPH_CLOSE, cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(size,size)) , iterations= cl_it)

    boundary = img
    img = cv2.morphologyEx(img, cv2.MORPH_ERODE, cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(size,size)) , iterations= er_it)
    
    boundary = boundary - img

    image[boundary >= 230] = np.uint8(np.array([147,20,255]))

    cv2.imwrite(name + "_output_algo2.jpg"  , image)


segmentation_function2("B1" , 0.57 , 0.7 , 1 , 5 , 1 , 5)
segmentation_function2("B2" , 0.57 , 0.5 , 1 , 5 ,1 , 7)
segmentation_function2("B3" , 0.57 , 0.48, 1 , 5 , 1 , 7)
segmentation_function2("B4" , 0.57 , 0.5 , 1 , 5 , 1 , 7)
segmentation_function2("B5" , 0.57 , 0.5 , 6 , 5 , 2 , 3)
