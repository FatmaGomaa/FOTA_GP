# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'gui_v1laLmTs.ui'
##
## Created by: Qt User Interface Compiler version 5.14.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide2.QtCore import (QCoreApplication, QMetaObject, QObject, QPoint,
    QRect, QSize, QUrl, Qt ,SIGNAL)
from PySide2.QtGui import (QBrush, QColor, QConicalGradient, QFont,
    QFontDatabase, QIcon, QLinearGradient, QPalette, QPainter, QPixmap,
    QRadialGradient)
from tkinter import *
from PySide2.QtWidgets import *
from PySide2 import QtWidgets
from PySide2 import QtCore
import sys
import os
import subprocess 
import time
import serial.tools.list_ports as port_list
import re
import threading
from subprocess import call
from PySide2.QtGui import *
##################################################################################
filename = ()

##We use this function to define the QString as String
def QString():
  str()
  return

##ProgressBar Thread Class
class Progress(QtCore.QThread):
    ##The initialization function 
    def __init__(self):
      QtCore.QThread.__init__(self)
      
    ##The runnable of ProgressBar
    def run(self):
      ## Reset progressbar value by zero 
      self.emit(QtCore.SIGNAL('__updateProgressBar(int)'), 0) 
      ## To read the modification file of progressBar
      file_in = "./progress.txt"

      ##Waiting for 0.1sec before start updating the progressBar
      time.sleep(0.1)
      while True:
        ##opening the progress file to update the progressBar
        with open(file_in) as f:
          line = f.readline()
          ##split the line using space
          Line_Array= line.split()

          ##Check if the value not updated so we will update StatusValue and progressBar
          if (int (float(Line_Array[1])) == 0):
            progressValue=0;
            
          ##In case flashing done  
          elif (Line_Array[1] == Line_Array[0]):
            progressValue=100
            ##will terminate the Thread
            self.exit()

          ## In case flashing the c app  
          else:
            progressValue = (int (float(Line_Array[1])) /int (float(Line_Array[0]))) *100
            
          ##updating the progressBar  
          self.emit(QtCore.SIGNAL('__updateProgressBar(int)'),progressValue)
          ##pending the Thread for 2Sec
          time.sleep(2)
              
##ComReceiver Thread Class  
class Import(QtCore.QThread):
  ##The initialization function 
  def __init__(self,MCU,ELFPath):
      QtCore.QThread.__init__(self)
      self.MCU=MCU
      self.ELFPath=ELFPath
  
  ##The runnable of ComReceiver c file
  def run(self):
    ##Files needed to be executed
    file_in = "./CommReceive.c"
    Dependences= "./TProtocol.c"
    print ("Hey this is Python Script Running\n")
    ##for checking MCU, COM_number and Elf_path
    print(self.MCU)
    print(self.ELFPath)
    ##Compiling C files
    subprocess.call(["gcc",file_in,Dependences])
    
    print("before")
    ##To run the executable file with input arguments ("AVR comN ElfPath")
    subprocess.call("./a.exe" + " " + self.ELFPath)
    print("After")
    
    print("python script")  
    call(["python", "FirebaseTrial.py"])
    ##pending the Thread for 2Sec  
    time.sleep(2)
      #self.exit()
         


class Ui_ELFI(object):
    def setupUi(self, ELFI):
        if ELFI.objectName():
            ELFI.setObjectName(u"ELFI")
       
        ELFI.setFixedSize(690, 440)
        #root = Tk() 
        #root.title("Melody")
        #root.mainloop()
        icon = QIcon()
        icon.addFile(u"./Pic/elfi.ico", QSize(), QIcon.Normal, QIcon.Off)
        ELFI.setWindowIcon(icon)
        self.BG_Image = QLabel(ELFI)
        self.BG_Image.setObjectName(u"BG_Image")
        self.BG_Image.setGeometry(QRect(-10, 0, 701, 441))
        self.BG_Image.setPixmap(QPixmap(u"Pic/12.jpg"))
        self.BG_Image.setScaledContents(True)
        
        self.BrowseButton = QPushButton(ELFI)
        self.BrowseButton.setObjectName(u"BrowseButton")
        self.BrowseButton.setGeometry(QRect(510, 240, 120, 31))
        
        self.BrowseButton.setStyleSheet(" QPushButton {color: white; background-color: rgb(13, 156, 141 , 0.5 ) ; border-style: outset;  border-color: black;  border-radius: 10px;  font: bold 14px;  min-width: 5em;  padding: 2px;} ")
        
        
        
        self.progressBar = QProgressBar(ELFI)
        self.progressBar.setObjectName(u"progressBar")
        self.progressBar.setGeometry(QRect(60, 380, 570, 31))
        
        self.progressBar.setStyleSheet( "  QProgressBar       {   padding:2px;  border-radius: 10px;     font: bold 12px;    border: 1px solid black;  text-align: center; }"   
                                      "  QProgressBar::chunk  {  background-color:rgb(13, 156, 141) ;  border-radius: 5px;} "   )
        
        
        self.FileLocation = QLineEdit(ELFI)
        self.FileLocation.setObjectName(u"FileLocation")
        self.FileLocation.setGeometry(QRect(60, 240, 431, 31))
        
        self.FileLocation.setStyleSheet( "  QLineEdit {border-radius: 10px;     font: bold 12px;    border: 1px solid black; }")
        
        self.UploadButton = QPushButton(ELFI)
        self.UploadButton.setObjectName(u"UploadButton")
        self.UploadButton.setGeometry(QRect(470, 310, 161, 61))
        font1 = QFont()
        font1.setPointSize(15)
        self.UploadButton.setFont(font1)
        
        self.UploadButton.setStyleSheet(" QPushButton {color: white; background-color: rgb(13, 156, 141 , 0.5 ); border-style: outset;  border-color: black;  border-radius: 10px;  font: bold 14px;  min-width: 5em;  padding: 2px;} ")

        
        self.HW_Family = QComboBox(ELFI)
        self.HW_Family.addItem(QString())
        self.HW_Family.addItem(QString())
        self.HW_Family.addItem(QString())
        self.HW_Family.setObjectName(u"HW_Family")
        self.HW_Family.setGeometry(QRect(250, 160, 151, 22))
        
        self.HW_Family.setStyleSheet(" QComboBox {  border-radius: 5px;  } ")
        
        self.ProgressMessage_Title = QLabel(ELFI)
        self.ProgressMessage_Title.setObjectName(u"ProgressMessage_Title")
        self.ProgressMessage_Title.setGeometry(QRect(60, 300, 351, 20))
        self.ProgressMessage_Title.setTextFormat(Qt.RichText)
        self.ProgressMessage_Title.setStyleSheet("QLabel { color:rgb(89, 216, 193 );  font: bold 18px; }")
        self.ProgressMessage_Title.setText("Status")
        
        self.ProgressMessage = QLabel(ELFI)
        self.ProgressMessage.setObjectName(u"ProgressMessage")
        self.ProgressMessage.setGeometry(QRect(60, 320, 351, 40))
        self.ProgressMessage.setTextFormat(Qt.RichText)
        self.ProgressMessage.setStyleSheet("QLabel { color : White;  font: bold 14px; }")
        self.ProgressMessage.setText("Idle")
        
       
        self.Title = QLabel(ELFI)
        self.Title.setObjectName(u"Title")
        self.Title.setGeometry(QRect(190, 40, 271, 61))
        font2 = QFont()
        font2.setPointSize(25)
        self.Title.setFont(font2)
        self.Title.setTextFormat(Qt.RichText)
        self.Title.setAlignment(Qt.AlignCenter)
        
        self.Title.setStyleSheet("QLabel { color:rgb(89, 216, 193 ) }")
        
        self.FileLocation_name = QLabel(ELFI)
        self.FileLocation_name.setObjectName(u"FileLocation_name")
        self.FileLocation_name.setGeometry(QRect(60, 220, 91, 16))
        
        self.FileLocation_name.setStyleSheet("QLabel { color:rgb(122, 243, 221); font: bold 14px; }")
        
        font3 = QFont()
        font3.setPointSize(12)
        self.FileLocation_name.setFont(font3)
        self.fireleft = QLabel(ELFI)
        self.fireleft.setObjectName(u"fireleft")
        self.fireleft.setGeometry(QRect(130, 30, 81, 81))
        self.fireleft.setPixmap(QPixmap(u"Pic/fire.png"))
        self.fireleft.setScaledContents(True)
        
        self.fireright = QLabel(ELFI)
        self.fireright.setObjectName(u"fireright")
        self.fireright.setGeometry(QRect(430, 30, 81, 81))
        self.fireright.setPixmap(QPixmap(u"Pic/fire.png"))
        self.fireright.setScaledContents(True)
        
        self.HardwareFamily_name = QLabel(ELFI)
        self.HardwareFamily_name.setObjectName(u"HardwareFamily_name")
        self.HardwareFamily_name.setGeometry(QRect(265, 135, 121, 21))
        self.HardwareFamily_name.setFont(font3)

        self.HardwareFamily_name.setStyleSheet("QLabel { color:rgb(122, 243, 221); font: bold 14px; }")

        self.retranslateUi(ELFI)

        QMetaObject.connectSlotsByName(ELFI)
    # setupUi

    def retranslateUi(self, ELFI):
        ELFI.setWindowTitle(QCoreApplication.translate("ELFI", u"ELFI", None))
        self.BG_Image.setText("")
        self.BrowseButton.setText(QCoreApplication.translate("ELFI", u"Browse", None))
        self.FileLocation.setPlaceholderText("")
        self.UploadButton.setText(QCoreApplication.translate("ELFI", u"Upload", None))
        self.HW_Family.setItemText(0, QCoreApplication.translate("ELFI", u"STM32", None))
        self.HW_Family.setItemText(1, QCoreApplication.translate("ELFI", u"NXP", None))
        self.HW_Family.setItemText(2, QCoreApplication.translate("ELFI", u"AVR ATmega32", None))

        
        self.Title.setText(QCoreApplication.translate("ELFI", u"HOT Bootloader", None))
        self.FileLocation_name.setText(QCoreApplication.translate("ELFI", u"File Location", None))
        self.fireleft.setText("")
        self.fireright.setText("")
        self.HardwareFamily_name.setText(QCoreApplication.translate("ELFI", u"Hardware Family", None))
        # retranslateUi
        
        ##Connect buttons
        self.BrowseButton.clicked.connect(self.BrowseFiles_handler)
        self.UploadButton.clicked.connect(self.GenerateFunction)

        ##object of progress class
        self.progressView = Progress()
        
    ##Function call when Connect_Button pressed
    def GenerateFunction(self) :
      global filename
      ##checking directory path validation
      if(os.path.isfile(self.FileLocation.text()) == False):
        ErrorBox = QtWidgets.QErrorMessage()
        ErrorBox.showMessage("This File doesn't exist")
        ErrorBox.exec_()
      else:
        ##To get MCU 
        Index = self.HW_Family.currentIndex()

        ##To get the ELF_File
        ElfPath = filename[0]
        ##starting the ComReceiver Thread
        self.file=Import(Index,ElfPath)
        self.file.start()
        ##initalization values og progress.txt
        f = open('./progress.txt','w') 
        f.write("0 0")
        f.close()
        ##starting The progressBar Thread
        QObject.connect (self.progressView , QtCore.SIGNAL("__updateProgressBar(int)"),self.__updateProgressBar)
        self.start()
        print("correct")
          
    
    #get the file location
    def BrowseFiles_handler(self):
        global filename
        filename = QFileDialog.getOpenFileName()
        self.FileLocation.setText(filename[0])

    @QtCore.Slot(int)
    def  __updateProgressBar(self, percent):
      ##initial state
      if percent == 0:
        self.ProgressMessage.setText("Setup")
      elif percent < 100:
        self.ProgressMessage.setText("in progress")
      elif percent == 100:
        self.ProgressMessage.setText("Done")
      self.progressBar.setValue(percent)
    def start(self):
      self.progressView.start()


##To create object of application
app = QApplication(sys.argv)
#to create the object of wedget
Widget=QWidget()
#to create form
ELFI = Ui_ELFI()
#to make setup to GUI and take widget
ELFI.setupUi(Widget)
Widget.show()
#to make GUI execute
app.exec_()
