// Copyright (c) Asobo Studio, All rights reserved. www.asobostudio.com
//------------------------------------------------------------------------------
//
//  SimConnect Set Data Sample
// 
//	Description:
//				When z is pressed, the user aircraft is moved
//				to a new location
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
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

ObservationPoint listOfPoints[11] = {
    { "Tegucigalpa, Honduras", 0.0, 14.057, -87.273, 0.0, 0.0, 335.0, 1, 0 },
    { "Tin City Broadband", 0.0, 65.561, -167.924, 0.0, 0.0, 85.0, 1, 0 },
    { "Pasadena Art Center, College of Design", 0.0, 34.171, -118.185, 0.0, 0.0, 85.0, 1, 0 },
    { "Mount Denham, Jamaica", 0.0, 18.226, -77.535, 0.0, 0.0, 85.0, 1, 0 },
    { "Shemya, Alaska", 0.0, 52.731, -185.897, 0.0, 0.0, 85.0, 1, 0 },
    { "Bogoin, Central African Republic", 0.0, 5.176, 18.424, 0.0, 0.0, 85.0, 1, 0 },
    { "La Paz, Bolivia", 0.0, -16.288, -68.131, 0.0, 0.0, 85.0, 1, 0 },
    { "Dry Valley, Vanda, Antarctica", 0.0, -77.517, 161.853, 0.0, 0.0, 85.0, 1, 0 },
    { "Villa Florida, Paraguay", 0.0, -26.331, -57.331, 0.0, 0.0, 85.0, 1, 0 },
    { "Enshi, Hubei Province, China", 0.0, 30.276, 109.494, 0.0, 0.0, 85.0, 1, 0 },
    { "Alibek, Turkmenistan", 0.0, 37.93, 58.119, 0.0, 0.0, 85.0, 1, 0 },
};

int numberOfLocations = *(&listOfPoints + 1) - listOfPoints;
int newLocation = 0;

typedef struct structOverlayText 
{
    string overlay;
};

structOverlayText ot = { "Hello World" };

int counter = 1;
int timer = 30;

void CALLBACK MyDispatchProcSD(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
    HRESULT hr;
    
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
                            newLocation = rand() % numberOfLocations;

                            // Cycle through locations
                            //if (newLocation != numberOfLocations + 1)
                            //{
                            //    newLocation++;
                            //}
                            //else
                            //{
                            //    newLocation = 0;
                            //}

                            SIMCONNECT_DATA_INITPOSITION Init;
                            Init.Altitude = 0.0;
                            Init.Latitude = listOfPoints[newLocation].Latitude;
                            Init.Longitude = listOfPoints[newLocation].Longitude;
                            Init.Pitch = 0.0;
                            Init.Bank = 0.0;
                            Init.Heading = listOfPoints[newLocation].Heading;
                            Init.OnGround = 1;
                            Init.Airspeed = 0;
                            hr = SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_6, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(Init), &Init);

                            // Trying to get the text description to show on screen by passing it to the ATC AIRLINE variable, which can hold a 50-char string 
                            ot.overlay = listOfPoints[newLocation].Name;
                            hr = SimConnect_SetDataOnSimObject(hSimConnect, DEFINITION_CALLSIGN, SIMCONNECT_OBJECT_ID_USER, 0, 0, sizeof(ot), &ot);

                            printf("\n%s", ot.overlay.c_str());
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

        // Set up a data definition for callsign
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_CALLSIGN, "A:ATC AIRLINE", "string");

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

    testDataSet();

	return 0;
}





