# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'GUI.ui'
##
## Created by: Qt User Interface Compiler version 5.14.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

###########################includes#############################################
from PySide2.QtCore import (QCoreApplication, QMetaObject, QObject, QPoint,
    QRect, QSize, QUrl, Qt ,SIGNAL)
from PySide2.QtGui import (QBrush, QColor, QConicalGradient, QFont,
    QFontDatabase, QIcon, QLinearGradient, QPalette, QPainter, QPixmap,
    QRadialGradient)
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
##################################################################################

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
      #loading = 0
      ##Waiting for 0.1sec before start updating the progressBar
      time.sleep(0.1)
      while True:
        ##opening the progress file to update the progressBar
        with open(file_in) as f:
          line = f.readline()
          ##split the line using space
          Line_Array= line.split()
          #f.close()  
          #for Line_Array[1] in range(int(Line_Array[0])):
          ##Check if the value not updated so we will update StatusValue and progressBar
          if (int (float(Line_Array[1])) == 0):
            progressValue=0;
            StatusValue="Setup"
            #time.sleep(1)
          ##In case flashing done  
          elif (Line_Array[1] == Line_Array[0]):
            progressValue=100
            StatusValue="Done"
            ##will terminate the Thread
            self.exit()
            #time.sleep(1)
          ## In case flashing the c app  
          else:
            progressValue = (int (float(Line_Array[1])) /int (float(Line_Array[0]))) *100
            #print(int(progressValue))
            StatusValue="in progress"
            #time.sleep(1)
          ##updating the progressBar  
          self.emit(QtCore.SIGNAL('__updateProgressBar(int)'),progressValue)
          ##pending the Thread for 2Sec
          time.sleep(2)
              
##ComReceiver Thread Class  
class Import(QtCore.QThread):
  ##The initialization function 
  def __init__(self,MCU,COMNUM,ELFPath):
      QtCore.QThread.__init__(self)
      self.MCU=MCU
      self.COMNUM=COMNUM
      self.ELFPath=ELFPath
  
  ##The runnable of ProgressBar
  def run(self):
    ##Files needed to be executed
    file_in = "../PC_Comm/CommReceive.c"
    Dependences= "../PC_Comm/TProtocol.c"
    #while(True):
    print ("Hey this is Python Script Running\n")
    ##for checking MCU, COM_number and Elf_path
    print(self.MCU)
    print(self.COMNUM)
    print(self.ELFPath)
    ##Compiling C files
    subprocess.call(["gcc",file_in,Dependences])
    ##To check MCU Entered
    if(self.MCU == 0):
      print("before")
      ##To run the executable file with input arguments ("AVR comN ElfPath")
      subprocess.call("../PC_Comm/a.exe AVR "+" " +self.COMNUM+" " +self.ELFPath)
      print("After")
    elif(self.MCU == 1):
      ##To run the executable file with input arguments ("ARM comN ElfPath")
      subprocess.call("../a.exe STM "+" " +self.COMNUM+" " +self.ELFPath)
    ##pending the Thread for 2Sec  
    time.sleep(2)
      #self.exit()
              
##Main application  
class Ui_Form(object):
    ##Setup GUI Tool
    def setupUi(self, Form):
      if Form.objectName():
        Form.setObjectName(u"Form")
      Form.resize(444, 279)
      ##background-image
      Form.setStyleSheet(u"\n"
"background-image:url(./18.jpg);\n"
"\n"
"")
      self.MCU_GroupBox = QGroupBox(Form)
      self.MCU_GroupBox.setObjectName(u"MCU_GroupBox")
      self.MCU_GroupBox.setGeometry(QRect(10, 50, 191, 71))
      self.MCU_GroupBox.setStyleSheet(u"")
      self.MCU = QComboBox(self.MCU_GroupBox)
      self.MCU.addItem(QString())
      self.MCU.addItem(QString())
      self.MCU.setObjectName(u"MCU")
      self.MCU.setGeometry(QRect(10, 30, 151, 22))
      self.progressBar = QProgressBar(Form)
      self.progressBar.setObjectName(u"progressBar")
      self.progressBar.setGeometry(QRect(10, 230, 431, 23))
      self.progressBar.setValue(0)
      self.Title = QTextBrowser(Form)
      self.Title.setObjectName(u"Title")
      self.Title.setGeometry(QRect(30, 0, 381, 41))
      self.Title.setStyleSheet(u"background-color: rgb(0, 0, 0);")
      self.Status = QLabel(Form)
      self.Status.setObjectName(u"Status")
      self.Status.setText("Setup")
      self.Status.setGeometry(QRect(10, 210, 151, 16))
      self.FileLocation_GroupBox = QGroupBox(Form)
      self.FileLocation_GroupBox.setObjectName(u"FileLocation_GroupBox")
      self.FileLocation_GroupBox.setEnabled(True)
      self.FileLocation_GroupBox.setGeometry(QRect(10, 120, 421, 80))
      self.FileLocation_GroupBox.setStyleSheet(u"")
      self.Connect_Button = QPushButton(self.FileLocation_GroupBox)
      self.Connect_Button.setObjectName(u"Connect_Button")
      self.Connect_Button.setGeometry(QRect(340, 30, 75, 23))
      self.FileLocation = QLineEdit(self.FileLocation_GroupBox)
      self.FileLocation.setObjectName(u"FileLocation")
      self.FileLocation.setGeometry(QRect(10, 30, 311, 20))
      self.PC_COM_PORT_GroupBox = QGroupBox(Form)
      self.PC_COM_PORT_GroupBox.setObjectName(u"PC_COM_PORT_GroupBox")
      self.PC_COM_PORT_GroupBox.setGeometry(QRect(220, 50, 201, 71))
      self.PC_COM_PORT_GroupBox.setStyleSheet(u"")
      self.pushButton = QPushButton(self.PC_COM_PORT_GroupBox)
      self.pushButton.setObjectName(u"pushButton")
      self.pushButton.setGeometry(QRect(70, 20, 71, 23))
      self.COMEdit = QLineEdit(self.PC_COM_PORT_GroupBox)
      self.COMEdit.setObjectName(u"COMEdit")
      self.COMEdit.setGeometry(QRect(50, 50, 111, 20))

      self.retranslateUi(Form)

      QMetaObject.connectSlotsByName(Form)
    # setupUi

    def retranslateUi(self, Form):
      Form.setWindowTitle(QCoreApplication.translate("Form", u"Form", None))
      self.MCU_GroupBox.setTitle(QCoreApplication.translate("Form", u"MCU", None))
      self.MCU.setItemText(0, QCoreApplication.translate("Form", u"AVR", None))
      self.MCU.setItemText(1, QCoreApplication.translate("Form", u"STM", None))
      
      self.Title.setHtml(QCoreApplication.translate("Form", u"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-size:18pt; font-weight:600; color:#aa557f;\">Welcome to our Bootloader</span></p></body></html>", None))
      self.Status.setText(QCoreApplication.translate("Form", u"Status", None))
      self.FileLocation_GroupBox.setTitle(QCoreApplication.translate("Form", u"FileLocation", None))
      self.Connect_Button.setText(QCoreApplication.translate("Form", u"Send", None))
      self.PC_COM_PORT_GroupBox.setTitle(QCoreApplication.translate("Form", u"PC_COM_port", None))
      self.pushButton.setText(QCoreApplication.translate("Form", u"Search", None))
    # retranslateUi
      
      ##Connect buttons
      self.Connect_Button.clicked.connect(self.GenerateFunction)
      self.pushButton.clicked.connect(self.ComFunction)
      
      ##object of progress class
      self.progressView = Progress()
      
      QMetaObject.connectSlotsByName(Form)
      
    ##Function call when Connect_Button pressed
    def GenerateFunction(self) :
      ##checking directory path validation
      if(os.path.isdir(self.FileLocation.text()) == False):
        ErrorBox = QtWidgets.QErrorMessage()
        ErrorBox.showMessage("This Directory doesn't exist")
        ErrorBox.exec_()
      else:
        #cmd ="Hello.c"
        ##To get MCU 
        Index = self.MCU.currentIndex()
        ##To get ComN
        COMNum=self.COMEdit.text()
        ##To get the ELF_File
        ##TODO:Change elffile_name automatically
        ElfPath = self.FileLocation.text() + "Main_APP.elf"
        ##starting the ComReceiver Thread
        self.file=Import(Index,COMNum,ElfPath)
        self.file.start()
        ##checking directory path validation
        if(os.path.isfile(r'./progress.txt') == False):
          self.Status.setText("Error")
          ErrorBox = QtWidgets.QErrorMessage()
          ErrorBox.showMessage("The progress file doesn't exist")
          ErrorBox.exec_()
        else:
          ##initalization values og progress.txt
          f = open('./progress.txt','w') 
          f.write("0 0")
          f.close()
          ##starting The progressBar Thread
          QObject.connect (self.progressView , QtCore.SIGNAL("__updateProgressBar(int)"),self.__updateProgressBar)
          self.start()
          
          print("correct")
          
                   
    ##Function call when Search_Button pressed
    ##TODO: To get all available coms in list in GUI
    ##searching the COM port automatically
    def ComFunction(self) :
      ports = list(port_list.comports())
      if ports:
        for p in ports:
          tuples=re.findall(r'COM(\d+)',str(p))
          COM= tuples[0] 
          self.COMEdit.setText("COM"+COM)
      if not ports:
        ErrorBox = QtWidgets.QErrorMessage()
        ErrorBox.showMessage("No COM detected")
        ErrorBox.exec_()
  
    ##function called by progressBar Thread to update progressBar and Status
    @QtCore.Slot(int)
    def  __updateProgressBar(self, percent):
      #self.Status.setText(Status)
      ##initial state
      if percent == 0:
        self.Status.setText("Setup")
      elif percent < 100:
        self.Status.setText("in progress")
      elif percent == 100:
        self.Status.setText("Done")
      self.progressBar.setValue(percent)
    def start(self):
      self.progressView.start()
      #QObject.connect (self.progressView , QtCore.SIGNAL("__updateProgressBar(int)"),self.__updateProgressBar)
      #self.start()
      
      
##To create object of application
app = QApplication(sys.argv)
#to create the object of wedget
Widget=QWidget()
#to create form
Form = Ui_Form()
#to make setup to GUI and take widget
Form.setupUi(Widget)
Widget.show()
#to make GUI execute
app.exec_()