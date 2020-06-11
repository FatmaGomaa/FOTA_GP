# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'ComboBox.ui'
##
## Created by: Qt User Interface Compiler version 5.14.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide2.QtCore import (QCoreApplication, QMetaObject, QObject, QPoint,
    QRect, QSize, QUrl, Qt ,Signal)
from PySide2.QtGui import (QBrush, QColor, QConicalGradient, QFont,
    QFontDatabase, QIcon, QLinearGradient, QPalette, QPainter, QPixmap,
    QRadialGradient)
from PySide2.QtWidgets import *

import sys
import os

class Ui_Form(object):
    def setupUi(self, Form):
        if Form.objectName():
            Form.setObjectName(u"Form")
        Form.resize(400, 300)
        self.comboBox = QComboBox(Form)
        self.comboBox.setObjectName(u"comboBox")
        self.comboBox.setGeometry(QRect(20, 40, 231, 22))
        self.pushButton = QPushButton(Form)
        self.pushButton.setObjectName(u"pushButton")
        self.pushButton.setGeometry(QRect(300, 40, 75, 23))

        self.retranslateUi(Form)

        QMetaObject.connectSlotsByName(Form)
    # setupUi

    def retranslateUi(self, Form):
        Form.setWindowTitle(QCoreApplication.translate("Form", u"Form", None))
        self.pushButton.setText(QCoreApplication.translate("Form", u"PushButton", None))
    # retranslateUi
        self.pushButton.clicked.connect(self.GenerateFunction)
    
    def GenerateFunction(self) :
        f = open('./items.txt','r')
        lines = f.readlines()
        f.close()
        self.comboBox.addItems(lines)
        
        
        
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
