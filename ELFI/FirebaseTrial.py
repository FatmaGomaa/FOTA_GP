# This Python file uses the following encoding: utf-8

import pyrebase
import time

print("iam in")

ResponseStartIDX=4   #holds where the response in response command is stored
ResponseEndIDX=6

SectionsCountStartIDX=4   #holds where the response in response command is stored
SectionsCountEndIDX=6


#Response Values according to TP
R_OK=0
R_NOT_SectionOffestViolation=1
R_NOT_SectionsOutOfScope=2
R_NOT_MismatchData=3
#FrameNames=["Data1" ,"Data2", "Data3","Data4","Data5","Data6","Data7","Data8","Data9","Data10" ];
IDX=0               ##holds instant index in file
LastSavedIDX=0      ##hold last valid index in file
SentDataBlocks=0    ##Number of valid sent sections
SectionsCount=0     ##Total Number of Sections to send
DataToSend=""
DataBlocks=0
timeOutCounter=0
f=open("tst.txt","r+")
f.seek(0)
lines = f.readlines()
f.close()

print("iam in 2")

SectionsCount = hex( int( lines[1][SectionsCountStartIDX : SectionsCountEndIDX] , 16) )
print("SectionsCount: ")
print(SectionsCount)

config = {
  "apiKey": "AIzaSyC9FyOjWEj58gdhC12SJ8G3jjn3Xd5qfrM",
  "authDomain": "fota-6b4d9.firebaseapp.com",
  "databaseURL": "https://fota-6b4d9.firebaseio.com",
  "projectId": "fota-6b4d9",
  "storageBucket": "fota-6b4d9.appspot.com",
  "messagingSenderId": "834511519114",
  "appId": "1:834511519114:web:0a7c6d6471f204de77dc73",
  "measurementId": "G-G7F80R2SWN"
    }
#FireBase Initialization     
firebase = pyrebase.initialize_app(config)
db = firebase.database()

#db.child("NodeMCUs").set('No_NodeMCUs_connected')

result = db.child("NodeMCUs").get()
if (result.val() ==  "No_Target_Connected"):
  db.child("NodeMCUSemaphore").set(False)


#Notification with New App
while True:
    result = db.child("FlashNewApp").get()
    if (result.val() == False ):
        break 
db.child("FlashNewApp").set(True)

f=open("SelectedTarget.txt","r+")
f.seek(0)
SelectedTarget = f.readline()
f.close()
Target = SelectedTarget.split("\n")
db.child("SelectedGateway").set(Target[0])


#Sending Marker Command
Line = lines[IDX].split("\n")
db.child("Marker").set(Line[0])
db.child("MarkerRQT").set(True)


#wait until MarkerRQT to be false
while True:
    result = db.child("MarkerRQT").get()
    if (result.val() == False ):
        break
    timeOutCounter +=1
    if (timeOutCounter == 100):
      f=open("progress.txt","r+")
      #f.seek(0,2)
      f.seek(0)
      f.write('0 0')
      f.write('\n')
      f.write("No_Target_Connected")
      f.close()
      timeOutCounter=0
      
result = db.child("Marker").get()      
if ( result.val() == "Same_Marker" ):
  print("Same_Marker")
  f=open("progress.txt","r+")
  progress_content =f.readlines()
  progress_content[1]="Same_Marker"
  f.seek(0)
  for i in range(2):
    f.write(progress_content[i])
  #f.write('\n')
  #f.write("Same_Marker")
  f.close()

while True: 
  if ((timeOutCounter != 100) and ( result.val() != "Same_Marker" )):
    break
    
print("break the while")

IDX = IDX + 1
print(IDX)

#Sending Erase Command
Line = lines[IDX].split("\n")
db.child("Frame").set(Line[0])
db.child("ResponseRQT").set(True)
db.child("Send").set(True)              ##NOTE: Change to TRUE

#listen to Erase Command Response
while True:
    result = db.child("Send").get()
    if (result.val() == False ):
        break
print("iam in 3")
ResponseCommand = db.child("Frame").get()
ResponseCommand = ResponseCommand.val()
Response        = hex( int( ResponseCommand[ResponseStartIDX : ResponseEndIDX] , 16 ) )
db.child("ResponseRQT").set(False)

if Response == hex(R_OK) :

    f=open("./progress.txt","r+")
    f.seek(0)
    f.write(str(int(SectionsCount,16)) +" "+ str(SentDataBlocks) ) 
    f.seek(0)
    progress_content=f.readlines()
    progress_content[1]='Erase_Done'
    f.seek(0)
    for i in range(2):
      f.write(progress_content[i])
    f.close()
  
   #f=open("./progress.txt","w")
   #f.seek(0)
   #f.write(str(int(SectionsCount,16)) +" "+ str(SentDataBlocks) )
   #f.write('\n')
   #f.write(0)
   #f.close()
    
    IDX += 1 
    print(IDX)
    while hex(SentDataBlocks) < SectionsCount :
        #send 200 Data Frame
        for i in range(200) :
            #print(IDX)
            Line = lines[IDX].split("\n")
            DataToSend += Line[0]
            print(Line[0])
            IDX += 1 
        print("200 frames")
        #db.child("Data Frames").child(FrameNames[DataBlocks]).set(DataToSend)
        db.child("Frame").set(DataToSend)
        DataBlocks += 1
        #Raise ResponseRQT if it's Verification Command
        if DataBlocks == 1:
          DataBlocks=0
          #Verification Command
          db.child("Send").set(True)
          #Verification Command
          while True:
            result = db.child("Send").get()
            if (result.val() == False ):
              break
          Line = lines[IDX].split("\n")
          db.child("Frame").set(Line[0])
          db.child("ResponseRQT").set(True)
          db.child("Send").set(True)
          IDX += 1
          while True:
            result = db.child("Send").get()
            if (result.val() == False ):
              break
          #Verification Command Response Analyse
          ResponseCommand = db.child("Frame").get()
          ResponseCommand = ResponseCommand.val()
          Response        = hex( int( ResponseCommand[ResponseStartIDX : ResponseEndIDX] , 16 ) )
          db.child("ResponseRQT").set(False)
  
          if( Response == hex(R_OK) ):
              SentDataBlocks +=1
              print(IDX)
              LastSavedIDX += IDX
              print("iam in 4")
              f=open("./progress.txt","r+")
              f.seek(0)
              f.write(str(int(SectionsCount,16)) +" "+ str(SentDataBlocks) ) 
              f.seek(0)
              progress_content=f.readlines()
              progress_content[1]='Block_hasBeen_sent'
              f.seek(0)
              for i in range(2):
                f.write(progress_content[i])
              f.close()              
              #f=open("./progress.txt","w")
              #f.write(str(int(SectionsCount,16)) +" "+ str(SentDataBlocks) )
              #f.close()
          else:
              IDX = LastSavedIDX
        DataToSend=""
        
         
    #db.child("FlashNewApp").set(False)
else:
    print("Erase Failed and Flash operation Stopped")


   