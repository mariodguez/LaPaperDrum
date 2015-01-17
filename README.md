# LaPaperDrum
Aquí almaceno el código para transformar la Touch Board de Bare Conductive en una Drum para GarageBand

Bare Conductive Touch and Proximity USB MIDI interface code
This is an extension of the simple touch-to-USB-MIDI code for the Bare Conductive Touch Board which adds generic support for both touch buttons and proximity sensors.

Using this you can create either a touch or proximity object for each electrode and map it to either a keypress or slider output. This is all documented by example in the comments.

Requirements

Arduino 1.5.6 or later

Arduino's instructions for installing libraries are available here

Bare Conductive MPR121 library (zip file download here)

ARcore Arduino Hardware Plugin (zip file download here) - instructions on installing this hardware plugin folder are available here.

Install

Close the Arduino IDE if you have it open.
Download the .zip or clone the repository to your local machine - if downloading the .zip, extract the contents somewhere that suits you.
Take the Midi_interface_generic folder and move it to Arduino Sketchbook Folder. This will be different for each operating system:

Windows

Libraries\Documents\Arduino

or

My Documents\Arduino

Mac

Documents/Arduino

Linux (Ubuntu)

Home/Arduino

If this folder does not exist, create it first.

Reopen the Arduino IDE - you should now be able to open the sketch in the File -> Sketchbook menu.
