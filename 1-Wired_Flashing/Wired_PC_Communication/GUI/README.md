# Purpose 

This document aims to present the QT Designer, how to use it, and the problems that we faced while dealing with QT Designer.

## Qt Designer and GUI

### Description:

**First we will start by how we can install Pyside2 to make our GUI:**

Pyside2 is newer than PyQt and supports Python **v3.8.1** so:

1.  First make sure that the version of python you have is 3.8.1 by open CMD and
    write :

**\>\>python –version**

1.  After checking the python version start download Pyside2 using CMD window
    and run this command:

-   **\>\>pip install PySide2**

-   This command will install PySide2 site-packages in the following path

-   **C:\\Program Files\\Python38\\Lib\\site-packages\\PySide2\\**

-   Also will install the PySide2 GUI converter in the following Path

-   **C:\\Program Files\\Python38\\Scripts\\PySide2-uic.exe**

-   Then in the PySide site-package folder you will find designer tool called

-   **designer.exe**

-   Then create shortcut from **designer.exe** of designer app by making **right
    click** the choose **send to -\>desktop**

**Now we have PySide2 designer ……………………………………………………………………………………........**

**Second we will show how we can make our GUI step by step :**

1.  First open Pyside2 just click on **designer.exe,** this page will open

![](/2-OTA_Flashing/GUI/Python_ELFI/media/af7236bb6ee803b4ffe23b4d21e201a7.png)

2.  Then choose Widget and press create button and this form will open

![](/2-OTA_Flashing/GUI/Python_ELFI/media/e94639d2c9e8b09b4c34ad3aa9962963.png)

3.  Now we can make your design of GUI by choosing the components from This list

![](/2-OTA_Flashing/GUI/Python_ELFI/media/57e240cd30a529508889e54695f84cb6.png)

**Now we will see an example to make for example Hardware Family box in our
GUI**

-   First we will choose Combo Box from Component list

![](/2-OTA_Flashing/GUI/Python_ELFI/media/0b6335744af827b4ba9863c96723704f.png)

-   Then we will add it in our form as below

![](/2-OTA_Flashing/GUI/Python_ELFI/media/14ea3fbb80dd94123605bf57145657ae.png)

-   Then we need to add different types of ECU

![](/2-OTA_Flashing/GUI/Python_ELFI/media/bd2bd9df81f6d5dac92e4efb1753a393.png)

-   Then if you need for example to change many options related to Combo Box

1.  **For example name of Qwidget from object Inspector part**

2.  **Changing Qwidget options from property part**

3.  **By adding signal to connect ComboBox with any another component using
    signal/slot editor**

![](/2-OTA_Flashing/GUI/Python_ELFI/media/e140c933d9be01eeeefb3660aee56767.png)

-   Then to show result in GUI mode use **ctrl+R**

![](/2-OTA_Flashing/GUI/Python_ELFI/media/bf9edabba57c00b0f736c28b30286be3.png)

**So in our GUI we used many components as:**

-   **Combo Box**

-   **Label**

-   **Line Edit**

-   **Push Button**

-   **Text Browser**

4.  Then after finishing your GUI design save it in any location you need Then
    go to this location and open CMD and write

**\>\>pyside2-uic File.ui –o File.py**

>   This command used to generate output Python file from File.ui generated from
>   PySide2 designer

5.  Then open File.py and Write Your Logic that you needed to serve your
    application



## problems

1- Qstring is not defined ---> solved by function that maps Qstring to str()
2- passing a c file with an argument to python script to make output file -> solved by (changing ./a.out to ./a.exe)
3- undefined "self" name is used -> solved by putting the function using self in the class UI_Form
4- readline of file invalid literal for int with base 10 -> solved by using line.split() & by casting the string to float then to int
5- if we run the gui then run the c file, the c file would take the terminal -> solved by running both the gui and the c file on threads to run concurrently by calling self.connect & self.emit 
6- the progress bar didn't update whenever a packet was flashed -> solved by putting a delay to match the speed between the progress bar to read the progess.txt file and the time the pc writes on the file
7- the progress.txt file wasn't proberly initialized when the send button is pressed, led to progressBar not functioning proberly -> solved by initializing the progress.txt file with the value (0 0) when the send button is pressed
8- TODO: update the feature of comm port detection: the function fetches the first comm port present in the PC (in case there are multiple comm ports connected on pc, might lead to connect with the wrong port)
9- TODO: update the ELF_file automation: the script would search in the path for the elf file rather than presenting it in the script

### Code Documentation:

1- The first step after you run the GUI would be to choose which machine you would like to flash the elf file specified to that machine.
2- QString() function that converts its input to str(), it overrides the default QString function in the QCore.
3- in Class Progress (a ProgressBar Thread Class) a run(self) is used to make a runnable thread for the progressBar that reads from the progress.txt file and update its value (the function that has a signal that is emitted to reset the value of the progressBar to be 0 at the beginning of the run)
   * the function then reads the values in the progress.txt and calculate how many packets of the total packets are flashed, and emits a signal to update the progressBar with the percentage of the total flashing operation
4- a run(self) used to make a runnable thread to compile and run the C files needed to send and flash the elf file on the machine
   * the function checks for the port commnumber and the elf file are presented to start running the execution file to parse and flash the elf packets
5- Ui_Form is the function generated by the pyside2 used to make the GUI
6- Connecting the signals from the buttons so that they would call a specific function
   * Connect_Button (Send) in the GUI is connected to the GenerateFunction, so once the Send Button is pressed the GenerateFunction is called
   * pushButton (Search) in the GUI is connected to ComFunction, once the Search Button is pressed it would fetch the commPort Number from the PC
7- create an Object from class Progress (in point 2) named progressView to connect its runnable with a function __updateProgressBar
8- GenerateFunction is the Function call when Button Send is pressed :
   * Check if the entered path for the elf file is a valid path, in case not valid generate an error dialogue window
   * in case the path is valid, it would take the CommPort Number and the elf file, import them to the CommReceive.c file and start its thread
   * check for the progress.txt if it is in the valid path or not, in case not valid it would generate an error dialogue window
   * in case a valid path for the progress.txt file, it would initialize it with value (0 0), then connect a signal between progressView with the function __updateProgressBar (in point 7)
9- ComFunction (in point 6 sub-section 2) fetches all the current commports on the PC (using class port_list from serial.tools.list_ports  i.e. it needs to have pyserial on the PC: run the following command in admin cmd -> pip install pyserial)
   * if ports are not available, it would generate an error dialogue window
   * it ports are availablem, use regex to fetch all the ports and add them to a tuple, and take the first instance (would be updated in Point number 8 in Problems section)
10- __updateProgressBar Function takes an input argument percent to update the progressBar Value with that percent.
    * the function sets the value of the progressBar with the percent calculated and passed to it as an input argument (in point 3 sub-section)
    * if the percent is 0, the status of the progress bar is set to "Setup"
    * if the percent is less than 100, the status of the progress bar is set to "in progress"
    * if the percent is equal to 100, then the status of the progress bar is set to "Done"
11- start function is used to run the progressView Thread
