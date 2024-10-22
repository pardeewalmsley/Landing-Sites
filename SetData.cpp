// Copyright (c) Asobo Studio, All rights reserved. www.asobostudio.com
//------------------------------------------------------------------------------
//
//  Landing Sites Montage Script
// 
//	Description:
//				This script communicates between a shot list and the MSFS engine.
//				Every 60 seconds, the camera cuts to a new location on the shot list.
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

#include "SimConnect.h"

int     quit = 0;
HANDLE  hSimConnect = NULL;

static enum GROUP_ID {
    GROUP_6,
};

static enum INPUT_ID {
    INPUT_6,
};

static enum EVENT_ID{
    EVENT_SIM_START,
    EVENT_SLEW_ON,
    EVENT_OVERLAYMENU,
    EVENT_6,
    EVENT_1S,
};

static enum DATA_DEFINE_ID {
    DEFINITION_6,
    DEFINITION_CALLSIGN,
};

static enum DATA_REQUEST_ID {
    REQUEST_6,
    REQUEST_CALLSIGN,
};

typedef struct ObservationPoint
{
    string Name {};
    double Altitude {};
    double Latitude {};
    double Longitude{};
    float Pitch {};
    float Bank {};
    float Heading {};
    int OnGround {};
    int Airspeed {};
};

vector<ObservationPoint> listOfPoints;
int numberOfLocations;
int newLocation = -1;

int counter = 1;
int timer = 15;

vector<ObservationPoint> parseCSV(const string& filename, int& numberOfLocations) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return listOfPoints;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        ObservationPoint data;

        // Read Name
        getline(ss, data.Name, ',');

        // Read Altitude
        getline(ss, token, ',');
        data.Altitude = stoi(token);

        // Read Latitude
        getline(ss, token, ',');
        data.Latitude = stod(token);

        // Read Longitude
        getline(ss, token, ',');
        data.Longitude = stod(token);

        // Read Pitch
        getline(ss, token, ',');
        data.Pitch = stod(token);

        // Read Bank
        getline(ss, token, ',');
        data.Bank = stod(token);

        // Read Heading
        getline(ss, token, ',');
        data.Heading = stod(token);

        // Read OnGround
        getline(ss, token, ',');
        data.OnGround = stod(token);

        // Read Airspeed
        getline(ss, token, ',');
        data.Airspeed = stod(token);

        // Push data into array
        listOfPoints.push_back(data);
    }

    numberOfLocations = listOfPoints.size(); // Calculate the number of items in the CSV
    file.close();
    return listOfPoints;
}

int ParseFile(void)
{
    string filename = "ShotList.txt"; // Change this to your file name
    vector<ObservationPoint> dataArray = parseCSV(filename, numberOfLocations);

    cout << "Number of items in shot list: " << numberOfLocations << endl;

    // Print parsed data
    for (const auto& data : dataArray) {
        cout << data.Name << ", " << data.Altitude << ", " << data.Latitude << ", " << data.Longitude << ", " << data.Pitch << ", " << data.Bank << ", " << data.Heading << ", " << data.OnGround << ", " << data.Airspeed << endl;
    }

    return 0;
}

void CALLBACK MyDispatchProcSD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
    HRESULT hr;
    srand(time(NULL));

    switch(pData->dwID)
    {

        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

            switch(evt->uEventID)
            {
                //case EVENT_6:
                case EVENT_1S:
					{   
                        counter++;

                        if (counter % timer == 0) 
                        {
                            // Random int
                            //newLocation = rand() % numberOfLocations;

                            // Cycle through locations
                            if (newLocation != numberOfLocations)
                            {
                                newLocation++;
                            }
                            else
                            {
                                newLocation = 0;
                            }

                            SIMCONNECT_DATA_INITPOSITION Init;
                            Init.Latitude = listOfPoints[newLocation].Latitude;
                            Init.Longitude = listOfPoints[newLocation].Longitude;
                            Init.Altitude = listOfPoints[newLocation].Altitude;
                            Init.Pitch = listOfPoints[newLocation].Pitch;
                            Init.Bank = 0.0;
                            Init.Heading = listOfPoints[newLocation].Heading;
                            Init.OnGround = 0;
                            Init.Airspeed = 0;
                            hr = SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_6, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(Init), &Init);

                            printf("\nNew Location: %s", listOfPoints[newLocation].Name.c_str());
                        }
                        
                    }
                    break;

                default:
                    break;
            }
            break;
        }

        case SIMCONNECT_RECV_ID_QUIT:
        {
            quit = 1;
            break;
        }

        default:
            printf("\nReceived:%d",pData->dwID);
            break;
    }
}

void testDataSet()
{
    HRESULT hr;

    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Set Data", NULL, 0, 0, 0)))
    {
        printf("\nConnected to Flight Simulator!");   
        
        // Set up a data definition for positioning data
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_6, "Initial Position", NULL, SIMCONNECT_DATATYPE_INITPOSITION);

        // Request a simulation start event
        hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");

        // Create a custom event
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_6, "My.z");

        // Link the custom event to some keyboard keys, and turn the input event off
        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT_6, "z", EVENT_6);
        hr = SimConnect_SetInputGroupState(hSimConnect, INPUT_6, SIMCONNECT_STATE_ON);

        // Subscribe to the four second timer

        hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_1S, "1sec");

        // Sign up for notifications for EVENT_6
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_6, EVENT_6);

        // Set default camera
        // Set no UI in game

        // Set slew
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_SLEW_ON, "SLEW_ON");
        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_6, EVENT_SLEW_ON);
        hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP_6, SIMCONNECT_GROUP_PRIORITY_HIGHEST);

        SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_SLEW_ON, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
        
        while( 0 == quit )
        {
            SimConnect_CallDispatch(hSimConnect, MyDispatchProcSD, NULL);
            Sleep(1);
        } 

        hr = SimConnect_Close(hSimConnect);
    }
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
    ParseFile();
    testDataSet();

	return 0;
}





