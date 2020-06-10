from cx_Freeze import setup, Executable 
  
setup(name = "gui" , 
      version = "0.1" , 
      description = "" , 
      executables = [Executable("gui_fota.py")]) 