
//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "fota-905e1.firebaseio.com"
#define FIREBASE_AUTH "Bpdn4xtRL1NG1vP1r2mgb85QpTxbVeWsEYgzpnky"
#define WIFI_SSID "Eba6al-SD"
#define WIFI_PASSWORD "Orochi12"

unsigned char RxBuffer[8];
unsigned char TxBuffer[8];

bool SendRQT, ResponseRQT;

void setup() {
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  /* TODO: to be removed, these variable controlled by PC */
  Firebase.setBool("FlashNewApp", false );

  Firebase.setBool("Send", true );
  Firebase.setBool("ResponseRQT", true );
  Firebase.setInt("MSBFrame", 0x0A020A02);
  Firebase.setInt("LSBFrame", 0x09040904);
  Firebase.setString("habda", "414507000030074d");
}



void loop()
{
  uint64_t u64_rxbuffer;
  String string_buffer;
  uint32_t u32_rxbufferMSB, u32_rxbufferLSB,*ptr;
  
  if (Firebase.getBool("FlashNewApp") == true)
  {
    SendRQT = Firebase.getBool("Send");

    string_buffer = Firebase.getString("habda");
   
    u32_rxbufferLSB = strtoul(string_buffer.substring(0,8).c_str(),NULL,16);
    u32_rxbufferMSB = strtoul(string_buffer.substring(8,17).c_str(),NULL,16) ;
    u64_rxbuffer = ((uint64_t)(u32_rxbufferMSB << 32))|((uint64_t)u32_rxbufferLSB);
    

    ptr = (uint32_t*)&u64_rxbuffer;

    
    delay(100);
    ResponseRQT = Firebase.getBool("ResponseRQT");

    delay(100);
    if (SendRQT == true && ResponseRQT == true)
    {

      delay(100);
      Serial.println(string_buffer);
      Serial.println(u32_rxbufferLSB);
      Serial.println(u32_rxbufferMSB);
      Serial.printf("%ld %ld\n",ptr[1],ptr[0]);
      SendRQT = false;
      Firebase.setBool("Send", SendRQT);
      delay(100);
      }

    }
}












/*

    int n = 0;
  // set value
  Firebase.setFloat("number", 42.0);
  // handle error
  if (Firebase.failed()) {
     Serial.print("setting /number failed:");
     Serial.println(Firebase.error());
     return;
  }
  delay(1000);
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);


  // update value

  // handle error
  if (Firebase.failed()) {
     Serial.print("setting /number failed:");
     Serial.println(Firebase.error());
     return;
  }
  delay(1000);

  // get value
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  Firebase.remove("number");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
     Serial.print("setting /message failed:");
     Serial.println(Firebase.error());
     return;
  }
  delay(1000);

  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
     Serial.print("setting /truth failed:");
     Serial.println(Firebase.error());
     return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
     Serial.print("pushing /logs failed:");
     Serial.println(Firebase.error());
     return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
*/
