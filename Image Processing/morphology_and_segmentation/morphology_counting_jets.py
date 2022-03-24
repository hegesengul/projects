import cv2
import numpy as np

def count_jets(binary_image):
    res , _ = cv2.connectedComponents(binary_image)

    # -1 because we do not want to count the background
    return res - 1


####################
######## A1 ########
####################

im1 = cv2.imread("Images/A1.png" , cv2.IMREAD_GRAYSCALE)


kernel = np.uint8(np.array([[1,1,1,1,1,1],\
                            [1,1,1,1,1,1],\
                            [1,1,1,1,1,1],\
                            [1,1,1,1,1,1],\
                            [1,1,1,1,1,1],\
                            [1,1,1,1,1,1]]))


im1 = cv2.adaptiveThreshold(im1,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY_INV,1501,5)

im1 = cv2.erode(im1 , kernel , iterations = 1)
im1 = cv2.dilate(im1 , kernel , iterations = 2)

print("A1:" + str(count_jets(im1)) + " jets")

cv2.imwrite('part1_A1.png', im1)

####################
######## A2 ########
####################

im2= cv2.imread("Images/A2.png" , cv2.IMREAD_GRAYSCALE)

im2 = cv2.adaptiveThreshold(im2,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY_INV,511,100)

kernel = np.uint8(np.array([[1,1],\
                            [1,1]]))

im2 = cv2.erode(im2 , kernel , iterations = 1)
im2 = cv2.dilate(im2 , kernel , iterations = 85)

print("A2:" + str(count_jets(im2)) + " jets")

cv2.imwrite('part1_A2.png', im2)

####################
######## A3 ########
####################

im3 = cv2.imread("Images/A3.png" , cv2.IMREAD_GRAYSCALE)

im3 = cv2.adaptiveThreshold(im3,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY_INV,3,5)

kernel = np.uint8(np.array([[1,1],[1,1]]))

im3 = cv2.erode(im3 , kernel , iterations = 1)
im3 = cv2.dilate(im3 , kernel , iterations = 30)

print("A3:" + str(count_jets(im3)) + " jets")

cv2.imwrite('part1_A3.png', im3)

####################
######## A4 ########
####################

im4 = cv2.imread("Images/A4.png" , cv2.IMREAD_GRAYSCALE)

im4 = cv2.adaptiveThreshold(im4,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY_INV,11,20)

kernel = np.uint8(np.array([1,1,1]))
kernel1 = np.uint8(np.array([[1,1,1],[1,1,1],[1,1,1]]))

im4 = cv2.erode(im4 , kernel , iterations = 4)
im4 = cv2.dilate(im4 , kernel1 , iterations = 5)

print("A4:" + str(count_jets(im4)) + " jets")

cv2.imwrite('part1_A4.png', im4)

####################
######## A5 ########
####################

im5 = cv2.imread("Images/A5.png" , cv2.IMREAD_GRAYSCALE)

im5 = cv2.adaptiveThreshold(im5,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY_INV,9,2)

kernel = np.uint8(np.array([[1,1,1],[1,1,1]]))
kernel1 = np.uint8(np.array([[1,1,1],[1,1,1],[1,1,1]]))

im5 = cv2.erode(im5 , kernel , iterations = 4)
im5 = cv2.dilate(im5, kernel1 , iterations = 100)

print("A5:" + str(count_jets(im5)) + " jets")

cv2.imwrite('part1_A5.png', im5)

####################
######## A6 ########
####################

im6 = cv2.imread("Images/A6.png" , cv2.IMREAD_GRAYSCALE)

im6 = cv2.adaptiveThreshold(im6,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY_INV,301,90)

kernel = np.uint8(np.array([[1,1,1],[1,1,1]]))

im6 = cv2.erode(im6 , kernel , iterations = 4)
im6 = cv2.dilate(im6 , kernel , iterations = 18)

print("A6:" + str(count_jets(im6)) + " jets")

cv2.imwrite('part1_A6.png', im6)
