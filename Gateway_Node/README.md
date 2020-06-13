# Purpose 

This document describes the steps we followed and the errors we faced to connect NodeMcu with Google Firebase

## Table of Contents
* [Introduction to NodeMCU](#introduction-to-nodemcu)
* [Steps for connecting NodeMcu with Google Firebase](#steps-for-connecting-nodemcu-with-google-firebase)
* [Project Firebase Structure](#project-firebase-structure)
* [Code FlowChart and Logic Explanation](#code-flowchart-and-logic-explanation)
* [Errors Causes and Solutions](#the-errors-causes-and-solutions)

## Introduction to NodeMCU.

<p align="center">
  <img src="/Gateway_Node/Images/intro0.png">
</p>

The development board equips the ESP-12E module containing ESP8266 chip having **Tensilica Xtensa® 32-bit LX106 RISC microprocessor** which operates at 80 to 160 MHz adjustable clock frequency.

<p align="center">
  <img src="/Gateway_Node/Images/intro1.PNG">
</p>

There’s also **128 KB RAM and 4MB of Flash memory** (for program and data storage) just enough to cope with the large strings that make up web pages, JSON/XML data, and everything we throw at IoT devices nowadays.

The ESP8266 Integrates **802.11b/g/n HT40 Wi-Fi transceiver**, so it can not only connect to a WiFi network and interact with the Internet, but it can also set up a network of its own, allowing other devices to connect directly to it. This makes the ESP8266 NodeMCU even more versatile.

As the operating voltage range of ESP8266 is **3V to 3.6V**, the board comes with a LDO voltage regulator to keep the voltage steady at 3.3V. It can reliably supply up to 600mA.

<p align="center">
  <img src="/Gateway_Node/Images/intro2.PNG">
</p>

The ESP8266 NodeMCU has total **17 GPIO** pins broken out to the pin headers on both sides of the development board. These pins can be assigned to all sorts of peripheral duties, including:

* **ADC channel** – A 10-bit ADC channel.
* **UART interface** – UART interface is used to load code serially.
* **PWM outputs** – PWM pins for dimming LEDs or controlling motors.
* **SPI, I2C & I2S interface** – SPI and I2C interface to hook up all sorts of sensors and peripherals.
* **I2S interface** – I2S interface if you want to add sound to your project.

<p align="center">
  <img src="/Gateway_Node/Images/intro3.PNG">
</p>

The ESP8266 NodeMCU features two buttons. One marked as **RST** located on the top left corner is the Reset button, used of course to reset the ESP8266 chip. The other **FLASH** button on the bottom left corner is the download button used while upgrading firmware.
The board also has a **LED** indicator which is user programmable and is connected to the D0 pin of the board.

<p align="center">
  <img src="/Gateway_Node/Images/intro4.PNG">
</p>

## Steps for connecting NodeMcu with Google Firebase

1. If you don't have ESP8266 library in your Arduino IDE use this link https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/ to install it, then Download the Firebase Arduino library from https://github.com/FirebaseExtended/firebase-arduino

![](/Gateway_Node/Images/1.jpg)

2. Add the Firebase Arduino library to Arduino IDE, click Sketch -> Include Library -> Add .ZIP Library.... and select downloaded library.

<p align="center">
  <img src="/Gateway_Node/Images/2.jpg">
</p>

3. If Firebase Arduino library is successfully added, it shows in Include Library, Now, Login the Google Firebase using your Google account. Create a Firebase project by clicking Add project.
 
 <p align="center">
  <img img src="/Gateway_Node/Images/3.jpg" width=300 height=380 />
  <img img src="/Gateway_Node/Images/4.jpg" width=300 height=380 /> 
</p>
 
4. Click, Project Overview setting where you will find your project's detail. Now, click on Service accounts option to view database secrets.
   
   A program on arduino IDE to connect NodeMCU and Google Click, File > Examples > FirebaseArduino > FirebaseDemo_ESP8266
  
   Click on Project Overview > Project Settings > Service Account > Database secrets to view firebase auth secrets, add this secret to your Arduino program's FIREBASE_AUTH.
   
   Go to database section at left-menu and search for Realtime Database, where you find the Firebase host URL. Copy this URL without "https://" and "/" the at end and paste it at FIREBASE_HOST in the program.
   
   Add Realtime database in your project, click Project Overview setting > Realtime Database.

![](/Gateway_Node/Images/5.jpg)

5. Add your WIFI name and password in WIFI_SSID & WIFI_PASSWORD respectively,then write the code sequence and build it.

## Project Firebase Structure. 
<p align="center">
  <img src="/Gateway_Node/Images/FirebaseChannels.PNG">
</p>

* **FlashNewApp**         : Indicates that a New Flashing Sequence is about to start.
* **Frame**               : Holds the Current Frame which is one of Four options **Data Command** or **Erase Command** or **Verification Command** or **Response Command**
* **Marker**              : Holds the Marker Frame and then the Marker Frame Response
* **MarkerRQT**           : Inicates is updated when New Flashing Sequence is about to start.
* **NodeMCUSemaphore**    : NodeMCUs checks that there is no other Node is writing in NodeMCUs channel right now.
* **NodeMCUs**            : Resgitered NodeMCUs that available for flashing. 
* **ResponseRQT**         : Flag to indicate that the uploaded command would wait for Response like **Erase Command** or **Verification Command**
* **SelectedGateway**     : Updates when **GUI** selectes specific target to Flash.
* **Send**                : **PC Application** sets it True when Uploading Command, **NodeMCU** sets it False after reading the Command, Major use is **Synchronization**.

## Code FlowChart and Logic Explanation.
<p align="center">
  <img src="/Gateway_Node/Images/NodeMCU_FlowChart.png">
</p>

* **NodeMCU** is used as a Gateway, so it's main job is to receive commands through WIFI from Firebase and trasnmit them serially using UART to STM or any microcontroller used.
* **NodeMCU** Synchrnoization between NodeMCU and PC Application responsible for Flashing Sequence is done through some Flags on Firebase ex) **Send** and **MarkerRQT**
* **NodeMCU** starts with setting up its Environment like Initializing WatchDog, Connecting to WIFI and Firebase, Setting up capacity of HTTP Transfer and Finally Registering itself on available **NodeMCUs** channel on Firebase
* **NodeMCU** Loop is Responsible for the Following:
   * Feeding the WatchDog Timer.
   * Fetching **FlashNewApp** and **SelectedGateway** Flags from Firebase, if the NodeMCU is the selected Node to be flashed and there is a new software to flash, **NodeMCU** enters Flashing Mode
   * In **Flashing Mode**, NodeMCU gets important flags from Firebase which are : **Send**, **ResponseRQT** and **MarkerRQT**
    * IF **MarkerRQT** is True it means that the **PC Application** started communication with the **NodeMCU**, and at that moment **NodeMCU** fires the DIO pin connecting it to STM to notify STM that there is a Flashing New Application Request
    
      When **STM** senses the event, it sends back Message to NodeMCU as a Notification that it's now available to receive the new **Marker**, at that moment **NodeMCU** fetches the **Marker** from **Firebase** and sends it STM, then wait for its response
    
      When **NodeMCU** receives Marker Response from STM, it checks against it, Positive Response means that the STM would proceed the flashing sequence, and Negative Response means that STM wouldn't proceed as the application already exists.
    
    * If **SendRQT** is True and **ResponseRQT** is True, this means that **NodeMCU** will receive Non-Data Command which it either **Erase Command** or **Verify Command**, these types of commands waits for response.
    
      So when it receives the command, **NodeMCU** sends it to **STM** and waits for the Response, when it receives Response it uploades the **Frame** to **Firebase** and set **Send** to false as an indicator that **PC Application** has something new to read.
    
    * If **SendRQT** is True and **ResponseRQT** is False, this means that **NodeMCU** will receive Data Command, this types of commands doesn't wait for response, and set **Send** to false as an indicator that **PC Application** has something new to read.
    
      So when it receives the command, **NodeMCU** sends it to **STM** and waits for **ResponseRQT** to be True -waits for **Verification Command**- , when it receives Response it Verification Command uploades the **Frame** to **Firebase** and set **Send** to false as an indicator that **PC Application** has something new to read.    

## The Errors Causes and Solutions 

### 1- ArduinoJson library not exist 

You should install ArduinoJson version 5.13.5 not the latest version.

### 2- Exception(9) and (28) 

![](/Gateway_Node/Images/6.jpg)

We Started with looking up exception code in the Exception Causes(EXCCAUSE) table to understand what kind of issue it is. We have no clues what it’s about and where it happens, so we used Arduino ESP8266/ESP32 Exception Stack Trace Decoder to find out in which line of application it is triggered.
After a lot of search and trying many solutions we discovered that the problem was because of some issues in the library we use at (2.1) step (1) so we used Firebase real-time database Arduino library for ESP8266 it’s Google's v 2.9.0, we used it with using the first library 

Steps for using it :
1. Instalation Using Library Manager At Arduino IDE, go to menu Sketch -> Include Library -> Manage Libraries..
   In Library Manager Window, search "firebase" in the search form then select "Firebase ESP8266 Client". Click "Install" button.
2. We added this line to our code and we started to use it in each Firebase API’s       
   
   ```ino
   // Declare the Firebase Data object in the global scope 
      FirebaseData firebaseData; 
   ```
Then we replaced our code with the new way using this object, for Example:

![](/Gateway_Node/Images/7.jpg)

### 3- Exception(29)

After reading it from the exception table and tring many unuseful solutions like using ArduinoJson library we found that when we used firebase database library as solution for problem (3.2), this library has buffer for each object and we should use those lines of code to change the size of the buffer corresponding to our data to avoid data corruption  
 

```ino
/* Optional, set the size of BearSSL WiFi to receive and transmit buffers */ 
       firebaseData.setBSSLBufferSize(4000, 4000); //minimum size is 4096 bytes, maximum size is 16384 bytes
```

```ino
/* Optional, set the size of HTTP response buffer */
       firebaseData.setResponseSize(4000); //minimum size is 400 bytes
```

And we also added those lines of code to avoid any watchdog timer issues 
 
```ino
   Add in setup()
   ESP.wdtDisable(); 
   ESP.wdtEnable(WDTO_8S);

   And in loop()
   ESP.wdtFeed();
```

### 4- Corrupted Data 

In our design, we have two kind of frames:
1. Non Data frame : it contains 8 bytes of data that includes the needed information about the Elf file.
2. Data frame : it contains 1600 bytes of data from the hex file.

As our project sequence is that the Pc send 8 bytes (Non Data frame) to the cloud in hex format as string and we should receive 8 bytes from it, we discovered after receiving it, that the data size is 16 bytes not 8 bytes, that’s because each byte of Data is represented as 2 string digits in hex format so we needed an algorithm to convert back the 16 bytes (digits) string to the original 8 bytes Data, then send those 8 Data bytes to the targeted MCU.
![](/Gateway_Node/Images/8.jpg)

To receive the data frame we received it as a string of 3200 hex string digit (the pc sends 200 frames each one of them is 8 bytes so the total number is 1600 byte and we receive those 1600 bytes multiplied by 2 because of each byte is represented as 2 digits in hex format as string so the numbe is 3200 ), we created (TX_string_buffer) and allocated it with 3200 char (3.2k bytes) becouse if it's not initialized with the size corresponding to our data, it will be initialized with the default size and it will corrapt our data

```ino
String TX_string_buffer = "00000…………”
```
Then we had a problem of sending the frames as an array from the PC as firebase Arduino library for esp didn't support receiving an array So we turned to receiving it as string as the size of string in the firebase Arduino library is big and the firebase can also receive and send a large sequence of string
Another problem is that when we receive the string frames on arduino we want to convert the frames first from string to hex values and to parse the hex values to be added into its place in the Txbuffer[8], so we used the functions strtoul to convert the string and the function substring to parse the string into sizes of 1 byte to be added to the buffer, but the problem was that the function strtoul was taking input parameter as a pointer to constant character
so we had to use an Arduino function c_str() to convert the string to pointer to constant character then the function substring to parse the string into characters in size of 1 byte then convert them using strtoul to be added to the Txbuffer[index] and finally we send this Data over UART to STM

```ino 
 Firebase.getString(firebaseData, "Frame");
 TX_Need_Resp_Buffer = firebaseData.stringData();
 
 TxBuffer[0] = strtoul(TX_Need_Resp_Buffer.substring(0,2).c_str()  ,NULL,16);
     .             
     .	 
 TxBuffer[7] = strtoul(TX_Need_Resp_Buffer.substring(14,16).c_str(),NULL,16);
 
 for(int index=0;index<8;index++)
    {
              Serial.write(TxBuffer[index]);
    }
```
 
## References

1. https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/
2. https://www.javatpoint.com/iot-project-google-firebase-nodemcu
3. https://arduino-esp8266.readthedocs.io/en/latest/exception_causes.html
4. https://github.com/me-no-dev/EspExceptionDecoder
5. https://arduino-esp8266.readthedocs.io/en/latest/faq/a02-my-esp-crashes.html#watchdog
6. https://github.com/mobizt/Firebase-ESP8266/blob/master/README.md
7. https://lastminuteengineers.com/esp8266-nodemcu-arduino-tutorial/
