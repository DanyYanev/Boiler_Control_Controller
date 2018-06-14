# Boiler_Control_Controller
## EVERYTHING IS ON BRANCH ESP


## Assignment 
 
Developing an easy-to-use device for controlling complicated water-heating systems through relays, giving the end user the ability to easily manage the heating system by the provided local touchscreen display, or remotely done so by the developed mobile Android application.
 

## General Characteristics of the problem

 
The product allows the end-user to easily and quickly control the system while being very efficient on resources such as time, electricity and water and taking fault proof into consideration.
It’s especially useful when manging the heating system of a distanced house in an underpopulated location. Using this system frees you from the responsibility to go there and manually manage everything and allows you to quickly setup your heating system just in time before you arrive as if you were just there yesterday. 

## General Working Schema

<img src="https://github.com/DanyYanev/Boiler_Control_Controller/blob/esp/resourses/Simple%20logical%20schema.png" width="70%">

  The whole system water-heating system is controlled by an Arduino Mega that communicates with an ESP8266 Module, the GUI is a [Nextion Display](https://nextion.itead.cc/) which is getting programmed through the Nextion Editor. All pictures were designed using Adobe Photoshop and are located in [here](/resourses/pictures/).

  The Arduino Mega and the ESP module communicate through regular serial. The serialEvent interrupt function in each controller parses the messages.

  The main functionality of the ESP module is to communicate with the API via HTTP requests. While the Mega is responsible for building the logic and managing the hardware. Both devises have acknowledgement whether or not a message has been parsed successfully.
		
  Every 10 seconds the ESP module attempts to fetch information from the server via a GET request at “[server_ulr]/users/[user_token]”. Every device has a unique user-generated user token and a controller token which is built in the ESP module and is unique for every microcontroller. After a successful request the body of the request is fetched and sent to the Mega controller. The body contains a JSON specified by the API Documentation here. If the parsing is successful the Mega controller will send an acknowledgement message. 

The ESP won’t send data to be parsed if the current running configuration is the same as the one fetched from the server.

After a local change is made the Mega will prepare a new JSON with the changes.

  Every 5 seconds if a change is made to the configuration, the updated JSON will be parsed to the ESP which will then wrap it in a PUT request and send it to the server. If an update is successful an acknowledgement message will be sent to the MEGA and the JSON object will be flushed. Else the MEGA will continue to send the updates every 5 seconds.

  All required variables for the logic to be built are saved in EEPROM. In case of power loss, on startup all variables are loaded from the local non-volatile memory.

Controller code for Boiler_Control project. ELSYS-2018

App Repo - https://github.com/DanyYanev/Boiler_Control_App

Server Repo - https://github.com/DanyYanev/Boiler_Control_Server
