# PropB3Buddy

PropB3Buddy – Companion device to the PropB3 Hammond organ emulator

This repo is a place to store the files for my ProB3 project.
First of all I didn’t design the PropB3, someone called H Bolling did that, but his website has now disappeared, however his files are included in this repo.

I built the PropB3 but left it on the shelf for a while.
After getting a new USB MIDI keyboard, I realised that an add on for the PropB3 was needed, I attempted to explain the process in this YouTube Video https://www.youtube.com/watch?v=6qKU-CcboIQ but here it is again for those who like words.

I got a new USB MIDI keyboard, a Nektar LX-61, this had several sliders and knobs that could be used to tweak various parameters – if it could speak to a 5 pin DIN MIDI, it would be really useful.
Using an Arduino USB Host shield from Circuits@home this was possible but wasted a whole genuine Arduino Uno.
At this stage, I could use USB MIDI to drive the PropB3 and convert USB MIDI to DIN MIDI.
There are several websites listing Hammond B3 registrations and one of these had an Excel spreadsheet of various sounds, by using the CSV feature of Excel, I converted these values to a text file that could be used in an Arduino sketch to program an EEPROM. 
This is the first of 2 sketches that are required for this project.

That’s great, but how will I know what sound I am on?  I need to add a display, so an I2C 16 x 2 LCD was added.
Also the USB host shield had 8 inputs and outputs, I couldn’t get the inputs to work but the outputs did, so I could also use LED’s to show if the Leslie was on or off and fast or slow.  It also shows other parameters such as keyclick, percussion, vibrato and chorus.
There were enough free Arduino pins to deal with the need for the buttons to control these too.

Also, the LCD has custom drawbar graphics that show the value of the Drawbars in real time and the other options such as Main volume, rotor/horn balance, distortion and reverb.

I added an optional circuit to flash LED’s with incoming and outgoing MIDI messages, what comes in to the DIN MIDI In is sent to DIN MIDI out, the PropB3 and flashes 2 LED’s. What comes from the USB MIDI is sent to the DIN MIDI out, the PropB3 and flashes one LED.
This optional circuit isn’t shown in the schematic, but is only a 7406 inverter chip, resistors and LED’s, it’s very simple.

The PropB3Buddy sketch sets everything up as all drawbars out, if it didn’t all drawbars would be in and no sound would be heard if we played some keys – just like on the real thing.  Moving any sliders updates the screen, it then waits for any MIDI message to be received on either the DIN MIDI or the USB MIDI and sends this to the PropB3, very simple really.

I’ve had a lot of fun with this. 
