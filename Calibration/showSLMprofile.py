#!/usr/bin/env python
# coding: utf-8

# In[1]:



import sys
# insert at 1, 0 is the script path (or '' in REPL)
#sys.path.insert(1, 'D://Intel//SPIM//SLM-code//SLM')
sys.path.insert(1, 'D://Intel//SPIM//SLM-code//myfolder//slm_response//QSIM//slm_files')

import time
import cv2
import numpy as np
import matplotlib.pyplot as plt
import detect_heds_module_path
from holoeye import slmdisplaysdk



'''
pattern generation for

area = (1024, 1024)
bin = (16, 16)
blocks of 128 or 0
'''    
def checkerboard(shape, bin):
    phase = np.zeros(shape)
    for i in range(shape[0]//bin):
        for j in range(shape[1]//bin):
            phase[i*bin: i*bin + bin, j*bin: j*bin + bin] = 128*((i+j)%2)  
    return phase
 

   

outer_bins = 2**4 # 16

# check here 
active_area = (2**9,2**9) # (512, 512)
area = (2**10,2**10) # (1024, 1024)


# Initializes the SLM library
slm = slmdisplaysdk.SLMInstance()

# Check if the library implements the required version
if not slm.requiresVersion(3):
    exit(1)

# Detect SLMs and open a window on the selected SLM
error = slm.open()
assert error == slmdisplaysdk.ErrorCode.NoError, slm.errorString(error)

# Open the SLM preview window in "Fit" mode:
# Please adapt the file showSLMPreview.py if preview window
# is not at the right position or even not visible.
from showSLMPreview import showSLMPreview
showSLMPreview(slm, scale=0.0)


# Reserve memory for the data:
dataWidth = slm.width_px
dataHeight = slm.height_px

x = slmdisplaysdk.createFieldSingle(area[0],area[1])

print("0. Type of slmdisplaysdk.createFieldUChar: ",  type(x), x.dtype)
numbers_shape =  8
bins = active_area[0]//numbers_shape # 8
try:
    number_part= np.loadtxt('numbers.csv', delimiter=",") # = np.ones((64, 64)) #np.array([[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,16]])
except:
    numbers = np.random.uniform(0, 1, (numbers_shape,numbers_shape)) # np.ones((numbers_shape,numbers_shape)) #
    numbers = (numbers*1000).astype(int)
    print(numbers)
    np.savetxt("numbers.csv", numbers, delimiter=',')

number_part= np.loadtxt('numbers.csv', delimiter=",")
x = np.pi*checkerboard(area,outer_bins)/128 # 0 or 3.14
print("1. Type of checkerboard: ",  type(x), x.dtype)

temp = np.zeros((bins*number_part.shape[0],bins*number_part.shape[1]))

#################################################################
###Generate Constant Pattern to plot on Pattern Generator
#################################################################
'''
for i in range(number_part.shape[0]):
    for j in range(number_part.shape[1]):
        temp[i*bins:i*bins+bins, j*bins:j*bins+bins] = (i+j)%2#

temp = np.pi * temp

x[(area[0]-active_area[0])//2:(area[0]+active_area[0])//2, (area[1]-active_area[1])//2:(area[1]+active_area[1])//2] = temp
'''

#################################################################
###Generate Constant Pattern to plot on Pattern Generator
#################################################################


#################################################################
###Generate Number Partition Pattern to plot on Pattern Generator
#################################################################
for i in range(number_part.shape[0]):
    for j in range(number_part.shape[1]):
        temp[i*bins:i*bins+bins, j*bins:j*bins+bins] = number_part[i,j]#(i+j)%2#

#temp = np.pi * temp

check = np.ones(active_area)
for i in range(active_area[0]//2):
    for j in range(active_area[1]//2):
        check[i*2:i*2+2,j*2:j*2+2] = (-1)**(i+j) # variable = (−1)𝑗

# this is generation of "mask"
number_part=temp # now number_part has the same dim as slm spins[Imp]
eta0 = np.max(number_part)
mask_temp1 = np.arccos(number_part/eta0)
print("%%% ==> %%^%", check.shape, mask_temp1.shape)
mask_temp = check * mask_temp1 # used by mask variable = (−1)𝑗 cos−1 𝜁𝑚


N = number_part.shape[0]
v = np.ones((N//bins,N//bins)) #np.sign(np.random.normal(0,1,(N//bins,N//bins))+1e-7) 

# @R why v[i] it should be min_idx not i
x_temp = (v+1)*np.pi/2 + (np.pi/2) # adding 3pi/2 for v[i] = 1 or pi/2 for v[i] = -1


temp = np.zeros((number_part.shape[0],number_part.shape[1]))
for i in range(number_part.shape[0]//bins):
    for j in range(number_part.shape[1]//bins):
        temp[i*bins:i*bins+bins, j*bins:j*bins+bins] = x_temp[i,j]

mask = np.zeros(area)
mask[(area[0]-active_area[0])//2:(area[0]+active_area[0])//2, (area[1]-active_area[1])//2:(area[1]+active_area[1])//2] = mask_temp

x_temp = temp
x[(area[0]-active_area[0])//2:(area[0]+active_area[0])//2, (area[1]-active_area[1])//2:(area[1]+active_area[1])//2] = x_temp

#################################################################
###Generate Number Partition Pattern to plot on Pattern Generator
#   1290 508  1362 600 width 72 height 92
#################################################################

error = slm.showPhasevalues(x+mask)
#error = slm.showPhasevalues(x)
assert error == slmdisplaysdk.ErrorCode.NoError, slm.errorString(error)
time.sleep(0.15)

plt.imshow(x)
plt.show()

cv2.imwrite('color_img.jpg', x)