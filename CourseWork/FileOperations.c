/***********************************************************************/
//Programme Banner: FileOperations.c
// Created by Khemraj Emrith on 25/11/2019.
//Description: This file contains the description/definitions of all file
//                                          operations used by the system
// version 1.0 - functions to read files and Display sensor data added
// version 2.0 - function to implement variance added

// Used and Adapted by Alex Goodacre on 25/01/2021
//Version 3.0 - added function to get the variance,
//Version 4.0 - added a function to check for faults and to control the state of devices
//Version 5.0 - added a function to write the states to a file, and print the faults, and print Device states at a specific time
//Version 6.0 - added a function to work out energy consumption and write it too a file
//Version 7.0 - added the final verison of the menu
/***********************************************************************/

#include "FileOperations.h"

/************************************/
// Function CaculateVariance
// Author   K. Emrith
// Date     15/12/2020
// Input    Array of float of size ArrSize
// Output   Variance of array values
// Version  1.0

/***********************************/
float CalculateVariance(float data[], int ArrSize){
    float var, mean, sum=0.0f, sdiff=0.0f;
    int i, j;

    //Compute average of elements
    for (i=0; i<ArrSize; i++){
        sum += data[i];
    }
    mean = (float) sum/ (float) ArrSize;

    //compute sum of squared differences with mean
    for (j = 0; j < ArrSize; j++) {
        sdiff += ((data[j] - mean) * (data[j] - mean));
    }

    return ( (float)sdiff / (float)ArrSize);
}


/********************************************************************/
//Function GetVariance
//Author A.Goodacre
//Date 26/1/2021
//Version 2.0
//Description: This Function Stores the variance of sensors by using calculate variance function to get the variance,
// the put it not the inputted array
/********************************************************************/

void GetVariance( int z, float v[])
{
    //Variables used in the function, Temp is used to input the data arrays into calculate variance
    int x, F = 1, i, y, a = 0;
    float Temp[HrsPerDay];
    float Humi = 0, Temp2;

    //Puts the inside temperature data into a 1d array for input
    for (i = 0; i < HrsPerDay; i++)
    {
        Temp[i] = Data[z].TempArr[i][0];
    }

    v[0] = CalculateVariance( Temp, HrsPerDay); // stores the variance into the array

    //Puts the Outside temperature data into a 1d array for input
    for (i = 0; i < HrsPerDay; i++)
    {
        Temp[i] = Data[z].TempArr[i][1];
    }
    v[1] = CalculateVariance( Temp, HrsPerDay);// stores the variance into the array


    //v[2] = CalculateVariance( Data[z].HumiArr, HrsPerDay); orginal

    // since Humidity sensors faults are decided by the mean, adding the data into 1 variable to then divide by HrsPerDay to get the mean
    for (i = 0; i < HrsPerDay; i++)
    {
        Humi += Data[z].HumiArr[i];
    }
    v[2] = Humi / HrsPerDay; // stores the variance into the array

    //First puts the 6 motion sensor values into temp2 and then is divided by 6 to gets the mean for each hour,
    // then put into the temp array so it can ne used by calculate
    for (i = 0; i < HrsPerDay; i++)
    {
        for (y = 0; y < 6; y++ )
        {
            Temp2 = Temp2 + Data[z].MotionArr[i][y];
        }
        Temp[i] = Temp2/6;
        Temp2 = 0; //resets temp2 so it doesn't stack
    }
    v[3] = CalculateVariance( Temp, HrsPerDay);
}

/********************************************************************/
//Function CheckForFault
//Author A.Goodacre
//Date 27/1/2021
//Version 2.0
//Desctiption: This Program checks for Faults using the Variance Gathered Earlier
/********************************************************************/

void CheckForFault ()
{
    // variables used in function, src is used for file root, and room is used to tell the function which part of the array to call
    int Room;
    char src[100]={};
    strcat( strcat(src, MYPATH), "Faulty.txt");
    Faulty = fopen(src,"a");
    //Checks that the file is loaded correctly
    if(Faulty == NULL)
    {
        printf("Error!\n");
        exit(1);
    }
    else
        printf("Success!\n");
    //Writes which room the sensors are in, and which index to use in the array
    for (Room = 0; Room < 3; Room++) {
        //prints the room the sensor is in
        switch (Room) {
            case 0 :
                fprintf(Faulty, "\nLivingRoom\t");
                break;
            case 1 :
                fprintf(Faulty, "\nBedroom\t");
                break;
            case 2 :
                fprintf(Faulty, "\nKitchen\t");
                break;

        }
        //Checks if Inside Temperature is Faulty
        if (Variance[Room][0] < 3.0) {
            //Sensor Faulty
            fprintf(Faulty, "Inside Temperature Sensor is Faulty\t");

        }
        // Checks if Outside Temperatre is Faulty
        if (Variance[Room][1] < 3.0) {
            //Sensor Faulty
            fprintf(Faulty, "Outside Temperature Sensor is Faulty\t");
        }
        // Checks if Humidity Sensor is Faulty
        if (Variance[Room][2] < 5.0) {
            //Sensor Faulty
            fprintf(Faulty, "Humidity Sensor is Faulty\t");
        }
        //Checks if the motions sensor is Faulty
        if (Variance[Room][3] < 2.0) {
            //Sensor Faulty
            fprintf(Faulty, "Motion Sensor is Faulty\t");
        }
    }
    fclose(Faulty); // closes the file
}

/********************************************************************/
//Function PrintFault
//Author A.Goodacre
//Date 28/1/2021
//Version 1.0
//Desctiption: This Program Prints for Faults using the Variance Gathered Earlier
/********************************************************************/

void PrintFault (float v[4])
{
    //printf("%f %f %f %f\n", v[0], v[1], v[2], v[3]); debug line to check the function corrctly used the variance values
    //Checks if Inside Temperature is Faulty
    if (v[0] < 3.0 )
    {
        //Sensor Faulty
        printf( "Inside Temperature Sensor is Faulty\n");

    }
    // Checks if Outside Temperatre is Faulty
    if (v[1] < 3.0 )
    {
        //Sensor Faulty
        printf("Outside Temperature Sensor is Faulty\n");
    }
    // Checks if Humidity Sensor is Faulty
    if (v[2] < 5.0 )
    {
        //Sensor Faulty
        printf("Humidity Sensor is Faulty\n");
    }
    //Checks if the motions sensor is Faulty
    if (v[3] < 2.0 )
    {
        //Sensor Faulty
        printf("Motion Sensor is Faulty\n");
    }

}


/********************************************************************/
//Function ControlDeviceState
//Author A.Goodacre
//Date 28/1/2021
//Version 1.5
//Description: Used to decide if a device is on or off, and stores the value into a pointer array for use later
/********************************************************************/

void ControlDeviceState( int x, int Room, const char *State[])
{
    //variables used in the function
    int i;
    float sum = 0 ;

    //Determines if the Heater is on or off using TempThresh
    if (Data[Room].TempArr[x][0] < TempThresh)
    {
        State[0] = "ON";
    }
    else
    {
        State[0] = "OFF";
    }
    //Determines if the Humidity extractor fan is off or on using the HumThresh
    if (Data[Room].HumiArr[0] > HumThresh)
    {
        State[1] = "ON";
    }
    else
    {
        State[1] = "OFF";
    }
    //Adds up the motion sensor values for the hour and gets the mean to check if its on or off
    for (i = 0; i < 6; i++)
    {
        sum = sum + Data[Room].MotionArr[x][i];
    }
    sum = sum/6;
    //Determines if the Lights is off or on using the Motion Thresh
    if (sum > MotThresh)
    {
        State[2] = "ON";
    }
    else
    {
        State[2] = "OFF";
    }

}


/********************************************************************/
//Function DisplaySensorData
//Author:K.Emrith
//Date: 25/11/2019
//Version : 1.0
/********************************************************************/

void DisplaySensorData(Sensor_data x) {
int k, t=0;

while (t<24) {
    //print
    printf("\n");
    printf("\t time=%s", x.TimeDat[t]); //myDat[t].time);
    printf("\t iTemp=%f", x.TempArr[t][1]);
    printf("\t Hum=%f", x.HumiArr[t]);
    printf("\t eTemp=%f", x.TempArr[t][0]);
    for (k = 0; k < 6; k++)
        printf(" Mot:%f\t", x.MotionArr[t][k]);

    t++;
    }
}

/********************************************************************/
//Function DisplayStateofDeviceatSpecificTime
//Author:A.Goodacre
//Date: 30/01/2021
//Version : 1.5
//Description: Displays the state of a device in a room in at specific time, Made so that can be used easily in a for loop
/********************************************************************/

void DisplayStateofDeviceatSpecificTime(int time, const char *Dev[24][3] ,int Device , int Room)
{
    //Shows the device based on the value inputted, using a character pointer array helped to make this streamlined
    switch (Device)
    {
        case 0 : printf("The Heater is %s at %s \n", Dev[time][0], Data[Room].TimeDat[time]);
            break;
        case 1 : printf("The Ventilator System is %s at %s \n", Dev[time][1], Data[Room].TimeDat[time]);
            break;
        case 2 : printf("The Lights are %s at %s \n", Dev[time][2], Data[Room].TimeDat[time]);
            break;
    }

}

/********************************************************************/
//Function ReadDataFile
//Author:K.Emrith
//Date: 25/11/2019
//Version : 1.0
//Description: Function allows sensor data to be read from different location
/********************************************************************/

void ReadDataFile(char *location, Sensor_data *x) {
    int t=0, k;
    char src[100]={};
    x->name = *location;
    strcat( strcat(src, MYPATH), location);
    //open file for reading
    ptr = fopen(src,"r");

    if(ptr == NULL)
    {
        printf("Error!\n");
        exit(1);
    }
    else
        printf("Success!\n");

    while ((!feof(ptr)) && (t<24)) {
        //read time
        fscanf(ptr, "%s", x->TimeDat[t]);
        //read temperatures
        fscanf(ptr, "%f%f", &x->TempArr[t][0], &x->TempArr[t][1]);
        //read humidity
        fscanf(ptr, "%f",  &x->HumiArr[t]);
        //read motion data
        for ( k=0; k<6; k++)
            fscanf(ptr,"%f",  &x->MotionArr[t][k]);

        t++;

    }
    fclose(ptr);
}

/********************************************************************/
//Function WriteStateFile
//Author:A.Goodacre
//Date: 30/01/2021
//Version : 1.0
//Description:Writes the State of each device into a file
/********************************************************************/

void WriteStateFile (int Room, const char *State[24][3])
{
    //variables used in function
    int i;
    char src[100]={};
    if(ptr == NULL)
    {
        printf("Error!\n");
        exit(1);
    }
    else
        printf("Success!\n");

    //used to open which file we will be storing the data in, based on the Room variable
    if (Room == 0)
    {
        char file[] = "LivingRoomDevices.txt";
        strcat( strcat(src, MYPATH), file);
        ptr = fopen(src,"w");
    }
    if (Room == 1)
    {
        char file[] = "BedroomDevices.txt";
        strcat( strcat(src, MYPATH), file);
        ptr = fopen(src,"w");
    }
    if (Room == 2)
    {
        char file[] = "KitchenDevices.txt";
        strcat( strcat(src, MYPATH), file);
        ptr = fopen(src,"w");
    }
    //writes if the device status at a certain time
    for (i = 0 ; i < HrsPerDay; i++)
    {
        fprintf(ptr, "%s\t", Data[Room].TimeDat[i]); //Prints the time

        fprintf(ptr, "%s\t", State[i][0]); // Prints the Heater status

        fprintf(ptr, "%s\t", State[i][1]); // Prints the Ventilator System status

        fprintf(ptr, "%s\t", State[i][2]); // Prints the Lights are on
        fprintf(ptr, "\n");  // Ensures the file has a format and that the times are on seperate line
    }
    fclose(ptr);
}

/********************************************************************/
//Function EnergyConsumption
//Author:A.Goodacre
//Date: 30/01/2021
//Version : 2.0
//Description: Gets the values to work out the energy consumption per day then stores it to a file
/********************************************************************/

void EnergyConsumption()
{
    //Code to get How many Hours each device in each room is on
    int a, b, c; // Variables to store how long each device is on for
    float x,y,z, Energy;
    c = 0;
    //Does a for loop to get how many hours each device is on for in all the rooms
    for (a = 0; a < HrsPerDay ; a++) // goes through each hour in the day
    {
        for (c = 0; c < 3; c++) { // goes through each room

            for (b = 0; b < 3; b++) //goes through each Device
            {
                if ((strcmp(Devices[c][a][b], "ON") == 0))
                {
                    switch (b)
                    {
                        case 0: // for Heater
                            x = x + 1;
                            break;
                        case 1: // for Ventilator
                            y = y + 1;
                            break;
                        case 2: // for Lights
                            z = z + 1;
                            break;
                    }
                }

                }

        }
    }
    Energy = x*Heating + y*Ventilator + z*Lights; // Energy Equation

    //Code for writing the energy consumption to a file
    char src[100]={};
    strcat( strcat(src, MYPATH), "Energy.txt");
    ptr = fopen(src,"w");
    //Checks that the file processed worked
    if(ptr == NULL)
    {
        printf("Error!\n");
        exit(1);
    }
    else
        printf("Success!\n");
    //writes to file then closes the file
    fprintf(ptr, "%f\n", Energy);
    fclose(ptr);
}



/********************************************************************/
//Function Menu
//Author:A.Goodacre
//Date: 31/01/2021
//Version : 2.0
//Description: This Function is the menu, it allows the user to view the data they want
/********************************************************************/


void Menu()
{
    // variablse used
    char response[16];//used to store the users main response
    int t = 0, s, i, x = 0, a, b; // x and t are used for whiles, s,a,b store responses that are used to then put into the print functions,//
    // i is used in for loops

    //A while loop will keep the program running, unless quit is entered, breaking the condition
    while ( t != 1)
    {

        //Asked the user to input a response, [] indicate the accepted inputs
        printf("\nChoose what you would like to do\n"
               "Enter [Sensor] Display Sensor Data \n"
               "Enter [Device] to Display Device State \n"
               "Enter [Fault] to Display Faults\n"
               "Enter [Quit] to Exit program\n");

        scanf("%s", response);
        //By using the Strcmp function, it checks if the responses are correct
        if (strcmp(response, "Device") == 0 )
        {
            // prints the options for the devices to be viewed
            printf("How would you want to view the Devices\n"
                   "Individually [0]\n"
                   "Per Room[1]\n"
                   "Per Device[2]\n"
                   "At a specific Time[3]\n");
            //checks the input is valid
            while (x != 1)
            {
                scanf("%d", &s);
                if (s >= 0 && s < 4)
                    break;
                printf("Error, try again\n");
            }
            x = 0;
            if ( s == 0)
            {
                //Gathers the data for the individual option
                printf("Enter a Room, Device and the Hour you want to check\n"
                       "Living Room [0]\n"
                       "Bedroom [1]\n"
                       "Kitchen [2]\n");
                while (x != 1) {
                    scanf("%d", &s);
                    if (s >= 0 && s < 3)
                        break;
                    printf("Error, try again\n");
                }
                x = 0;
                printf("Heater[0]\n"
                       "Ventilator[1]\n"
                       "Lights[2]\n");
                while (x != 1) {
                    scanf("%d", &a);
                    if (a >= 0 && a < 3)
                        break;
                    printf("Error, try again\n");
                }
                x = 0;
                printf("Enter time of day, by the hour in 24hr clock\n");
                while (x != 1)
                // checks that an hour is entered
                {
                    scanf("%d", &b);
                    if (b >= 0 && b < 24)
                        break;
                    printf("Error, try again\n");}
                // uses the data gathered, and inputs it into the print function
                DisplayStateofDeviceatSpecificTime(b,Devices[s], a, s);
                x = 0;
            }
            else if (s == 1)
            {
                // gathers data needed for the room option
                printf("Which Room do you want to see\n"
                       "Living Room [0]\n"
                       "Bedroom [1]\n"
                       "Kitchen [2]\n");
                while (x != 1)
                {
                    scanf("%d", &s);
                    if (s >= 0 && s < 3)
                        break;
                    printf("Error, try again\n");
                }
                x = 0;
                printf("Enter time of day, by the hour in 24hr clock\n");
                while (x != 1)
                {
                    scanf("%d", &b);
                    if (b >= 0 && b < 24)
                        break;
                    printf("Error, try again\n");
                }
                // the for loop runs so that each device in the room is displayed
                for (i = 0; i < 3; i++)
                {
                    DisplayStateofDeviceatSpecificTime(b, Devices[s], i, s);
                }
                x = 0;

            }
            else if (s == 2)
            {
                // Gets the data needed for a device option
                printf("Which Device do you want to see\n"
                       "Heater[0]\n"
                       "Ventilator[1]\n"
                       "Lights[2]\n");
                while (x != 1) {
                    scanf("%d", &s);
                    if (s >= 0 && s < 3)
                        break;
                    printf("Error, try again\n");
                }
                x = 0;
                printf("Enter time of day, by the hour in 24hr clock\n");
                while (x != 1) {
                    scanf("%d", &b);
                    if (b >= 0 && b < 24)
                        break;
                    printf("Error, try again\n");
                }
                for (i = 0; i < 3; i++)
                {
                    //Goes through each room and display the status the desired device type
                    DisplayStateofDeviceatSpecificTime(b, Devices[i], s, i);
                }
                x = 0;
            }
            else if (s == 3)
            {
                //asks for the time wannt for the final option
                printf("Enter time of day, by the hour in 24hr clock\n");
                while (x != 1) {
                    scanf("%d", &b);
                    if (b >= 0 && b < 24)
                        break;
                    printf("Error, try again\n");
                }
                //Does a for loop for each room
                for (i = 0; i < 3; i++)
                {
                    switch (i)
                    {
                        case 0 : printf("Living Room\n");
                            break;
                        case 1 : printf("Bedroom\n");
                            break;
                        case 2 : printf("Kitchen\n");
                            break;
                    }
                    for(a = 0; a < 3; a++) // a loop that goes through each Device in a room
                        DisplayStateofDeviceatSpecificTime(b, Devices[i], a, i);
                }
                x = 0;
            }
        }
        if (strcmp(response, "Sensor") == 0 )
        {
            //Uses the displaySensorData function, to display the data, based on the user input
            printf("Which Room do you want to see\n"
                   "Living Room [0]\n"
                   "Bedroom [1]\n"
                   "Kitchen [2]\n"
                   "");
            while (x != 1)
            {
                scanf("%d", &s);
                if (s >= 0 && s < 3)
                    break;
                printf("Error, try again");
            }
            DisplaySensorData(Data[s]);

        }
        if (strcmp(response, "Fault") == 0 )
        {
            //uses the print fault function to show all the faults in each room
            printf("the Living Room Sensors\n");
            PrintFault(Variance[0]);
            printf("the Bedroom Sensors\n");
            PrintFault(Variance[1]);
            printf("the Kitchen Sensors\n");
            PrintFault(Variance[2]);
        }
        if (strcmp(response, "Quit") == 0 )
        {
            //Exits the program
            t = 1;
            break;
        }
    }
}