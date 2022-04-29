//
// Created by Khemraj Emrith on 25/11/2019.
//
//

#ifndef FILEPROCESSING_FILEOPERATIONS_H
#define FILEPROCESSING_FILEOPERATIONS_H

//header files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

//defines
#define MYPATH  "/home/alex/CLionProjects/CourseWork/"
#define MotThresh 40  //Assuming that the range of motion is {0 .. 100}, Turn on Lights if Motion greater than 40
#define TempThresh 15 // Turn on Heater if internal temperature is less than 15
#define HumThresh 65  // Turn on Extractor if Humidity is greater than 65
#define HrsPerDay 24   // constant for number of hours per day

#define Heating 300 //Energy Consumption per hr KJ for Heating
#define Lights 100 //Energy Consumption per hr KJ for Lights
#define Ventilator 250 //Energy Consumption per hr KJ for Ventilator

typedef struct {
    char name;
    #define HrsPerDay 24   // constant for number of hours per day
    float HumiArr[HrsPerDay];   //24 values of humidity  -  one every hour
    float TempArr[HrsPerDay][2]; //24  pairs of values for temperature  - (external, internal)
    float MotionArr[HrsPerDay][6]; // 24 sets of values for motion sensors ( 6 values every hour)
    char TimeDat[HrsPerDay][25]; // 24 values of date & time
}Sensor_data;

//global Variables
FILE *ptr;
FILE *Faulty;

Sensor_data Data[3];

const char *Devices[3][24][3];
float Variance[3][4];
//float EnergyConsumptionPerDay;

//prototypes
void ReadDataFile(char *location, Sensor_data *x);
float CalculateVariance(float data[], int ArrSize);
void DisplaySensorData(Sensor_data x);

void ControlDeviceState( int x, int Room, const char *State[]);
void GetVariance( int z, float v[]);
void WriteStateFile (int Room, const char *State[24][3]);
void EnergyConsumption();
void DisplayStateofDeviceatSpecificTime(int time, const char *Dev[24][3] ,int Device, int Room );
void CheckForFault ();
void PrintFault (float v[4]);
void Menu();

#endif //FILEPROCESSING_FILEOPERATIONS_H
