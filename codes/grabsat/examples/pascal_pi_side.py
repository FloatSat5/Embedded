#!/usr/bin/env python3

# for serial UART
import serial, time
# for opencv
import cv2 
import numpy as np
import math 
#for camera
import time, libcamera
from picamera import PiCamera

counter = 0

def get_circle_Hough(image) :   
    
    # temporary image for ranging
    image_temp = image.copy()

    # define threshold for color filtering, defines scale of white that is left over
    lower_white = np.array([0, 2, 0], dtype = np.uint8) #sweet spot 0 2 0
    upper_white = np.array([255, 255, 255], dtype = np.uint8)

    # turn everything black just not the white color
    image_temp = cv2.inRange(image_temp, lower_white, upper_white)

    # cv2.imwrite("test_debris.png", image_temp)
    
    # get the circles with hough circle detection
    circles = cv2.HoughCircles(image_temp, cv2.HOUGH_GRADIENT,
        dp=1.55,      # accumulator resolution (size of the image / 2)
        minDist=20,   # minimum distance between two circles
        param1=50,    # Canny threshold
        param2=20,    # minimum number of votes
        minRadius=20,  # minimum radius
        maxRadius=40, # maximum radius
    )

    # get biggest circle
    maxRadius = 0
    pointX = 0
    pointY = 0

    # if detected circle
    if circles is not None:

        # go true each  detected circle
        for i in circles[0, :]:
            #get center coordinates and radius
            center = (i[0], i[1])
            radius = i[2]
            # get biggest circle between 10 and 30 pixel radius
            if (radius > maxRadius):
                maxRadius = radius
                pointX = i[0]
                pointY = i[1]
        
        print('i found the debris with raduis =', maxRadius , 'at x', pointX, 'y ', pointY)
        
        # draw circle in black and return image 
        # img_circ = cv2.circle(image, center, int(maxRadius), (0, 255, 0), -1)
        
        # get the width of the image
        width = image.shape[1]
        
        # get distance from the debris to middle of image
        distance_center = 0.005* (pointX - (width/2))
        print("the distance to the center ", distance_center)

        #get angle to the image
        sin_angle = distance_center / 12.0

        if(sin_angle > 1):
            sin_angle = 1
        if(sin_angle < -1):
            sin_angle = -1

        print("the sin anngle is", sin_angle)

        return math.degrees(math.asin(sin_angle)), distance_center
    else:
        return 555, 555


# take a picture    
def DoAPicture(camera):

    camera.start_preview()
    time.sleep(0.1)
    camera.capture('image_test.jpg')
    camera.stop_preview()

def telemetry(angle_to_debris, distance_to_debris, angle_star_cam):
    # build the strings for telemetry
    str_debris_ang = "dang," + str(angle_to_debris) + ","
    str_debris_dis = "dist," + str(distance_to_debris) + ","
    str_star_camera = "scam," + str(angle_star_cam)

    # build the checksum of the array
    checksum = len(str_debris_ang + str_debris_dis + str_star_camera)
    str_checksum = ",x," + str(checksum + len(str(checksum)) + 3)

    # encode and publish telemtry
    telemetry = (str_debris_ang + str_debris_dis + str_star_camera + str_checksum).encode('utf-8')     # -> checksum length dang,60.4312,dist,10.2834,scam,20 
    ser.write(telemetry)
    print(telemetry)

#to read telecomands -> mode,3,x,11 ->  1: Debri, 2: Star Tracker, 3: Idle
def telecomand():

    # Read 11 bytes from serial port to data
    data_read = ser.readline()                                     
    print("Received message via UART:", data_read)
    
    checksum = 0
    bool_faound_x = False
    # check the testsum
    for element in data_read:
        if element == 'x':   
            bool_faound_x = True
            continue
        if bool_faound_x == True and element != ',':
            print(element)
            checksum *= 10
            checksum += int(element)
    
    print("checksum is:", checksum)
    
    cmd = 0

    if checksum == len(data_read) and checksum != 0:
        cmd = int(data_read[5])

    return cmd



if __name__ == '__main__':
    # init serial port
    ser = serial.Serial('/dev/serial0', baudrate=115200, timeout = 0)
    
    # camera = PiCamera()
        
    while True:
        
        # # take a picture
        # DoAPicture(camera)

        # # path to images
        # pathToImageToCompute = 'image_test.jpg'
        # image = cv2.imread(pathToImageToCompute, cv2.IMREAD_COLOR)

        # # get the circle angle   
        # angle_to_debris, distance_to_debris = get_circle_Hough(image)
        
        # # send telemtry
        # telemetry(angle_to_debris, distance_to_debris, 10)

        # read telecomand
        cmd = telecomand()
        print("command is:", cmd)

        time.sleep(2)

    ser.close()
    camera.close()