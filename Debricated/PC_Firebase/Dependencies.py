import sys
from subprocess import call
import os
from os import environ


call(["python", "-m", "pip", "install", "--upgrade" ,"pip"])
call(["pip", "install", "pyserial"])
call(["pip", "install", "pycryptodome"])
call(["pip", "install", "pyrebase4"])

