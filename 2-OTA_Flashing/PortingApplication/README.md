# Firebase with pyinstaller
When you want to convert the python file to an executable using py installer, you need additional hooks to use firebase and gcloud. For that you need to create a new python file at some directory NAMED 'hook-gcloud.py' with the following code:

```
from PyInstaller.utils.hooks import copy_metadata
datas = copy_metadata('gcloud')
```

When you want to convert the file into an executable you have to add the argument '--additional-hooks-dir the directory to the hook-gcloud.py file'.

Example:
```
pyinstaller --onefile --additional-hooks-dir C:\Users\user\hooks index.py
```

## Steps to build whole project:

1- compile comreceiver.c [gcc CommReceiver.c TProtocol.c ]

2- create 'hook-gcloud.py' and add the lines of code above. 

3- pyinstaller --onefile -w --additional-hooks-dir G:\FOTA_GP\ELFI FirebaseTrial.py

3- pyinstaller --onefile -w --additional-hooks-dir G:\FOTA_GP\ELFI FetchNodeMCUs.py

4- pyinstaller --onefile -w --icon=./Pic/elfi.ico gui_fota_v2.py

5- move a.exe and Pics to same directory of dist 

6- run gui_fota.exe
