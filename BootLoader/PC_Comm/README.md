# Serial Port Programming using Win32 API
The software is written using C language and communicates with the Serial Port using Win32 API.

In Windows ,Serial ports are named as COM1,COM2 ,COM3.. etc .COM1 and COM2 usually refer to the hardware serial ports present in the PC while COM numbers in double digits like COM32,COM54,COM24.. etc are given to USB to Serial Converters or PCI serial port extenders.

## Finding out your COM port Number
To find out the COM number corresponding to your serial port, Open **Device Manager** by right clicking on My Computer icon and selecting **Manage** → **Device Manager**.Under Ports(COM & LPT) you can see the parallel and serial ports (COM) detected by your system.
/*****Port Image Here*****/

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

## References:
```
https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
```
