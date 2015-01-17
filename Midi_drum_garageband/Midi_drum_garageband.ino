/*******************************************************************************

 Bare Conductive Touch Board USB MIDI interface for Drum Kit
 ------------------------------------------------------
 
 basado en el codigo de Midi_interface_generic.ino - USB MIDI touch and proximity example
 https://github.com/BareConductive/generic-usb-midi-interface

 See the comments for details and experiment for best results.
 
 Requires Arduino 1.5.6+ or greater and ARCore Hardware Cores 
 https://github.com/rkistner/arcore - remember to select 
 Bare Conductive Touch Board (arcore, iPad compatible) in the Tools -> Board menu
 
 Bare Conductive code written by Stefan Dzisiewski-Smith.
 
 This work is licensed under a Creative Commons Attribution-ShareAlike 3.0 
 Unported License (CC BY-SA 3.0) http://creativecommons.org/licenses/by-sa/3.0/
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*******************************************************************************/

#include <MPR121.h>
#include <Wire.h>
#include "Midi_object.h"
#include "Compiler_Errors.h"
MIDIEvent e;

#define numElectrodes 12
midi_object_t MIDIobjects[numElectrodes]; // create an array of MIDI objects to use (one for each electrode)

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);

  MPR121.begin(0x5C);
  MPR121.setInterruptPin(4);

  // set up electrode 0 as a touch key attached to note 35 or 36 (Bass Drum)
  MIDIobjects[0].type = MIDI_NOTE;
  MIDIobjects[0].noteNumber = 36;
  MIDIobjects[0].touchThreshold = 40;    // always make sure that the touch threshold is
  MIDIobjects[0].releaseThreshold = 20;  // larger than the release threshold

  // set up electrode 1 as a touch key attached to note 37 (Side Stick)
  MIDIobjects[1].type = MIDI_NOTE;
  MIDIobjects[1].noteNumber = 37;
  MIDIobjects[1].touchThreshold = 40;   // higher values = less sensitivity = trigger by touch, not proximity
  MIDIobjects[1].releaseThreshold = 20;  

  // set up electrode 2 as a touch key attached to note 38 or 40 (Acoustic Snare or Electric Snare)
  MIDIobjects[2].type = MIDI_NOTE;
  MIDIobjects[2].noteNumber = 38;
  MIDIobjects[2].touchThreshold = 40;
  MIDIobjects[2].releaseThreshold = 20;   

  // set up electrode 3 as a touch key attached to note 42 (Closed Hi-Hat)
  MIDIobjects[3].type = MIDI_NOTE;
  MIDIobjects[3].noteNumber = 42;
  MIDIobjects[3].touchThreshold = 40;
  MIDIobjects[3].releaseThreshold = 20;
  
  // set up electrode 4 as a touch key attached to note 41 o 43 (Low Floor Tom or High Floot Tom)
  MIDIobjects[4].type = MIDI_NOTE;
  MIDIobjects[4].noteNumber = 41;
  MIDIobjects[4].touchThreshold = 40;
  MIDIobjects[4].releaseThreshold = 20;
  
  // set up electrode 5 as a touch key attached to note 44 (Pedal Hi-Hat)
  MIDIobjects[5].type = MIDI_NOTE;
  MIDIobjects[5].noteNumber = 44;
  MIDIobjects[5].touchThreshold = 40;
  MIDIobjects[5].releaseThreshold = 20;
  
  // set up electrode 6 as a touch key attached to note 46 (Open Hi-Hat)
  MIDIobjects[6].type = MIDI_NOTE;
  MIDIobjects[6].noteNumber = 46;
  MIDIobjects[6].touchThreshold = 40;
  MIDIobjects[6].releaseThreshold = 20;
  
  // set up electrode 7 as a touch key attached to note 45 o 47 (Low Mid Tom)
  MIDIobjects[7].type = MIDI_NOTE;
  MIDIobjects[7].noteNumber = 45;
  MIDIobjects[7].touchThreshold = 40;
  MIDIobjects[7].releaseThreshold = 20;
  
  // set up electrode 8 as a touch key attached to note 48 o 50 (High Mid Tom)
  MIDIobjects[8].type = MIDI_NOTE;
  MIDIobjects[8].noteNumber = 48;
  MIDIobjects[8].touchThreshold = 40;
  MIDIobjects[8].releaseThreshold = 20;
  
  // set up electrode 9 as a touch key attached to note 49 (Crash Cymbal)
  MIDIobjects[9].type = MIDI_NOTE;
  MIDIobjects[9].noteNumber = 49;
  MIDIobjects[9].touchThreshold = 40;
  MIDIobjects[9].releaseThreshold = 20;
  
  // set up electrode 10 as a touch key attached to note 51 (Ride Cymbal)
  MIDIobjects[10].type = MIDI_NOTE;
  MIDIobjects[10].noteNumber = 51;
  MIDIobjects[10].touchThreshold = 40;
  MIDIobjects[10].releaseThreshold = 20;
  
  // set up electrode 11 as a touch key attached to note 53 (Ride Bell)
  MIDIobjects[11].type = MIDI_NOTE;
  MIDIobjects[11].noteNumber = 53;
  MIDIobjects[11].touchThreshold = 40;
  MIDIobjects[11].releaseThreshold = 20;

  // set touch and release thresholds for electrodes that require it
  for(int i=0; i<numElectrodes; i++){
    if(MIDIobjects[i].type == MIDI_NOTE){
      MPR121.setTouchThreshold(i, MIDIobjects[i].touchThreshold);
      MPR121.setReleaseThreshold(i, MIDIobjects[i].releaseThreshold);
    }
  }     

  // start with fresh data
  MPR121.updateAll();
}

int e_map = 0;

void loop() {
  // check note electrodes
  if(MPR121.touchStatusChanged()){
    MPR121.updateTouchData();
    for(int i=0; i<numElectrodes; i++){
      if(MIDIobjects[i].type==MIDI_NOTE){ // if this is a note type object...
        e.type = 0x08;
        e.m2 = MIDIobjects[i].noteNumber; // set note number
        //e.m3 = 127;  // maximum volume
        if(MPR121.isNewTouch(i)){
          // if we have a new touch,
          
          MPR121.updateFilteredData(); // does this for continuous proximity data
          //map the value (FilteredData) between 0 and 127 to change the volume depending on the Drum Beat (electrode)
          e_map = abs(map(MPR121.getFilteredData(i)/4,0,250,127,0));
          if (e_map > 127) { e_map = 127; }
          e.m3 = e_map;
          
          // turn on the onboard LED and
          // send a "note on" message with the appropriate note set          
          digitalWrite(LED_BUILTIN, HIGH);
          e.m1 = 0x90;
          
        } else if(MPR121.isNewRelease(i)){
          // if we have a new release, turn off the onboard LED and
          // send a "note off" message
          digitalWrite(LED_BUILTIN, LOW);
          e.m1 = 0x80;
        } else {
          // else set a flag to do nothing...
          e.m1 = 0x00;  
        }
        // only send a USB MIDI message if we need to
        if(e.m1 != 0x00){
          MIDIUSB.write(e);
        }
      }
    }
  }

  // flush USB buffer to ensure all notes are sent
  MIDIUSB.flush();

}
