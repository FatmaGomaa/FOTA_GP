import pyrebase
import time

f=open("tst.txt","r+")
lines = f.readlines()
f.close()

config = {
    "apiKey": "AIzaSyDnycEG7ipMx6wts4g7ms9Follqgjbj6YI",
    "authDomain": "fota-project.firebaseapp.com",
    "databaseURL": "https://fota-project.firebaseio.com",
    "projectId": "fota-project",
    "storageBucket": "fota-project.appspot.com",
    "messagingSenderId": "860459599123",
    "appId": "1:860459599123:web:f4d700235b3f13b1c5c6e6",
    "measurementId": "G-LQ0651W1PH"
    }
    
firebase = pyrebase.initialize_app(config)
#auth = firebase.auth()
    
#email = input("please enter your Email : ")
#password = input("please enter your password :")
    
#user = auth.create_user_with_email_and_password(email ,password)
#print("user created ")

#signin = auth.sign_in_with_email_and_password(email ,password)
#print("sigin done ")

#auth.send_email_verification(signin['idToken'])
#print("email verification has been sent")

#auth.send_password_reset_email(email)
#print("check your mail")

db = firebase.database()

#data = {"name":"Donia"}
 
#db.child("users").push(data)
#print("Data added to real time database ")

#db.child("users").child("OwnKey").set(data)

#db.child("users").child("OwnKey").update({"name":"Fota"})
#print("Data updated successfully ")
 
#db.child("users").child("-M8btWxXfbDwrzVMutQM").update({"name":"Updated Name"})
#print("Data updated successfully ")

#db.child("header").child("data").set(line)
#print("done")

users = db.child("header").child("data").child("0").get()
print(users.val())

#all_users = db.child("users").get()
 
#for users in all_users.each():
#    print(users.val())
#    print(users.key())
#    
#db.child("users").child("-M8btWxXfbDwrzVMutQM").remove()
#print("User removed")

#for i in lines:
#  line=i.split("\n")
#  db.child("header").child("data").set(line)
#  time.sleep(2)
  #print(i)
  #print("\n")