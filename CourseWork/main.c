#include <stdio.h>

#include "FileOperations.h"

int main() {
    int i;

    ReadDataFile("LivingRoomDemo.txt", &Data[0]);
    ReadDataFile("BedroomDemo.txt", &Data[1]);
    ReadDataFile("KitchenDemo.txt", &Data[2]);
    GetVariance(0,Variance[0]);
    GetVariance(1,Variance[1]);
    GetVariance(2,Variance[2]);

    for (i = 0 ; i < HrsPerDay ; i++)
    {
        ControlDeviceState(i, 0, Devices[0][i]);
    }

    for (i = 0 ; i < HrsPerDay ; i++)
    {
        ControlDeviceState(i, 1, Devices[1][i]);
    }

    for (i = 0 ; i < HrsPerDay ; i++)
    {
        ControlDeviceState(i, 2, Devices[2][i]);
    }
    WriteStateFile(0,Devices[0]);
    WriteStateFile(1,Devices[1]);
    WriteStateFile(2,Devices[2]);
    CheckForFault( );

    EnergyConsumption();


    Menu();
    return 0;
}
