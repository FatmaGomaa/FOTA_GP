# Purpose 

This document describes the steps we followed and the errors we faced to connect NodeMcu with Google Firebase

## Steps for connecting NodeMcu with Google Firebase

1. Download the Firebase Arduino library from https://github.com/FirebaseExtended/firebase-arduino

![](/Gateway_Node/Images/1.jpg)

2. Add the Firebase Arduino library to Arduino IDE, click Sketch -> Include Library -> Add .ZIP Library.... and select downloaded library.

<p align="center">
  <img src="/Gateway_Node/Images/2.jpg">
</p>

3. If Firebase Arduino library is successfully added, it shows in Include Library, Now, Login the Google Firebase using your Google account. Create a Firebase project by clicking Add project.

4. A program on arduino IDE to connect NodeMCU and Google Click, File > Examples > FirebaseArduino > FirebaseDemo_ESP8266 Click on Project Overview > Project Settings > Service Account > Database secrets to view firebase auth secrets, add this secret to your Arduino program's FIREBASE_AUTH.
   
   Go to database section at left-menu and search for Realtime Database, where you find the Firebase host URL. Copy this URL without "https://" and "/" the at end and paste it at FIREBASE_HOST in the program.
   
   Add Realtime database in your project, click Project Overview setting > Realtime Database.

![](/Gateway_Node/Images/3.jpg)

5. Add your WIFI name and password in WIFI_SSID & WIFI_PASSWORD respectively,then write the code sequence and build it.


## The Errors Causes and Solutions 

### 1- ArduinoJson library not exist 

You should install ArduinoJson version 5.13.5 not the latest version.

### 2- Exception(9) and (28) 

![](/Gateway_Node/Images/4.jpg)

We Started with looking up exception code in the Exception Causes(EXCCAUSE) table to understand what kind of issue it is. We have no clues what it’s about and where it happens, so we used Arduino ESP8266/ESP32 Exception Stack Trace Decoder to find out in which line of application it is triggered.
After a lot of search and trying many solutions we discovered that the problem was because of some issues in the library we use at (2.1) step (1) so we used Firebase real-time database Arduino library for ESP8266 it’s Google's v 2.9.0, we used it with using the first library 

Steps for using it :
1. Instalation Using Library Manager At Arduino IDE, go to menu Sketch -> Include Library -> Manage Libraries..
   In Library Manager Window, search "firebase" in the search form then select "Firebase ESP8266 Client". Click "Install" button.
2. We added this line to our code and we started to use it in each Firebase API’s       
   
   ```
   // Declare the Firebase Data object in the global scope 
      FirebaseData firebaseData; 
   ```
Then we replaced our code with the new way using this object, for Example:

![](/Gateway_Node/Images/5.jpg)

### 3- Exception(29)

After reading it from the exception table and trying many solutions like using ArduinoJson library we found that it’s because when we used firebase database library as solution for problem (3.2), this library has buffer for each object and we should use those line of code to change the size of the buffer corresponding to our data to avoid data corruption  
 

```
/* Optional, set the size of BearSSL WiFi to receive and transmit buffers */ 
       firebaseData.setBSSLBufferSize(4000, 4000); //minimum size is 4096 bytes, maximum size is 16384 bytes
```

```
/* Optional, set the size of HTTP response buffer */
       firebaseData.setResponseSize(4000); //minimum size is 400 bytes
```

And we also added those line of code to avoid any watchdog timer issues 
 
```
   Add in setup()
   ESP.wdtDisable(); 
   ESP.wdtEnable(WDTO_8S);

   And in loop()
   ESP.wdtFeed();
```

### 4- Corrupted Data 

As our project sequence is the Pc send 8 bytes to the cloud in hex format and we should receive from it 8 bytes, we discovered after receiving it, the data size is 16 bytes not 8 bytes and that’s because each byte is equal 2 digit in hex format so we must do something to send those 16 bytes as 8 bytes to the target we use (STM32F10)
![](/Gateway_Node/Images/6.jpg)

We used buffer to receive the data from the cloud and we initialized this buffer by 3200 zero’s for two reasons first one the initialization it self to avoid any data corruption, Second one this number because the  pc send 200 frame each one of them is 8 bytes so the total number is 1600 byte and we receive those 1600 byte multiplied by 2 because of each byte is equal 2 digits in hex format so the numbe is 3200

```
String TX_string_buffer = "00000…………”
```
Then we receive each 2 digits from the cloud in buffer and we send it to STM via UART

## References
```
1. https://www.javatpoint.com/iot-project-google-firebase-nodemcu
2. https://arduino-esp8266.readthedocs.io/en/latest/exception_causes.html
3. https://github.com/me-no-dev/EspExceptionDecoder
4. https://arduino-esp8266.readthedocs.io/en/latest/faq/a02-my-esp-crashes.html#watchdog
5. https://github.com/mobizt/Firebase-ESP8266/blob/master/README.md
```