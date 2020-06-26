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
    Final_List_NodeMCUs = []
    result = db.child("NodeMCUs").get()
    temp = result.val()
    list_NodeMCUs = temp.split()
    
    #put the non similar elements in a list
    for Node in list_NodeMCUs:
        if Node not in Final_List_NodeMCUs :
            Final_List_NodeMCUs.append(Node)
            
    NodestobeUpload = ''
    
    #concatinate the elements in string
    for element in Final_List_NodeMCUs:
        NodestobeUpload += element
        NodestobeUpload += " "
    
    f=open("NodeMCUs.txt","w")
    f.write(NodestobeUpload)
    f.close()
    