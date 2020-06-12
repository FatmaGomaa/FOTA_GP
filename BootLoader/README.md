# Purpose 

This document describes the sequence of the Bootloader Application and the Flashing commands (Hint Before building the project At the End to Document)

## Sequence of Bootloader Application

<kbd>![](/Images/BL_FlowChart_2.jpg )</kbd>


## Sequence of Flashing Commands 

### ID_EraseCommand

1. We receive by tprotocol_ReceiveFrame the buffer and the erase command and the message id to check that the mcu received the command correctly
2. Then we calculate checksum of that frame to check that we received the frame bytes correctly
3. Check if the EraseCheckSum (in the frame parsed) is equal to EraseCommand.CheckSum Erase (that has been calculated)
4. We enter a while loop to erase the amount of sections stated in EraseCommand.SectionCount and also check on the ErrorCounter if its less than MAXERRORCOUNT
5. We check the local error returned from the Flash_ErassPage and here we erase the memory sections given by the sender     
	  ```
     Local_Error = Flash_ErassPage( MAINADDRESS + (EraseCommand.SectionOffset) + ( PAGESIZE * SectionCount) );
	  ```
6. An error counter is used to check for error timeout if the code is stuck on local error not_ok to return not_ok
7. Check if local_error is ok set the response in the responseCommand = R_OK;
8. Check if local_error is not_ok set the response in the responseCommand = R_NOT_EraseFailure;
9. Fill the transmitterBuffer in the TProtocol_SendFrame with the responseCommand and send it through UART to the Sender with the responseFrame
10. if the Checksum of the EraseFrame is not equal to the EraseCommand.Checksum then set the response in responseCommand = R_NOT_MismatchData 
11. Send the ResponseFrame to the TProtocol_SendFrame to fill the TransmitterBuffer and send it through UART to the Sender

### ID_DataCommand

1. We receive by tprotocol_ReceiveFrame the buffer and the DataCommand and the message id to check that the mcu received the command correctly
2. Receive the data from the DataCommand.Data[FrameBytes] in the DataBytes[DataIterator]
3. Calculate the Checksum for the Data send in the DataCommand.Data[FrameBytes]

### ID_VerifyCommand

1. We receive by tprotocol_ReceiveFrame the buffer and the VerifyCommand and the message id to check that the mcu received the command correctly
2. Check if the DataCheckSum is equal VerifyCommand.CheckSum 
   - Set the response in the responseCommand = R_OK
   - Enable PRIMASK to stop the interrupts (make a critical section)
   - Start Writing the Data Bytes received to the Flash Memory at the specified offset for each page size (the offset is incremented each page size flashed)
   - Disable the PRIMASK to end the critical section
3. check if the DataCheckSum is  not equal VerifyCommand.CheckSum 
   - Set the response in the responseCommand = R_NOT_MismatchData
4. Send the ResponseFrame to the TProtocol_SendFrame to fill the TransmitterBuffer and send it through UART to the Sender
5. Check if all the required sections are filled with data Erase the FlashedAppVersion, flash the RequestedAppVersion(RAM) to the FlashedAppVersion (ROM) and set Marker to High

### ID_MarkerCommand

1. We receive by tprotocol_ReceiveFrame the buffer and the MarkerCommand and the message id to check that the mcu received the command correctly
2. Check if the Marker(in ROM) is equal MarkerCommand.marker(from NodeMcu)  
   - Set the response in the responseCommand = R_NOT_SAMEAPPLICATION
   - Fill the transmitterBuffer in the TProtocol_SendFrame with the responseCommand and Send it through UART to the Sender with the responseFrame
3. Else if the Marker(in ROM) is not equal MarkerCommand.marker(from NodeMcu)
   - Update value of RepuestedAppVersion(RAM) by the value of MarkerCommand.marker(from NodeMcu)
   - Set the response in the responseCommand = R_OK
   - Fill the transmitterBuffer in the TProtocol_SendFrame with the responseCommand and send it through UART to the Sender with the responseFrame
   - To Change the marker value to LOW We must erase it's page first and as it's with the FlashedAppVersion var in same section we will store FlashedAppVersion in temp then erase the page to update the Marker value 
     1. Copy the value in FlashedAppVersion to LocalTemp
     2. Erase page of the FlashedAppVersion 
     3. Return Value from local temp to FlashedAppVersion
     4. Set Marker By LOW


## Hint

Before building our project we includes all folders that we need like (Application, RCC_Handler, USART_Handler, Libraries, ARM_Delay, FLASH, GPIO, RCC, USART, TransportProtocol ) and (RCC, LIB, GPIO) in Main_App

### Steps

1. Right click on project and choose properties
2. Click on settings
3. Choose GNU ARM cross C compiler 
4. Click on includes 
5. click on ADD icon
6. Select Folders we want to include by clicking on WorkSpace 

<table>
 <p align="center">
 <kbd> <img img src="/Images/1.jpg"  /> </kbd>
 <kbd> <img img src="/Images/2.jpg"  /> </kbd>
</p>
</table>
	
<table>
<p align="center">
  <kbd> <img img src="/Images/3.jpg"   /> </kbd>
  <kbd> <img img src="/Images/4.jpg"  /> </kbd>
</p>
</table>

<table>
<p align="center">
  <kbd> <img img src="/Images/5.jpg" /> </kbd>
 <kbd> <img img src="/Images/6.jpg" /> </kbd>
</p>
</table>
