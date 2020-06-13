import pyrebase
import time

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

while True:
    result = db.child("NodeMCUs").get()
    f=open("NodeMCUs.txt","w")
    f.write(result.val())
    f.close()
    