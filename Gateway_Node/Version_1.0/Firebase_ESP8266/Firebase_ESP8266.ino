//1. Include Firebase ESP8266 library (this library)
#include "FirebaseESP8266.h"

//2. Include ESP8266WiFi.h and must be included after FirebaseESP8266.h
#include <ESP8266WiFi.h>


//3. Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

// Set these to run example.
#define FIREBASE_HOST "fota-905e1.firebaseio.com"
#define FIREBASE_AUTH "Bpdn4xtRL1NG1vP1r2mgb85QpTxbVeWsEYgzpnky"
#define WIFI_SSID "LINKDSL-Dawoud"
#define WIFI_PASSWORD "ilhpfeae3143949"
#define DELAY_TIME    100

/* Definitions */
String TX_string_buffer , TX_Need_Resp_Buffer , RX_string_buffer;

 
unsigned char RxBuffer[8];
unsigned char TxBuffer[8];
char counter =0;
bool SendRQT, ResponseRQT;

void setup() {
  Serial.begin(115200);
//  node.setcpufreq(node.CPU160MHZ);

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
  Firebase.setBool(firebaseData, "FlashNewApp", false );
  Firebase.setBool(firebaseData, "Send", true );
  Firebase.setBool(firebaseData, "ResponseRQT", false );
  Firebase.setString(firebaseData, "Frame", "414507000030074d");
}
void loop()
{ 
 
  bool FlashNewApp_Flag;
  Firebase.getBool(firebaseData, "FlashNewApp");
  FlashNewApp_Flag = firebaseData.boolData();
 /* Check if a new program has been uploaded */
  if (FlashNewApp_Flag == true)
  {
    /* Get the Current Status of the Send and ResponseRQT */
    Firebase.getBool(firebaseData, "Send"); 
    SendRQT = firebaseData.boolData();
    Firebase.getBool(firebaseData, "ResponseRQT");
    ResponseRQT = firebaseData.boolData();

    /* If it is Send and Receive Operation */
    if (SendRQT == true && ResponseRQT == true)
    {
    /* Get the Current  */
    Firebase.getString(firebaseData, "Frame");
    TX_Need_Resp_Buffer = firebaseData.stringData();

    /* Divide the Frame into 8 bytes for UART to be send */
    /*  TxBuffer[0] = strtoul(TX_string_buffer.substring(0,2).c_str()  ,NULL,16);
      TxBuffer[1] = strtoul(TX_string_buffer.substring(2,4).c_str()  ,NULL,16);
      TxBuffer[2] = strtoul(TX_string_buffer.substring(4,6).c_str()  ,NULL,16);
      TxBuffer[3] = strtoul(TX_string_buffer.substring(6,8).c_str()  ,NULL,16);
      TxBuffer[4] = strtoul(TX_string_buffer.substring(8,10).c_str() ,NULL,16);
      TxBuffer[5] = strtoul(TX_string_buffer.substring(10,12).c_str(),NULL,16);
      TxBuffer[6] = strtoul(TX_string_buffer.substring(12,14).c_str(),NULL,16);
      TxBuffer[7] = strtoul(TX_string_buffer.substring(14,16).c_str(),NULL,16);
    */
      /*TODO: Send Data over UART to STM */
     /* for(int index=0;index<8;index++)
      {
        //uart.write(0,TxBuffer[index]);    
         Serial.printf("%02x",TxBuffer[index]);         
      }*/

      /* For testing */
      Serial.print(TX_Need_Resp_Buffer);
      Serial.print("\n");
         
      /* TODO: Receive Data from STM  in RXBuffer*/
      /*for(int index=0;index<8;index++)
      {
       RxBuffer[i] = uart.on();        
      }*/
    
    /*TODO: Concatenate the RxBuffer into string to Send it to FireBase */
    /*RX_string_buffer  = String(RxBuffer[7],HEX);
    RX_string_buffer += String(RxBuffer[6],HEX);
    RX_string_buffer += String(RxBuffer[5],HEX);
    RX_string_buffer += String(RxBuffer[4],HEX);
    RX_string_buffer += String(RxBuffer[3],HEX);
    RX_string_buffer += String(RxBuffer[2],HEX);
    RX_string_buffer += String(RxBuffer[1],HEX);
    RX_string_buffer += String(RxBuffer[0],HEX);
    RX_string_buffer += "\0";*/
      
    /* Put the Response and Set the Send to false in firebase*/
      SendRQT = false;
      Firebase.setString(firebaseData, "Frame", "0000000000000000");
      Firebase.setBool(firebaseData, "Send", SendRQT);

      }
    
    
    /* If it is Send Operation Only */
      else if(SendRQT == true && ResponseRQT == false)
      {
    /* Get the Current Frame */
    TX_string_buffer =" ";
    Firebase.getString(firebaseData, "Frame");
    TX_string_buffer = firebaseData.stringData() + '\0'; 

      /* Divide the Frame into 8 bytes for UART to be send */
      /*  TxBuffer[0] = strtoul(TX_string_buffer.substring(0,2).c_str()  ,NULL,16);
        TxBuffer[1] = strtoul(TX_string_buffer.substring(2,4).c_str()  ,NULL,16);
        TxBuffer[2] = strtoul(TX_string_buffer.substring(4,6).c_str()  ,NULL,16);
        TxBuffer[3] = strtoul(TX_string_buffer.substring(6,8).c_str()  ,NULL,16);
        TxBuffer[4] = strtoul(TX_string_buffer.substring(8,10).c_str() ,NULL,16);
        TxBuffer[5] = strtoul(TX_string_buffer.substring(10,12).c_str(),NULL,16);
        TxBuffer[6] = strtoul(TX_string_buffer.substring(12,14).c_str(),NULL,16);
        TxBuffer[7] = strtoul(TX_string_buffer.substring(14,16).c_str(),NULL,16);
      */
        /* Send Data over UART to STM */
       /* for(int index=0;index<8;index++)
        {
          //uart.write(0,TxBuffer[index]);  
          Serial.printf("%02x",TxBuffer[index]);        
        }*/

        /* For testing */
        long * ptr;
        ptr =(long *) &TX_string_buffer;
        Serial.print("Data length");
        Serial.print("\n");
        Serial.print(TX_string_buffer.length());
        Serial.print("\n");
        Serial.print(TX_string_buffer);
        counter++;
        Serial.print("\n");
        Serial.print(counter);
        
        /*Serial.print(TX_string_buffer);
        Serial.print("\n");*/
         
    /*Set the Send flag on firebase to false */
        SendRQT = false;
        Firebase.setBool(firebaseData, "Send", SendRQT);
     }

  }

  


   
 /* String a;
  int b;
  Firebase.setString(firebaseData, "habda", "414507000030074d"); 
  Firebase.getString(firebaseData, "habda");
  a = firebaseData.stringData();
  Serial.println(a);  
  Firebase.setInt(firebaseData, "habda2", 11); 
  Firebase.getInt(firebaseData, "habda2"); 
  b = firebaseData.intData();
  Serial.println(b);*/ 

}
