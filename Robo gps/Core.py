#modules need for program
from math import radians, sin, cos, acos
import sys
import time
import RPi.GPIO as GPIO
##from Threading import Thread
import json


mode = GPIO.getmode

#resets pins from previous programs so no errors
GPIO.cleanup()

#pins that will connect to the motors and will control them
XrA = 37
XrB = 35
YuA = 32
YuB = 36
XlC = 40
XlD = 38
YdC = 31
YdD = 33

li = [XrA,XrB,YuA,YuB,XlC,XlD,YdC,YdD]

#setups up pins as outputs
GPIO.setmode(GPIO.BOARD)
GPIO.setup(XrA, GPIO.OUT)
GPIO.setup(XrB, GPIO.OUT)
GPIO.setup(YuA, GPIO.OUT)
GPIO.setup(YuB, GPIO.OUT)
GPIO.setup(XlC, GPIO.OUT)
GPIO.setup(XlD, GPIO.OUT)
GPIO.setup(YdC, GPIO.OUT)
GPIO.setup(YdD, GPIO.OUT)

#simplied comand
def Move(x) :
    GPIO.output(x, GPIO.HIGH)

#def for stopping
def Stop(x):
    GPIO.output(x, GPIO.LOW)
#def for moving
def goForward() :
    Move(XrA)
    Move(XlC)

def goBackward():
    Move(XrB)
    Move(XlD)

def goLeft():
     Move(YuB)
     Move(YdD) 
     
def goRight():
     Move(YuA)
     Move(YdC)


def TurnRight() :
     Move(XrA)
     Move(XlD)
     Move(YuA)
     Move(YdD)

def TurnLeft() :
     Move(XrB)
     Move(XlC)
     Move(YuB)
     Move(YdC)

#Brake
def Brake() :
    for i in li :
        Stop(i)  
"""
SS = exec('server-socket.py')
tSS = Thread(target = SS)
tSS.start()

GPS = exec('test3.py')
tGPS = Thread(target = GPS)
tGPS.start()
"""
try:
    Cur = open("file1", "r")
    DesCor = Cur.readlines()
    Cur.close()
except:
    print("oh no")
StartingLat = DesCor[0]
StartingLon = DesCor[1]
while True :
    while True:
        try:
            Des = open("file2", "r")
            DesCor = Des.read()
            DesCor = DesCor.replace("'","\"")
            ##print(DesCor)
            Des.close()
            break
        except:
            print("oh no")
    y = json.loads(DesCor)
    DesLon = y['gps']['longitude']
    DesLat = y['gps']['latitude']
    DesBearing = y['gps']['bearing']
    print(DesLon, DesLat, DesBearing)
    ##lDesCor = DesCor.strip().split("'gps'")
    
    ##for i in lDesCor :
      ##  lDesCor = lDesCor.strip().split("=")
        
        ##if 
    while True :
        try:
            Cur = open("file1", "r")
            DesCor = Cur.readlines()
            Cur.close()
            break
        except:
            print("oh no")
    CurrentLat= DesCor[0]
    CurrentLon= DesCor[1]
    CurrentBearing= DesCor[2]
    slat = radians(float(DesCorLat))
    slon = radians(float(DesCorLon))
    elat = radians(float(StartingLat))
    elon = radians(float(StartingLon))  
    dist = 6371.01 * acos(sin(slat)*sin(elat) + cos(slat)*cos(elat)*cos(slon - elon))
    #print("The distance is %.2fkm." % dist)
    slat = radians(float(DesCorLat))
    slon = radians(float(DesCorLon))
    elat = radians(float(CurrentLat))
    elon = radians(float(CurrentLon))  
    CurrentDist = 6371.01 * acos(sin(slat)*sin(elat) + cos(slat)*cos(elat)*cos(slon - elon))

    while BearingBes != CurrentBearing:
        TurnLeft()
    
    while BearingDes == CurrentBearing and dist >= CurrentDist :
        goForward()











    
