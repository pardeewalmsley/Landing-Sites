# Landing Sites
 
## Overview

*Landing Sites* is a live-streamed “clock”, intended for display in places of transit where people may pass by but not remain for long periods, such as train stations, airports, public squares, or the lobbies of public buildings. Built within the simulation engine Microsoft Flight Simulator and streamed via twitch.tv, the work may be shown on screens of various sizes, and for variable durations - in theory constantly (in practice depending on multiple factors, not least computing resources).   
</br>
The camera of the clock’s face cuts continuously between a series of sites around the planet in real-time, alongside displaying a number of variables relating to the location of the shot, the local time at each site, and CPU/GPU performance of the local machine on which the simulation is running. Each of the sites - the so-called “Landing Sites” of the work’s title - lies on a standard meridian, and therefore aligns with the boundary of one of the earth’s time zones. The camera is able to jump between all these sites thanks to the 3D “digital twin” of the earth within MSFS, which in the game serves as the backdrop for the flying experience. MSFS also displays live weather conditions at each site based on API data from METAR weather stations around the world. The “clock” cuts to a new scene once per minute, and so completes a cycle of the earth in 24 minutes.  
</br>
At each of the sites is placed a sculpture - at times clearly visible in the frame, at times receding into the landscape. Each sculpture takes as its base a 3D scan of a natural object (rocks and trees) downloaded from the Quixel Megascan library that is subsequently manipulated using boolean operations with primitive shapes (cubes, spheres, cylinders) within Blender.  
</br>
The live stream (when it is active) as well as recordings of streams from the past 7 days, can be found on [my twitch.tv channel](https://www.twitch.tv/alex_walmsley).

![LandingSites_Still](https://github.com/user-attachments/assets/d2fcf13e-ed05-4518-956f-613487f7cb47)

## Design

The software stack diagram below illustrates the main components of the work:  
</br>
*MSFS Engine*: The engine itself, built by Microsoft and Asobo Studios, which generates the graphics and physics in real-time. In addition, the engine draws in data from APIs in order to generate accurate real-time air and water traffic, as well as weather.

*Free-floating camera*: A custom virtual camera system for the MSFS engine that allows a camera to be free-floating, rather than always attached to an aircraft (the default in the simulator).

*Shot list*: A csv file containing the metadata for each camera shot in the film. The shot list contains the latitude, longitude and altitude for each shot, as well as the pitch, roll, and rotation of the camera.

*Montage controller*: A custom C++ script that communicates with the simulator in order to take control of the virtual camera and make it cut every 60 seconds to any location on the shot list (see Fig. 6).

*Overlay*: A simple interface using HTML and CSS that displays a short text string as well as the latitude/longitude and local time of the location where the camera has been placed in the simulation (see Fig. 6).

*Sculptures*: A series of digital 3D sculptures, installed at different locations on the virtual Earth. They are then augmented through dynamic elements that react to real-time weather and lighting conditions.

![Landing Sites Stack Diagram](https://github.com/pardeewalmsley/Landing-Sites/blob/main/Fig5_Software_Stack_Diagram.png)

## Installation and running requirements

*System requirements*: Those required to run Microsoft Flight Simulator 2020.  

*Software requirements*:  
 1. [Microsoft Flight Simulator 2020](https://store.steampowered.com/app/1250410/Microsoft_Flight_Simulator_40th_Anniversary_Edition/)
 2. [Visual Studio 2019](https://visualstudio.microsoft.com/de/vs/older-downloads/)
 3. Montage Controller Mod + Shot List (current repo)
 4. ["Planetary Camera" mod](https://github.com/pardeewalmsley/Planetary-Camera/tree/main)
 5. 25 Landing Sites sculptures
 6. [Flo Pro](https://parallel42.com/products/flow-pro?srsltid=AfmBOop1qH8QfSUyRVL0LMlCY8DrzdT2A-HieRiTgn3Qff3bmUMrgrth)
 7. [Flo Pro Interface widget](https://github.com/pardeewalmsley/Landing-Sites-Interface)
 8. [OBS Studio](https://obsproject.com/)  

## Instructions

*Setup*: 
 1. Install Microsoft Flight Simulator 2020
 2. Clone/Download current repo and open in Visual Studio 2019
 3. Build project in VS2019
 4. Download "Planetary Camera" mod and put in community folder (Hint: [How to find community folder](https://helpdesk.aerosoft.com/hc/en-gb/articles/5023507568925-How-to-locate-the-Community-folder-in-Microsoft-Flight-Simulator))
 5. Download Landing Sites sculptures and put in community folder
 6. Create a new widget in Flo Pro with widget code
 7. Set up stream with OBS Studio

*Run*:
 1. Start MSFS 2020
 2. Select "World Map"
 3. Select "Planetary Camera" from list of aircraft
 4. Select any airport in the world and start from one of the gates (i.e. not on the runway)
 5. Once the simulation is running, switch on Developer Mode
 6. Turn on the debug overlay from Debug > Display FPS
 7. Turn on Flo Pro widget by pressing Tab and selecting "Realtime Film Overlay"
 8. Run SetData.exe (/run from within VS2019)
 9. In OBS Studio, press "Start Streaming"

*Settings for MSFS 2020*:  
 1. 

## Documentation

*Links*:
 - [Stream](https://www.twitch.tv/alex_walmsley) on twitch.tv
 - [Project website](https://www.alexanderwalmsley.co.uk/landing-sites-2025)
 - [Article](https://classof24.xcoax.org/paper09.html) about background and design of project
