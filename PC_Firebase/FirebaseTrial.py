import pyrebase
import time

ResponseStartIDX=4   #holds where the response in response command is stored
ResponseEndIDX=6

SectionsCountStartIDX=4   #holds where the response in response command is stored
SectionsCountEndIDX=6


#Response Values according to TP
R_OK=0
R_NOT_SectionOffestViolation=1
R_NOT_SectionsOutOfScope=2
R_NOT_MismatchData=3

IDX=0               ##holds instant index in file
LastSavedIDX=0      ##hold last valid index in file
SentDataBlocks=0    ##Number of valid sent sections
SectionsCount=0     ##Total Number of Sections to send

f=open("tst.txt","r+")
lines = f.readlines()
f.close()


SectionsCount = hex( int( lines[0][SectionsCountStartIDX : SectionsCountEndIDX] , 16) )
#print(SectionsCount)

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

#Notification with New App
db.child("FlashNewApp").set(True)

#Sending Erase Command
Line = lines[IDX].split("\n")
db.child("Command").set(Line[0])
db.child("ResponseRQT").set(True)
db.child("Send").set(True)              ##NOTE: Change to TRUE

#listen to Erase Command Response
while True:
    result = db.child("Send").get()
    if (result.val() == False ):
        break

ResponseCommand = db.child("Command").get()
ResponseCommand = ResponseCommand.val()
Response        = hex( int( ResponseCommand[ResponseStartIDX : ResponseEndIDX] , 16 ) )
db.child("ResponseRQT").set(False)

if Response == hex(R_OK) :
    f=open("progress.txt","w")
    f.write(str(int(SectionsCount,16)) +"\t" + str(SentDataBlocks) )
    f.close()
    IDX += 1 
    while hex(SentDataBlocks) < SectionsCount :
        #send 200 Data Frame + Verification Frame
        for i in range(201) :
            Line = lines[IDX].split("\n")
            db.child("Command").set(Line[0])
            db.child("Send").set(True) 
            
            #Raise ResponseRQT if it's Verification Command
            if(i==200):
                db.child("ResponseRQT").set(True)

            while True:
                result = db.child("Send").get()
                if (result.val() == False ):
                    break
            IDX += 1 

        #Verification Command Response Analyse
        ResponseCommand = db.child("Command").get()
        ResponseCommand = ResponseCommand.val()
        Response        = hex( int( ResponseCommand[ResponseStartIDX : ResponseEndIDX] , 16 ) )
        db.child("ResponseRQT").set(False)

        if( Response == hex(R_OK) ):
            SentDataBlocks +=1
            LastSavedIDX += IDX
            f=open("progress.txt","w")
            f.write(str(int(SectionsCount,16)) +"\t" + str(SentDataBlocks) )
            f.close()
        else:
            IDX = LastSavedIDX

else:
    print("Erase Failed and Flash operation Stopped")


   