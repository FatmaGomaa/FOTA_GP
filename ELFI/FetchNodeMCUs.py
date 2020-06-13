import pyrebase
import time

config = {
  "apiKey": "AIzaSyCLVYhUGxHVeIfLW8DkWPZy7kBu4f1-79o",
  "authDomain": "fota-905e1.firebaseapp.com",
  "databaseURL": "https://fota-905e1.firebaseio.com",
  "projectId": "fota-905e1",
  "storageBucket": "fota-905e1.appspot.com",
  "messagingSenderId": "746270454506",
  "appId": "1:746270454506:web:32183900e0176d27b31d93",
  "measurementId": "G-ZE9JE4FGRP"
    }
    
#FireBase Initialization     
firebase = pyrebase.initialize_app(config)
db = firebase.database()

while True:
    result = db.child("NodeMCUs").get()
    f=open("NodeMCUs.txt","w")
    f.write(result.val())
    f.close()
    