// WhiteBox Labs -- Tentacle Shield --  UART example
//
// Simple example on how to use 4 (or 8 if using 2 Tentacle shields) Atlas Scientivic devices
// in serial mode. This sketch assumes all of your devices are either older serial devices or
// EZO circuits in serial mode. You can use the tentacle_setup.ino to autodetect and setup your devices.
//
// This sample code was written on an Arduino MEGA, with cross-compatibility for UNO in mind.
// This code does not work on the Arduino YUN; see the YUN examples.
//
// USAGE:
//---------------------------------------------------------------------------------------------
// Set host serial terminal to 9600 baud
// Serial channel numbers are 0-3. Channels 4-7 are also available, if you're using two stacked
// Tentacle shields.
// To open a channel send the number of the channel, a colon and the command ending with a carriage return.
//
// 0:r<CR>
// 1:i<CR>
// 2:c<CR>
// 3:r<CR>
// Channels on the second shield are called 4-7
//
//---------------------------------------------------------------------------------------------
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//---------------------------------------------------------------------------------------------

#include <SoftwareSerial.h>         //Include the software serial library  

SoftwareSerial sSerial(11, 10);    // RX, TX  - Name the software serial library sftSerial (this cannot be omitted)
                                    // assigned to pins 10 and 11 for maximum compatibility
int s0 = 7;                         // Tentacle uses pin 7 for multiplexer control S0
int s1 = 6;                         // Tentacle uses pin 6 for multiplexer control S1
int enable_1 = 5;	            // Tentacle uses pin 5 to control pin E on shield 1
int enable_2 = 4;	            // Tentacle uses pin 4 to control pin E on shield 2

char computerdata[20];              //A 20 byte character array to hold incoming data from a pc/mac/other
char sensordata[30];                //A 30 byte character array to hold incoming data from the sensors
byte computer_bytes_received = 0;   //We need to know how many characters bytes have been received
byte sensor_bytes_received = 0;     //We need to know how many characters bytes have been received

char *channel;                      //Char pointer used in string parsing
char *cmd;                          //Char pointer used in string parsing



void setup() {
  pinMode(s0, OUTPUT);             //Set the digital pin as output.
  pinMode(s1, OUTPUT);             //Set the digital pin as output.
  pinMode(enable_1, OUTPUT);       //Set the digital pin as output.
  pinMode(enable_2, OUTPUT);       //Set the digital pin as output.

  Serial.begin(9600);              //Set the hardware serial port to 9600
  sSerial.begin(9600);             //Set the soft serial port to 9600 (change if all your devices use another baudrate)
  intro();                         // display startup message
}


void serialEvent() {              //This interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received
  computer_bytes_received = Serial.readBytesUntil(13, computerdata, 20); //We read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received
  computerdata[computer_bytes_received] = 0; //We add a 0 to the spot in the array just after the last character we received.. This will stop us from transmitting incorrect data that may have been left in the buffer
}


void intro() {                                  // print intro
  Serial.flush();
  Serial.println(" ");
  Serial.println("READY_");
}


void loop() {

  if (computer_bytes_received != 0) {             //If computer_bytes_received does not equal zero
    channel = strtok(computerdata, ":");          //Let's parse the string at each colon
    cmd = strtok(NULL, ":");                      //Let's parse the string at each colon
    open_channel();                               //Call the function "open_channel" to open the correct data path
    if (cmd != 0) {                               //if no command has been sent, send nothing
      sSerial.print(cmd);                        //Send the command from the computer to the Atlas Scientific device using the softserial port
      sSerial.print("\r");                       //After we send the command we send a carriage return <CR>
    }
    computer_bytes_received = 0;                  //Reset the var computer_bytes_received to equal 0
  }

  if (sSerial.available() > 0) {                 //If data has been transmitted from an Atlas Scientific device
    sensor_bytes_received = sSerial.readBytesUntil(13, sensordata, 30); //we read the data sent from the Atlas Scientific device until we see a <CR>. We also count how many character have been received
    sensordata[sensor_bytes_received] = 0;         //we add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer
    Serial.println(sensordata);                    //let’s transmit the data received from the Atlas Scientific device to the serial monitor
  }
}


void open_channel() {                        //This function controls what UART port is opened.

  switch (*channel) {                        //Looking to see what channel to open

    case '0':                                //If channel==0 then we open channel 0
      digitalWrite(enable_1, LOW);           //Setting enable_1 to low activates primary channels: 0,1,2,3
      digitalWrite(enable_2, HIGH);          //Setting enable_2 to high deactivates secondary channels: 4,5,6,7
      digitalWrite(s0, LOW);                 //S0 and S1 control what channel opens
      digitalWrite(s1, LOW);                 //S0 and S1 control what channel opens
      break;                                 //Exit switch case

    case '1':
      digitalWrite(enable_1, LOW);
      digitalWrite(enable_2, HIGH);
      digitalWrite(s0, HIGH);
      digitalWrite(s1, LOW);
      break;

    case '2':
      digitalWrite(enable_1, LOW);
      digitalWrite(enable_2, HIGH);
      digitalWrite(s0, LOW);
      digitalWrite(s1, HIGH);
      break;

    case '3':
      digitalWrite(enable_1, LOW);
      digitalWrite(enable_2, HIGH);
      digitalWrite(s0, HIGH);
      digitalWrite(s1, HIGH);
      break;

    case '4':
      digitalWrite(enable_1, HIGH);
      digitalWrite(enable_2, LOW);
      digitalWrite(s0, LOW);
      digitalWrite(s1, LOW);
      break;

    case '5':
      digitalWrite(enable_1, HIGH);
      digitalWrite(enable_2, LOW);
      digitalWrite(s0, HIGH);
      digitalWrite(s1, LOW);
      break;

    case '6':
      digitalWrite(enable_1, HIGH);
      digitalWrite(enable_2, LOW);
      digitalWrite(s0, LOW);
      digitalWrite(s1, HIGH);
      break;

    case '7':
      digitalWrite(enable_1, HIGH);
      digitalWrite(enable_2, LOW);
      digitalWrite(s0, HIGH);
      digitalWrite(s1, HIGH);
      break;
  }
}
