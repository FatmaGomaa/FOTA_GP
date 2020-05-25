# Serial Port Programming using Win32 API
The software is written using C language and communicates with the Serial Port using Win32 API.

In Windows ,Serial ports are named as COM1,COM2 ,COM3.. etc .COM1 and COM2 usually refer to the hardware serial ports present in the PC.

## Finding out your COM port Number
To find out the COM number corresponding to your serial port:
1. Open **Device Manager** by right clicking on My Computer icon
2. select **Manage** → **Device Manager**.
3. Under Ports(COM & LPT) you can see the parallel and serial ports (COM) detected by your system.
![](/Images/KnowingPortExample.png)


## Opening and Closing a Serial Port
In Windows we use the CreateFile() function to open a serial port.
**CreateFile()** is a Win 32 function which is used to create or open a file, stream or an IO device like serial port.On success CreateFile() will return a handle which is then used to refer the connection in all subsequent operations.

After opening a serial port using the CreateFile() function you should close it with **CloseHandle()** function, otherwise port will become unavailable to other programs.

```
HANDLE hComm;

  hComm = CreateFile(“\\\\.\\COM24”,                   //port name remove "\\\\.\\" if it's less the COM 10
                      GENERIC_READ | GENERIC_WRITE,   //Read/Write
                      0,                              // No Sharing
                      NULL,                           // No Security
                      OPEN_EXISTING,                  // Open existing port only
                      0,                              // Non Overlapped I/O
                      NULL);                          // Null for Comm Devices
```

**CreateFile()** function takes 7 arguments,

1. **Name of the serial port** to be opened here \\\\.\\COM24.
2. Mode of **access**, here Read and Write
3. Sharing options, Serial ports can't be shared so 0
4. NULL for Serial ports, used for File operations
5. Open the existing port, OPEN_EXISTING
6. Overlapped IO or Non overlapped IO, here 0 means we are using NonOverlapped IO. Overlapped IO is used for multithreaded programs where
several threads can interact with the port simultaneously.
7. NULL for Serial port, used for file operations

If the function succeeds in opening the serial port, it returns a valid handle to hcomm which is then used for error checking.

## Configuring the DCB Structure

In Windows ,settings like Baud rate ,Number of start/Stop bits,data formats etc for the serial port are controlled by the DCB structure.

To Configure the DCB structure we use,
**SetCommState()** function which configures the serial port with the new values in DCB structure provided by us.

First you declare a new structure of type DCB and initializes it.
```
DCB dcbSerialParams = { 0 }; // Initializing DCB
.
.
.
dcbSerialParams.BaudRate = CBR_9600;          // Setting BaudRate = 9600
dcbSerialParams.ByteSize = 8;                 // Setting ByteSize = 8
dcbSerialParams.StopBits = ONESTOPBIT;        // Setting StopBits = 1
dcbSerialParams.Parity   = NOPARITY;          // Setting Parity = None

```

## Writing Data to Serial Port
```

char lpBuffer[] = "Hello from Win32";
DWORD dNoOFBytestoWrite;                          // No of bytes to write into the port
DWORD dNoOfBytesWritten = 0;                      // No of bytes written to the port
dNoOFBytestoWrite = sizeof(lpBuffer);

Status = WriteFile(hComm,                         // Handle to the Serial port
                   lpBuffer,                     // Data to be written to the port
                   dNoOFBytestoWrite,            //No of bytes to write
                   &dNoOfBytesWritten,          //Bytes written
                   NULL);
```

Writing data to the opened serial port is accomplished by the **WriteFile()** function. **WriteFile()** function can be used to write both into the files and I/O ports.

## Reading from the Serial Port
Reading from the serial port is accomplished by the ReadFile() function.

**ReadFile()** , setting up an event and let windows notify us when a character is received is the methodology we are going to use.
1. **Create an Event** for a particular action like character reception.
2. Ask windows to wait for the event set by **SetCommMask()** function using **WaitCommEvent()** and notify us when the condition happens.
3. Call **ReadFile ()** to read the received data from the Serial port.

**SetCommMask** 
```
BOOL SetCommMask(
  HANDLE hFile,
  DWORD  dwEvtMask
);
```
Specifies a set of events to be monitored for a communications device, it takes a handle to the communications device and The events to be enabled.
```
Status = SetCommMask(hComm, EV_RXCHAR);
```

**WaitCommEvent** 
```
BOOL WaitCommEvent(
  HANDLE       hFile,
  LPDWORD      lpEvtMask,
  LPOVERLAPPED lpOverlapped
);
```
Waits for an event to occur for a specified communications device. The set of events that are monitored by this function is contained in the event mask associated with the device handle.

It takes A handle to the communications device, A pointer to a variable that receives a mask indicating the type of event that occurred and A pointer to an OVERLAPPED structure.
```
DWORD dwEventMask; 
Status = WaitCommEvent(hComm, &dwEventMask, NULL);  
```
**dwEventMask** contains a hex value indicating the event, which has caused WaitCommEvent() to return.

After **WaitCommEvent()** has returned, call **ReadFile()** function to read the received characters from the Serial Port Buffer

**ReadFile**
```
BOOL ReadFile(
  HANDLE       hFile,
  LPVOID       lpBuffer,
  DWORD        nNumberOfBytesToRead,
  LPDWORD      lpNumberOfBytesRead,
  LPOVERLAPPED lpOverlapped
);
```
takes:
1. A handle to the device.
2. A pointer to the buffer that receives the data read from a file or device.
3. The maximum number of bytes to be read.
4. A pointer to the variable that receives the number of bytes read when using a synchronous hFile parameter.
5. A pointer to an OVERLAPPED structure is required if the hFile parameter was opened with FILE_FLAG_OVERLAPPED, otherwise it can be NULL.


```
char TempChar; //Temporary character used for reading
char SerialBuffer[256];//Buffer for storing Rxed Data
DWORD NoBytesRead;
int i = 0;

do
 {
   ReadFile( hComm,                         //Handle of the Serial port
             &TempChar,                     //Temporary character
             sizeof(TempChar),              //Size of TempChar
             &NoBytesRead,                  //Number of bytes read
             NULL);

   SerialBuffer[i] = TempChar;              // Store Tempchar into buffer
   i++;
  }

while (NoBytesRead > 0);
```


## Serial Communication Between PC and STM32 using TTL
![](/Images/ttlConnection.jpg)
## References:
```
https://docs.microsoft.com/en-us/windows/win32/api/
https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
```
