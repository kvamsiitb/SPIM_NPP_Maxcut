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

from pypylon import pylon
from pypylon import genicam

import argparse
# Calibration_v3.py --x1 1015 --y1 542 --x2 1020 --y2 547 --bins 64 --EXP_TIME 2899 --saturate_percent 40
   
def checkerboard(shape, bin):
    phase = np.zeros(shape)
    for i in range(shape[0]//bin):
        for j in range(shape[1]//bin):
            phase[i*bin: i*bin + bin, j*bin: j*bin + bin] = 128*((i+j)%2)  
    return phase
 
def grab_image(imgx1, imgy1, imgx2, imgy2, EXP_TIME, saturate_percent):
    bool_Exp = 0
    try:
        print("Using device ", camera.GetDeviceInfo().GetModelName())
        camera.Open()       
        camera.ExposureTime.SetValue(EXP_TIME*1.0)
        # Print the model name of the camera.
        numberOfImagesToGrab = 1
        camera.StartGrabbingMax(numberOfImagesToGrab)
        while camera.IsGrabbing():       
            grabResult = camera.RetrieveResult(15000, pylon.TimeoutHandling_ThrowException)#15000
            if grabResult.GrabSucceeded():
                #assert(camera.Width.Max == grabResult.Width)
                #assert(camera.Height.Max == grabResult.Height)
                img = (grabResult.Array)[imgy1:imgy2, imgx1:imgx2]             
                #print("Shape of the target image is: ", img.shape)
                sat_val = np.sum(img == 255)
                num_val = np.size(img)
                sat_percent = (sat_val/num_val)*100
                if sat_percent <= saturate_percent:
                    bool_Exp = 1
                mean_intensity = np.sum(img) / np.size(img)
            else:
                camera.Close()
            grabResult.Release()
        camera.Close()
        
    except genicam.GenericException as e:
        print("Could not grab an image: ", e.GetDescription())
        camera.Close()
        sys.exit("Image cant capture by camera ")
    return bool_Exp, mean_intensity




def main():
    
    parser = argparse.ArgumentParser()
    # Ising spec
    parser.add_argument('--bins', help='Ising spin size', default = 64, type=int)
    parser.add_argument('--outer_bins', help='other spins/Outside Ising model', default = 16, type=int)
    parser.add_argument('--active_area', help='size of Ising model',  default = 512, type=int)
    parser.add_argument('--area', help='size of SLM occupied',  default = 1024, type=int)
    
    # Camera Spec
    
    parser.add_argument('--x1', required=True,
      help='top left point X in rectange', type=int)
    parser.add_argument('--y1', required=True,
      help='top left point Y in rectange', type=int)
    parser.add_argument('--x2', required=True,
      help='bottom right point X in rectange', type=int)  
    parser.add_argument('--y2', required=True,
      help='bottom right point X in rectange', type=int)

    parser.add_argument('--EXP_TIME', required=True,
      help='Exposure time of Camera', type=int) 
    parser.add_argument('--saturate_percent',help='saturate_percent for exp time in camera',   default = 20, type=int)  
    args = parser.parse_args()
    
    # SLM specs
    bins =  args.bins
    outer_bins = args.outer_bins
    active_area = (args.active_area, args.active_area)
    area = (args.area, args.area)
    mask = np.zeros(area)
    
    # Cam Spec
    imgx1 = args.x1
    imgx2 = args.x2
    imgy1 = args.y1
    imgy2 = args.y2


    ##
    assert(active_area[0]%bins == 0) # so number_shape is integer
    number_part = np.ones((active_area[0]//bins, active_area[1]//bins))

    number_shape = number_part.shape#(active_area[0]//bins, active_area[1]//bins) 





    # Initializes the SLM library
    slm = slmdisplaysdk.SLMInstance()

    # Check if the library implements the required version
    if not slm.requiresVersion(3):
        exit(1)

    # Detect SLMs and open a window on the selected SLM
    error = slm.open()
    assert error == slmdisplaysdk.ErrorCode.NoError, \
        slm.errorString(error)

    # Open the SLM preview window in "Fit" mode:
    # Please adapt the file showSLMPreview.py if preview window
    # is not at the right position or even not visible.
    from showSLMPreview import showSLMPreview
    showSLMPreview(slm, scale=0.0)

    # Reserve memory for the data:
    dataWidth = slm.width_px
    dataHeight = slm.height_px

    x = slmdisplaysdk.createFieldUChar(area[0], area[1])
    
    x = np.pi * checkerboard(area, outer_bins) / 128  # 0 or 3.14


    #################################################################
    ###Generate Constant Pattern to plot on Pattern Generator
    #################################################################

    temp = np.zeros((bins * number_part.shape[0], bins* number_part.shape[1]))

    for i in range(number_part.shape[0]):
        for j in range(number_part.shape[1]):
            temp[i * bins:i * bins + bins, j * bins:j * bins + bins] = \
                number_part[i, j]
    check = np.ones(active_area)
    for i in range(active_area[0] // 2):
        for j in range(active_area[1] // 2):
            check[i * 2:i * 2 + 2, j * 2:j * 2 + 2] = (-1) ** (i + j)  # variable = (−1)𝑗
            
            

    # this is generation of "mask"

    number_part = temp  # now number_part has the same dim as slm spins[Imp]
    eta0 = np.max(number_part)
    mask_temp1 = np.arccos(number_part / eta0)
    print ('%%% ==> %%^%', check.shape, mask_temp1.shape)
    mask_temp = check * mask_temp1  # used by mask variable = (−1)𝑗 cos−1 𝜁𝑚

    N = number_part.shape[0]
    v = np.ones((N // bins, N // bins))#np.sign(np.random.normal(0, 1, (N // bins, N // bins)) + 1e-7)

    x_temp = (v + 1) * np.pi / 2 + np.pi / 2  # adding 3pi/2 for v = 1 or pi/2 for v = -1

    temp = np.zeros((number_part.shape[0], number_part.shape[1]))
    for i in range(number_part.shape[0] // bins):
        for j in range(number_part.shape[1] // bins):
            temp[i * bins:i * bins + bins, j * bins:j * bins + bins] = \
                x_temp[i, j]

    mask[(area[0] - active_area[0]) // 2:(area[0] + active_area[0])
         // 2, (area[1] - active_area[1]) // 2:(area[1]+ active_area[1]) // 2] = mask_temp

    x_temp = temp
    x[(area[0] - active_area[0]) // 2:(area[0] + active_area[0]) // 2,
      (area[1] - active_area[1]) // 2:(area[1] + active_area[1])
      // 2] = x_temp

    plt.imshow(x + mask)
    plt.show()
            

    error = slm.showPhasevalues(x + mask)
    assert error == slmdisplaysdk.ErrorCode.NoError, slm.errorString(error)
    time.sleep(0.15)
    
    # Auto-Calibration
    global camera
    camera = pylon.InstantCamera(
            pylon.TlFactory.GetInstance().CreateFirstDevice())
    
    curr_EXP_TIME_List = np.arange(args.EXP_TIME, 100.0, -10.0)
    
    for exp_time in curr_EXP_TIME_List:
        bool_exp, mean_intensity = grab_image(imgx1, imgy1, imgx2, imgy2, exp_time, args.saturate_percent)
        if bool_exp != 0:
            FINAL_EXP_TIME = exp_time 
            FINAL_MEAN_INTENSITY = mean_intensity
            break
    
    
    # Wait until the SLM process is closed:
    print ('Waiting for SDK process to close. Please close the tray icon to continue ...')
    error = slm.utilsWaitUntilClosed()
    assert error == slmdisplaysdk.ErrorCode.NoError, \
        slm.errorString(error)

    # Unloading the SDK may or may not be required depending on your IDE:

    slm = None
    print()
    print()
    input("Switch off the laser to calculate the ambient light, we will remove this value from the intenity in the SPIM ")
    mean_intensities = []
    for i in range(10):
        _, mean_intensity1 = grab_image(imgx1, imgy1, imgx2, imgy2, FINAL_EXP_TIME, args.saturate_percent)
        mean_intensities.append(mean_intensity1)
    avg_intenisty = np.array(mean_intensities).sum()/10
    
    print()
    print()
    print("FINAL_EXPOSURE_TIME for SPIM operations: ", FINAL_EXP_TIME)
    print()
    print("FINAL_MEAN_INTENSITY for SPIM operations: ", FINAL_EXP_TIME)
    print()
    print("Mean noise floor for SPIM in the selected area ", avg_intenisty)
    
if __name__ == '__main__':
    main()


