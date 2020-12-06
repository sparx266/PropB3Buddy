/*
 *******************************************************************************
   USB-MIDI to Legacy Serial MIDI converter
   Copyright 2012-2014 Yuuichi Akagawa

   Idea from LPK25 USB-MIDI to Serial MIDI converter
     by Collin Cunningham - makezine.com, narbotic.com

   for use with USB Host Shield 2.0 from Circuitsathome.com
   https://github.com/felis/USB_Host_Shield_2.0
 *******************************************************************************
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>
 *******************************************************************************
*/

#include <Usb.h>
#include <usbh_midi.h>
#include <USB_Host_Shield_GPIO.h>
#include <MIDI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
#include <Bounce2.h>

#define disk1 0x50    //Address of 24LC256 eeprom chip

//These are the custom drawbars for the LCD
uint8_t db1[8]  = {0x1F, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t db2[8]  = {0xe, 0x1F, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t db3[8]  = {0xe, 0xe, 0x1F, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t db4[8]  = {0xE, 0xE, 0xE, 0x1f, 0x0, 0x0, 0x0, 0x0};
uint8_t db5[8]  = {0xE, 0xE, 0xE, 0xE, 0x1f, 0x0, 0x0, 0x0};
uint8_t db6[8]  = {0xE, 0xE , 0xE, 0xE, 0xE, 0x1F, 0x0, 0x0};
uint8_t db7[8]  = {0xE, 0xE, 0xE, 0xe, 0xE, 0xe, 0x1F, 0x0};
uint8_t db8[8]  = {0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0x1F};
//End of custom LCD

int x;              //Variable to hold the MIDI Channel number (its 1 in this case so is a constant really)
int y;              //Variable to hold MIDI CC number
int z;              //Variable to hold MIDI data value
int vibcount = 1;   //Variable to hold Vibrato count
int perccount = 1;  //Variable to hold percussion count
char voicename[16]; //Array to hold the Patch name
int k;              //Variable to hold
int address=0;


//unsigned int address = 0; //Variable to hold the EEPROM address

byte patch=0;       //

unsigned long TimerA;
unsigned long TimerB;
unsigned long TimerC;
unsigned long TimerD;
unsigned long TimerE;
unsigned long TimerF;
unsigned long TimerG;

bool leslie;     
//bool slow;
//bool fast;

#define BUTTON_PIN_2 2
#define BUTTON_PIN_3 3
#define BUTTON_PIN_4 4
#define BUTTON_PIN_5 5
#define BUTTON_PIN_6 6
#define BUTTON_PIN_7 7
#define BUTTON_PIN_14 14
#define BUTTON_PIN_15 15

// Instantiate a Bounce object :
Bounce debouncer2 = Bounce();

// Instantiate another Bounce object
Bounce debouncer3 = Bounce();

// Instantiate a Bounce object
Bounce debouncer4 = Bounce();

// Instantiate another Bounce object
Bounce debouncer5 = Bounce();

// Instantiate a Bounce object
Bounce debouncer6 = Bounce();

// Instantiate another Bounce object
Bounce debouncer7 = Bounce();

// Instantiate a Bounce object
Bounce debouncer14 = Bounce();

// Instantiate another Bounce object
Bounce debouncer15 = Bounce();


#ifdef USBCON
#define _MIDI_SERIAL_PORT Serial1
#else
#define _MIDI_SERIAL_PORT Serial
#endif
//////////////////////////
// MIDI Pin assign
// 2 : GND
// 4 : +5V(Vcc) with 220ohm
// 5 : TX
//////////////////////////
MIDI_CREATE_DEFAULT_INSTANCE();
MaxGPIO max;


USB  Usb;
USBH_MIDI  Midi(&Usb);

void MIDI_poll();
void doDelay(unsigned long t1, unsigned long t2, unsigned long delayTime);
void drawbar2(byte y, byte z);
void divideby16(byte z);

void setup()
{
  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer2.attach(BUTTON_PIN_2);
  debouncer2.interval(100);


  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer3.attach(BUTTON_PIN_3);
  debouncer3.interval(100); // interval in ms

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN_4, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer4.attach(BUTTON_PIN_4);
  debouncer4.interval(100); // interval in ms

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN_5, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer5.attach(BUTTON_PIN_5);
  debouncer5.interval(100); // interval in ms

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN_6, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer6.attach(BUTTON_PIN_6);
  debouncer6.interval(100); // interval in ms

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN_7, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer7.attach(BUTTON_PIN_7);
  debouncer7.interval(100); // interval in ms

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN_14, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer14.attach(BUTTON_PIN_14);
  debouncer14.interval(100); // interval in ms

  // Setup the second button with an internal pull-up :
  pinMode(BUTTON_PIN_15, INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer15.attach(BUTTON_PIN_15);
  debouncer15.interval(100); // interval in ms



//  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.backlight();

  lcd.createChar(1, db1);
  lcd.createChar(2, db2);
  lcd.createChar(3, db3);
  lcd.createChar(4, db4);
  lcd.createChar(5, db5);
  lcd.createChar(6, db6);
  lcd.createChar(7, db7);
  lcd.createChar(8, db8);
  lcd.home();


  _MIDI_SERIAL_PORT.begin(31250);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  //Workaround for non UHS2.0 Shield
  //pinMode(7, OUTPUT);
  //digitalWrite(7, HIGH);

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );

  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print("     PropB3");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("    Sparx266    ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("                 ");
  //
  lcd.setCursor(0, 1);
  lcd.write(8);
  lcd.write(8);
  lcd.write(8);
  lcd.write(8);
  lcd.write(8);
  lcd.write(8);
  lcd.write(8);
  lcd.write(8);
  lcd.write(8);
  lcd.setCursor(12, 1);
  lcd.write(8);
  lcd.write(8);
  lcd.write(8);
  lcd.write(8);

  max.write(0, LOW);  //Leslie On/Off LED
  max.write(1, LOW);   //Leslie slow LED
  max.write(2, LOW);   //Leslie fast LED
  max.write(3, LOW);
  max.write(4, LOW);
  max.write(5, LOW);
  max.write(6, LOW);
  max.write(7, LOW);
}

void loop()
{


  unsigned long t1;
  uint8_t msg[4];
  Usb.Task();
  t1 = micros();
  if ( Usb.getUsbTaskState() == USB_STATE_RUNNING )
  {
    MIDI_poll();

  }
  if (MIDI.read()) {
    msg[0] = MIDI.getType();
    if ( msg[0] == 0xf0 ) { //SysEX
      //TODO
      //SysEx implementation is not yet.
    } else {
      msg[1] = MIDI.getData1();
      msg[2] = MIDI.getData2();
      Midi.SendData(msg, 0);
    }
    x = msg[0];
    y = msg[1];
    z = msg[2];

    if (x == 0xB0) {
      // Push CC messages from MIDI to the drawbar display routine
      drawbar2(y, z);
    }


  }

  //delay(1ms)
  doDelay(t1, micros(), 1000);
  // This is where the buttons are read
  // Update the Bounce instance :
  debouncer2.update();
  debouncer3.update();
  debouncer4.update();
  debouncer5.update();
  debouncer6.update();
  debouncer7.update();
  debouncer14.update();
  debouncer15.update();

  // Call code if Bounce fell (transition from HIGH to LOW) :
  if ( debouncer2.fell() ) {
  //  Serial.println("Fast Pressed");
    y = 1;
    z = 64;
    //Send a CC
    MIDI.sendControlChange(1, 64, 1);
    //End of send the CC
    drawbar2(y, z);
  }
  else if ( debouncer3.fell() ) {
    //    Down Pressed
    lcd.setCursor(0, 0);
    lcd.print("   ");
    patch=(patch-1);
    if (patch == 1)
    {
      patch = 1;
    }
//    lcd.setCursor(0, 0);
//    lcd.print(patch,DEC);
    getVoice(patch);
//    }

  }

  else if ( debouncer14.fell() ) {
    // Serial.println("Slow Pressed");
    y = 1;
    z = 0;
    //Send a CC
    MIDI.sendControlChange(1, 0, 1);
    //End of send the CC
    drawbar2(y, z);
  }

  else if ( debouncer15.fell() ) {
    //  Serial.println("Leslie Pressed");
    y = 68;
    if (leslie == 1)
    {
      leslie = 0;
      z = 0;
      //Send a CC
      MIDI.sendControlChange(68, 0, 1);
      //End of send the CC
    }
    else if (leslie == 0)
    {
      z = 64;
      leslie = 1;
      //Send a CC
      MIDI.sendControlChange(68, 64, 1);
      //End of send the CC
    }

    drawbar2(y, z);

  }
  else if ( debouncer6.fell() ) {
    //  Serial.println("V/C Pressed");
    if (vibcount == 1)
    {
      y = 31;
      z = 8;
      //Send a CC
      MIDI.sendControlChange(31, 8, 1); // Added this to the EEPROM routine 
      //End of send the CC
      drawbar2(y, z);

    }
    else if (vibcount == 2)
    {
      y = 31;
      z = 16;
      //Send a CC
      MIDI.sendControlChange(31, 16, 1);
      //End of send the CC
      drawbar2(y, z);

    }
    else if (vibcount == 3)
    {
      y = 31;
      z = 0;
      //Send a CC
      MIDI.sendControlChange(31, 0, 1);
      //End of send the CC
      drawbar2(y, z);

    }
    vibcount = (vibcount + 1);
    if (vibcount > 3)
    {
      (vibcount = 1);

    }


  }
  else if ( debouncer5.fell() ) {
      //  Serial.println("Percussion Pressed");

    if (perccount == 1)
    {
      y = 66;
      z = 8;
      //Send a CC
      MIDI.sendControlChange(66, 8, 1);
      //End of send the CC
      drawbar2(y, z);

    }
    else if (perccount == 2)
    {
      y = 66;
      z = 16;
      //Send a CC
      MIDI.sendControlChange(66, 16, 1);
      //End of send the CC
      drawbar2(y, z);

    }
    else if (perccount == 3)
    {
      y = 66;
      z = 0;
      //Send a CC
      MIDI.sendControlChange(66, 0, 1);
      //End of send the CC
      drawbar2(y, z);

    }
    perccount = (perccount + 1);
    if (perccount > 3)
    {
      (perccount = 1);
    }

  }
  else if ( debouncer4.fell() ) {
    //  Serial.println("Up Pressed");
    lcd.setCursor(0, 0);
    lcd.print("   ");
    patch=(patch+1);
    if (patch > 175)
    {
      patch = 175;
    }
  //  lcd.setCursor(0, 0);
  //  lcd.print(patch,DEC);
// Put the Voice up routine here
  getVoice(patch);
  }

  // End of where the buttons are read

}

// Poll USB MIDI Controller and send to serial MIDI
void MIDI_poll()
{
  byte outBuf[ 3 ];
  uint8_t size;

  do {
    if ( (size = Midi.RcvData(outBuf)) > 0 ) {
      //MIDI Output
      _MIDI_SERIAL_PORT.write(outBuf, size);
      //  Push USB MIDI CC messages to the drawbar display routint
      if (outBuf[0] == 0xB0)
      {
        x = outBuf[0];
        y = outBuf[1];
        z = outBuf[2];
        drawbar2(y, z);
      }
    }
  } while (size > 0);
}



// drawbar 2 function
void drawbar2(byte y, byte z)
{
  //Set the LCD position for a drawbar
  switch (y ) {
    //Leslie On/Off
    case 68:
      if (z == 0)
      {
        max.write(0, LOW);
      }
      else if (z == 64)
      {
        max.write(0, HIGH);
      }
      break;
    // Leslie Slow or Fast
    case 1:
      if (z == 0)
      {
        max.write(1, HIGH);
        max.write(2, LOW);
      }
      else if (z == 64)
      {
        max.write(1, LOW);
        max.write(2, HIGH);
      }
      break;

    //
    case 31:
      if (z == 0)
      {
        max.write(3, LOW);
        max.write(4, LOW);

      }
      else if (z == 8)
      {
        max.write(3, HIGH);
        max.write(4, LOW);
      }

      else if (z == 16)
      {
        max.write(3, LOW);
        max.write(4, HIGH);
      }
      break;

    //Percussion
    case 66:
      if (z == 0)
      {
        max.write(5, LOW);
        max.write(6, LOW);

      }
      else if (z == 8)
      {
        max.write(5, HIGH);
        max.write(6, LOW);
      }

      else if (z == 16)
      {
        max.write(5, LOW);
        max.write(6, HIGH);
      }
      break;


    // Main volume
    case 7:
      //do something when y equals 7
      lcd.setCursor(12, 1);
      divideby16(z);
      break;

    // Balance
    case 8:
      //do something when y equals 8
      lcd.setCursor(13, 1);
      divideby16(z);
      break;


    // Distortion
    case 9:
      //do something when y equals 9
      lcd.setCursor(14, 1);
      divideby16(z);
      break;

    // Reverb
    case 91:
      //do something when y equals 91
      lcd.setCursor(15, 1);
      divideby16(z);
      break;

    // Drawbars
    case 12:
      //do something when y equals 12
      lcd.setCursor(0, 1);
      divideby16(z);
      break;

    case 13:
      //do something when y equals 13
      lcd.setCursor(1, 1);
      divideby16(z);
      break;

    case 14:
      //do something when y equals 14
      lcd.setCursor(2, 1);
      divideby16(z);
      break;

    case 15:
      //do something when y equals 15
      lcd.setCursor(3, 1);
      divideby16(z);
      break;

    case 16:
      lcd.setCursor(4, 1);
      divideby16(z);
      break;

    case 17:
      //do something when y equals 17
      lcd.setCursor(5, 1);
      divideby16(z);
      break;

    case 18:
      //do something when y equals 18
      lcd.setCursor(6, 1);
      divideby16(z);
      break;

    case 19:
      //do something when y equals 19
      lcd.setCursor(7, 1);
      divideby16(z);
      break;

    case 20:
      //do something when y equals 20
      lcd.setCursor(8, 1);
      divideby16(z);
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }
}


// Delay time (max 16383 us)
void doDelay(unsigned long t1, unsigned long t2, unsigned long delayTime)
{
  unsigned long t3;

  if ( t1 > t2 ) {
    t3 = (4294967295 - t1 + t2);
  } else {
    t3 = t2 - t1;
  }

  if ( t3 < delayTime ) {
    delayMicroseconds(delayTime - t3);
  }
}

void divideby16(byte z)
{
  //z is now divided by 16, this is to update the LCD drawbars
  z = ((z + 1) / 16);
  if (z == 0) {
    lcd.print(" ");
  }
  else {
    lcd.write(z);
  }
  // divide by 16 end
}

void getVoice(byte patch)
{
 int cc12;
 
  address=(patch*32);
   for (k = 0; k < 16; k++)
   {
   
//Serial.print(readEEPROM(disk1, address), DEC);
   voicename[k]=(readEEPROM(disk1, address));
   address++;
   }
   lcd.setCursor(0, 0);
   lcd.print("                ");
   lcd.setCursor(0, 0);
   lcd.print(voicename);
   //Now get the voice data:
  
//  address++;
  address++;
  address++;
  address++;
  
   
  z = (readEEPROM(disk1, address));
  x = 1;
  y = 12;
  drawbar2(y, z);
  MIDI.sendControlChange(y, z, x);
 //lcd.setCursor(10, 1);
 //  lcd.print(z);

  address++;
  //x = 0xB0;
  y = 13;
  z = (readEEPROM(disk1, address));
  drawbar2(y, z);
  MIDI.sendControlChange(y, z, 1);

  address++;
  x = 0xB0;
  y = 14;
  z= (readEEPROM(disk1, address));
  drawbar2(y, z);
  MIDI.sendControlChange(y, z, 1);

  address++;
  x = 0xB0;
  y = 15;
  z= (readEEPROM(disk1, address));
  drawbar2(y, z);
  MIDI.sendControlChange(y, z, 1);

  address++;
  x = 0xB0;
  y = 16;
  z= (readEEPROM(disk1, address));
  drawbar2(y, z);
  MIDI.sendControlChange(y, z, 1);

 address++;
  x = 0xB0;
  y = 17;
  z= (readEEPROM(disk1, address));
  drawbar2(y, z);
  MIDI.sendControlChange(y, z, 1);

 address++;
  x = 0xB0;
  y = 18;
  z= (readEEPROM(disk1, address));
  drawbar2(y, z);
  MIDI.sendControlChange(y, z, 1);

 address++;
  x = 1;
  y = 19;
  z= (readEEPROM(disk1, address));
  drawbar2(y, z);
  MIDI.sendControlChange(y, z, 1);

 address++;
  x = 1;
  y = 20;
  z = (readEEPROM(disk1, address));
  drawbar2(y, z);
  MIDI.sendControlChange(y, z, 1);
 // End of drawbars

address++;

 //Turn Leslie on or off:
z = (readEEPROM(disk1, address));
y=1;
drawbar2(y, z);
  if (z != 64)
      {
      max.write(0, LOW);
      max.write(1, LOW);
      max.write(2, LOW);
      MIDI.sendControlChange(68, 0, 1);
      }
      else if (z == 64)
      {
        max.write(0, HIGH);
              //Send a CC
      MIDI.sendControlChange(68, 64, 1);
      }
  // End of turn Leslie light on or off

//if (leslie == 1)
//    {
//      leslie = 0;
//      z = 0;
//      //Send a CC
//      MIDI.sendControlChange(68, 0, 1);
//      //End of send the CC
//    }
//else if (leslie == 0)
//    {
//      z = 64;
//      leslie = 1;
      //Send a CC
//      MIDI.sendControlChange(68, 64, 1);
//   }
 //End of send the CC
   
 address++;
 z = (readEEPROM(disk1, address));
// Vib or Chorus
x = 1;
y=31;
MIDI.sendControlChange(31, z, 1);
drawbar2(y, z);


// end of vib/chorus
 address++;

// Start of Leslie speed
z = (readEEPROM(disk1, address));
y = 1;
x = 1;

MIDI.sendControlChange(x, z, 1);  //Changed from the line below
//MIDI.sendControlChange(31, z, 1);
drawbar2(y, z);
// End of Leslie speed
     
address++;


// Start of percussion
z = (readEEPROM(disk1, address));
y = 66;
x = 0xB0;
MIDI.sendControlChange(66, z, 1);
drawbar2(y, z);
//  address++;
// Temp test routine
// lcd.setCursor(10, 0); //Start at character 0 on line 0
//  lcd.print(address);
//  lcd.print(" ");
//  z = (readEEPROM(disk1, address));
//  lcd.print(z);
//End of test routine


// End of percussion

//   for (x = 0; x < 13; x++)
//   {
//   z=(readEEPROM(disk1, address));
   //Serial.print(x,DEC);
//   Serial.print(" ");
//   Serial.print(z,DEC);
//   Serial.print(" ");
   //Serial.print(address);
   //Serial.print(" ");
//   address++;
//   }
//End of get voice data  
address++;
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte myChar ) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(myChar);
  Wire.endTransmission();
 
  delay(5);
}

byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,1);
 
  if (Wire.available()) rdata = Wire.read();
 
  return rdata;
}


