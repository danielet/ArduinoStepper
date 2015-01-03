/*
Arduino Stepper
Copyright (C) 2014  Matteo Danieletto matteo.danieletto@dei.unipd.it
University of Padova, Italy +34 049 827 7778
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <SPI.h>
#include <Ethernet.h>




// Enter a MAC address of your board
byte mac[] = { 0x98, 0x4F, 0xEE, 0x01, 0x05, 0x4D };
//IP address you want assign
IPAddress ip(192,168,1,10);
//IP address of your gateway and subnet are optional:
IPAddress gateway(192,168,1,254);
IPAddress subnet(255,255,255,0);


// telnet defaults to port 23
EthernetServer server(23);
// whether or not the client was connected previously
boolean        alreadyConnected = false; 
// whether or not to enable stepper
boolean        enableStepper = false; 

String         Digits          =  "";
int            multiple      =  10;

//Setup during the boot
void setup() {
  // initialize the ethernet device
  Ethernet.begin(mac, ip, gateway, subnet);
  // start listening for clients
  server.begin();
 // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.print("Open connection: ");
  Serial.println(Ethernet.localIP());
  
  //Stepper setup
  Serial.print("Stepper Setup");
  //Enable PIN 2,3,4,5
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  //Setup PIN 2,3,4,5 LOW status
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  
}//setup




void stepperMove(int numberTimes)
{
 Serial.println("StepperMove");
 
 for(int ii=0; ii < (multiple*numberTimes); ii++)
 {
//  Serial.println(ii); 
  //Fed only first 
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(2);
 
  //Fed only second 
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(2);

  //Fed only third
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(2);


  //Fed only fourth
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(2);
 }
}


void loop() {
  // wait for a new client:
  EthernetClient client = server.available();
  int numberMove;
  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clead out the input buffer:
      client.flush();    
//      Serial.println("We have a new client");
      client.println("You are connected with Arduino stepper mode"); 
      alreadyConnected = true;
    } 

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char singleCharRead = client.read();
           
      switch(singleCharRead)
      {
        case 0x0d:
          numberMove  = Digits.toInt();
//          Serial.println("Carraige return");
          Serial.println(numberMove); 
          if(numberMove > 0 )
          {
            stepperMove(numberMove);
          }
          else
          {
            client.println("Close Connection\n");
            alreadyConnected = false;
            client.stop();
          }
        break;
        
        case '\n':      
          Digits="";
//          Serial.println("Newline");        
        break;
        case 'h':
          client.println("Menu");
        break;
        default:
          Digits+=(char)singleCharRead;
        
      }  
    }
  }
}
