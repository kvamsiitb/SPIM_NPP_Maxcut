#!/usr/bin/python
# -*- coding: utf-8 -*-

# In[1]: polarization 45

### IMPORTING REQUISITE PACKAGES

import sys

# insert at 1, 0 is the script path (or '' in REPL)
# sys.path.insert(1, 'D://Intel//SPIM//SLM-code//SLM')

sys.path.insert(1,
                'D://Intel//SPIM//SLM-code//myfolder//slm_response//QSIM//slm_files'
                )

import numpy as np
import threading
import time
import copy
import matplotlib
import matplotlib.pyplot as plt

import csv
import os
from os.path import isfile, join
import random
from random import randint
import cv2
import pandas as pd
# import scipy as sp
# from scipy.signal import *

import cProfile
import pstats
import io
from pypylon import pylon
from pypylon import genicam
import sys  # We need sys so that we can pass argv to QApplication

import detect_heds_module_path
from holoeye import slmdisplaysdk

import matplotlib.colors as mcolors

import argparse


#beta = np.arange(0.004, 1, 0.00025)




def spin_tuple(shape, bin_size):
    arr = []
    for i in range(0, shape[0] // bin_size):
        for j in range(0, shape[1] // bin_size):
            arr.append((i, j))
    return arr


def checkerboard(shape, bin):
    phase = np.zeros(shape)
    for i in range(shape[0] // bin):
        for j in range(shape[1] // bin):
            phase[i * bin:i * bin + bin, j * bin:j * bin + bin] = 128 \
                * ((i + j) % 2)
    return phase


def flip_np(
    spin_arr,
    x,
    tot_shape,
    shape,
    bin,
    d):
    # print("what is the spin_flips: ", d)
    y = copy.copy(x)
    l = random.sample(spin_arr, d)
    for i in range(d):
        y[(tot_shape[0] - shape[0]) // 2 + l[i][0] * bin:(tot_shape[0]
          - shape[0]) // 2 + l[i][0] * bin + bin, (tot_shape[1]
          - shape[1]) // 2 + l[i][1] * bin:(tot_shape[1] - shape[1])
          // 2 + l[i][1] * bin + bin] = (np.pi + y[(tot_shape[0]
                - shape[0]) // 2 + l[i][0] * bin:(tot_shape[0]
                - shape[0]) // 2 + l[i][0] * bin + bin, (tot_shape[1]
                - shape[1]) // 2 + l[i][1] * bin:(tot_shape[1]
                - shape[1]) // 2 + l[i][1] * bin + bin]) % (2 * np.pi)
    return y






def exp_decay(initial_temp, final_temp,num_iters, Critical_temp): # T_c = 2(N - 1) where N = number of spins
    A = initial_temp - final_temp
    B = final_temp
    alpha = (-2.0/num_iters)*np.log( (Critical_temp - final_temp)/(initial_temp - final_temp))
    print("alpha ", alpha)
    num_range = np.arange(0, num_iters, 1)
    temperture_list = []
    for i in num_range:
        temperture_list.append( A*np.exp(-alpha*i) + B)
    temperture_list = np.array(temperture_list)
    return temperture_list, num_range



# python number_partitioning_v3.py --number_file numbers16.csv --spin_flips 8 --x1 1015 --y1 543 --x2 1020 --y2 547 --EXP_TIME 2899 --initial_temp 530 --num_iters 1000 --Critical_temp 510 --final_temp 0.01
def main():
    spinflip = []

    loss_arr = []
    loss_arr.append(255)
    fidelity_arr = []
    

    intensities = []
    Hamiltonian_energies = []   
    
    parser = argparse.ArgumentParser(description='Input to SPIM like interaction strenght, Ising spin size, expusore time.')
    # Ising spec
    parser.add_argument('--bins', help='Ising spin size', default = 64, type=int)
    parser.add_argument('--outer_bins', help='other spins/Outside Ising model', default = 16, type=int)
    parser.add_argument('--active_area', help='size of Ising model',  default = 512, type=int)
    parser.add_argument('--area', help='size of SLM occupied',  default = 1024, type=int)
    parser.add_argument('--number_file', type=str, default='numbers.csv',
                    help='Choose filename: "numbers8.csv" or "numbers4.csv".')

    parser.add_argument('--final_temp', help='final temp for MH algo',  default = 0.01, type=float)
    parser.add_argument('--initial_temp', help='initial temp for MH algo',  default = 600, type=float)
    parser.add_argument('--Critical_temp', help='Critical temp for MH algo',  default = -1.0, type=float)
    parser.add_argument('--num_iters', help='number of iterations for MH algo',  default = 300,type=int)
    parser.add_argument('--spin_flips', help='number of spins to be flip in single iteration',  default = 1,type=int)  
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
    args = parser.parse_args()

    # SLM specs
    bins =  args.bins
    outer_bins = args.outer_bins
    active_area = (args.active_area, args.active_area)
    area = (args.area, args.area)
    folder = ".//numbers_list//"
    filename = args.number_file
    filename = folder + filename
    count = 0
    
    mask = np.zeros(area)
    spin_flips = args.spin_flips
    # Cam Spec
    imgx1 = args.x1
    imgx2 = args.x2
    imgy1 = args.y1
    imgy2 = args.y2

    EXP_TIME = args.EXP_TIME*1.0
    
    number_part = np.loadtxt(filename, delimiter=',')  # = np.ones((64, 64)) #np.array([[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,16]])
    number_shape = number_part.shape
    bins_height = active_area[0]//number_shape[0]
    bins_width = active_area[1]//number_shape[1]
    assert(bins_height == bins_width)
    bins = bins_height
    assert( number_shape[0] == active_area[0]//bins and   number_shape[1] == active_area[1]//bins)
    spin_arr = spin_tuple(active_area, bins)
    zeta = number_part.reshape(1, -1)
    print("zeta", zeta)
    J_matrix = zeta * np.transpose(zeta)
    print("J_matrix",J_matrix)
    
    
    
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
    temp = np.zeros((bins * number_part.shape[0], bins
                    * number_part.shape[1]))

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
         // 2, (area[1] - active_area[1]) // 2:(area[1]
         + active_area[1]) // 2] = mask_temp

    x_temp = temp
    x[(area[0] - active_area[0]) // 2:(area[0] + active_area[0]) // 2,
      (area[1] - active_area[1]) // 2:(area[1] + active_area[1])
      // 2] = x_temp

    plt.imshow(x + mask)
    plt.show()

    error = slm.showPhasevalues(x + mask)
    assert error == slmdisplaysdk.ErrorCode.NoError, slm.errorString(error)
    time.sleep(0.15)

    camera = \
        pylon.InstantCamera(pylon.TlFactory.GetInstance().CreateFirstDevice())
    camera.Open()
    
    
    try:

        camera.ExposureTime.SetValue(EXP_TIME)

        # Print the model name of the camera.

        print ('Using device ', camera.GetDeviceInfo().GetModelName())
        numberOfImagesToGrab = 1
        camera.StartGrabbingMax(numberOfImagesToGrab)
        while camera.IsGrabbing():
            grabResult = camera.RetrieveResult(15000,
                    pylon.TimeoutHandling_ThrowException)
            if grabResult.GrabSucceeded():
                img = grabResult.Array  # img = np.asarray(grabResult.Array)
                print ('Shape of the target image is: ', img.shape)

            grabResult.Release()

        # print('as', img.shape)
        # print('sas',img[850:874, 593:618].shape)

        img1 = img[imgy1:imgy2, imgx1:imgx2]  # img[ 550:623, 852:876]# crop_img = img[y:y+h, x:x+w]
        imgplot = plt.imshow(img1)
        plt.show()
        cv2.imshow('image', img)
        cv2.waitKey(0)
        #cv2.destroyAllWindows()
        #cv2.imshow('image', cv2.cvtColor(img1, cv2.COLOR_GRAY2BGR))
        cv2.imwrite('Intensity.png', img)
        #cv2.waitKey(0)
        cv2.destroyAllWindows()
    except genicam.GenericException as e:

        # Error handling.

        print('An exception occurred.')
        print( e.GetDescription())
        exitCode = -1
        sys.exit(exitCode)

    exp_time = 45
    
    
    final_temp = args.final_temp
    initial_temp = args.initial_temp
    num_iters = args.num_iters
    
    
    if args.Critical_temp == -1.0:
        if args.Critical_temp < initial_temp:
            Critical_temp = 2.0*(number_shape[0]*number_shape[1] - 1)
        else:
            Critical_temp = (initial_temp + final_temp)/2
    else:
        Critical_temp = args.Critical_temp
        
    print()
    '''
    final_temp = 50.0
    initial_temp = 200.0
    num_iters = 3e4
    Critical_temp = 2.0*(number_shape[0]*number_shape[1] - 1)  
    '''
    
    #assert Critical_temp < initial_temp
    #exp_decay(initial_temp, final_temp,num_iters)
    temperture_list, num_range = exp_decay(initial_temp, final_temp, num_iters, Critical_temp)
    #num_range = num_range1[int(num_iters//2) - 1000:]
    #temperture_list = temperture_list1[int(num_iters//2) - 1000:]
    print ('Optimal exposure time is: ', EXP_TIME, ' microseconds')
    camera.ExposureTime.SetValue(EXP_TIME * 1.0)

    print ('Set the exposure time ==> %%%%%%%%%%%%%%%%%%%% ', EXP_TIME)

    MIN_FIDE = np.inf
    FINAL_SPIN = []
    REPEAT_CAM_FLAG = True
    
    start = time.time()
    while not count == len(temperture_list) - 1:
        if REPEAT_CAM_FLAG:       
            print ('%%% ==> %%^%')
            x2 = flip_np(
                spin_arr,
                x,
                area,
                active_area,
                bins,
                spin_flips)

            error = slm.showPhasevalues(x2 + mask)
            time.sleep(0.15)

        camera.StartGrabbingMax(1)
        while camera.IsGrabbing():
            # Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            grabResult = camera.RetrieveResult(15000,
                    pylon.TimeoutHandling_ThrowException)
            
            # Image grabbed successfully?
            if grabResult.GrabSucceeded():
                # Access the image data.
                # img = rebin(grabResult.Array, imgbin)

                img = grabResult.Array[imgy1:imgy2, imgx1:imgx2]
                C1 = np.sum(img) / np.size(img)
                delE = (C1) - (loss_arr[-1]) # H = -intensity
                # print(C1, COST[-1], delE)
                # try:
                #p = np.exp(-1 * (1.0 / Temperature[count]) * delE)
                # except:
                prob_ratio = np.exp(-1*(1.0/temperture_list[count])*delE)
                acceptance_probability = min([1.0, prob_ratio]);
                if delE <= 0:
                    x = x2
                    # print("Accepted")

                    loss_arr.append(C1)
                    spinflip.append(spin_flips)
                elif np.random.uniform() >= acceptance_probability: # np.random.choice(a=[0,1], p=[p, 1-p])==0:
                    x = x2
                    loss_arr.append(C1)
                    spinflip.append(spin_flips)
                else:
                    x = x
                    loss_arr.append(loss_arr[-1])
                    spinflip.append(0)

                count += 1
                final_screen = x[(area[0] - active_area[0]) // 2:
                                 (area[0] + active_area[0]) // 2,
                                 (area[1] - active_area[1]) // 2:
                                 (area[1] + active_area[1]) // 2]
                
                # np.savetxt('text_final_screen.txt', final_screen, delimiter=',')


                # #np.savetxt('text_S2.txt', s2, delimiter=',')
                '''

                s1 = np.where(final_screen < np.pi)
                s2 = np.where(final_screen >= np.pi)                
                fidelity = (np.sum(number_part[s1])
                            - np.sum(number_part[s2])) \
                    / (np.sum(number_part[s1])
                       + np.sum(number_part[s2]))
                '''
                # @R //here
                # precision(Energy = final/initial) vs iteration
                # remove DC offset(FFT)
                

                spins = []
                for i in range(number_shape[0]):
                    for j in range(number_shape[1]):
                        num_set = np.unique(final_screen[i * bins:i
                                * bins + bins, j * bins:j * bins
                                + bins])
                        assert num_set.shape[0] == 1
                        if num_set[0] < np.pi:
                            spins.append(-1.0)
                        elif num_set[0] >= np.pi:
                            spins.append(1.0)
                fide = zeta * spins # @R
                if np.abs(fide.sum()) < MIN_FIDE:
                    FINAL_SPIN = spins
                    MIN_FIDE = np.abs(fide.sum())
                Hamilton_ene = 0.0
                
                for ii in range(len(spins)):
                    for jj in range(len(spins)):
                        Hamilton_ene += J_matrix[ii][jj]*spins[ii]*spins[jj]
                
                Hamiltonian_energies.append(Hamilton_ene)
                print (
                    '\nFidely: ',
                    fide.sum(),
                    'Temperature: ',
                    temperture_list[count],
                    'delE: ',
                    delE,
                    ' Hamiltonian: ',
                    Hamilton_ene
                    )

            # DA vs SPIM

                fidelity_arr.append(fide.sum())
                
                REPEAT_CAM_FLAG = True
            else:
                REPEAT_CAM_FLAG = False
                #print ('Error: ', grabResult.ErrorCode,                      grabResult.ErrorDescription)
            grabResult.Release()

    print (loss_arr)
    end = time.time()
    elapsed = end - start
    print("time taken {} for this iteration {}".format(elapsed, count))
    # CLOSING DOWN THE INSTRUMENTS

    camera.Close()
   

    # Wait until the SLM process is closed:
    print ('Waiting for SDK process to close. Please close the tray icon to continue ...')
    error = slm.utilsWaitUntilClosed()
    assert error == slmdisplaysdk.ErrorCode.NoError, \
        slm.errorString(error)

    # Unloading the SDK may or may not be required depending on your IDE:

    slm = None
    folder_name = ".//thesis//{}x{}_spins//".format(number_shape[0], number_shape[1])
    
    temperture_list =  np.array(temperture_list).reshape(1, -1)
    num_range = np.array(num_range).reshape(1, -1)
    loss_arr = np.array(loss_arr).reshape(1, -1)
    
    fidelity_arr = np.array(fidelity_arr).reshape(1, -1)
    
    Hamiltonian_energies = np.array(Hamiltonian_energies).reshape(1, -1)
    
    
    FINAL_SPIN = np.array(FINAL_SPIN).reshape(1, -1)
    np.savetxt(folder_name + 'FINAL_SPIN.csv', FINAL_SPIN, delimiter='\n')
    # SHOWING ALL THE PLOTS GENERATED
    print(temperture_list[0,1:])
    print()
    print(fidelity_arr[0])
    if(temperture_list.shape[1]-1 == num_range.shape[1]-1 == loss_arr.shape[1]-1 == fidelity_arr.shape[1] == Hamiltonian_energies.shape[1]):
        print("Dataframe")
        dataframae = {'num_range':num_range[0,1:], 'temperture_list':temperture_list[0,1:], 'Intensity':loss_arr[0,1:], 'Magnetization': fidelity_arr[0], 'Hamiltonian_energies': Hamiltonian_energies[0]}
        
        dataframae_df = pd.DataFrame(data=dataframae)
        dataframae_df.to_csv(folder_name + "all_data.csv", encoding='utf-8', index=False)
    else:
        print("No Dataframe")
        np.savetxt(folder_name + 'num_range.csv', num_range, delimiter='\n')
        np.savetxt(folder_name + 'temperture_list.csv', temperture_list, delimiter='\n')
        np.savetxt(folder_name + 'loss_arr.csv', loss_arr, delimiter='\n')
        np.savetxt(folder_name + 'fidelity_arr.csv', fidelity_arr, delimiter='\n')
        np.savetxt(folder_name + 'Hamiltonian_energies.csv', Hamiltonian_energies, delimiter='\n')
        
        
    
    plt.imshow(final_screen)
    plt.show()
    print()# ('fidelity', fidelity_arr)
    print()# ('loss', loss_arr)
    print()
    print()
    print("MIN_FIDE: ", MIN_FIDE)
    print()
    print("FINAL_SPIN: ", FINAL_SPIN)


if __name__ == '__main__':
    main()

