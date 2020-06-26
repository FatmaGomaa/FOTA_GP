# Steps to allocate section in RAM
1- First open the ldscript folder in the project (this file holds the linker script files and most importantly you would find mem.ld & sections.ld)
<p align="center">
 <kbd> <img img src="/Images/pic_4.PNG" width=450 height=400 /> </kbd>
</p>
2- In the file mem.ld you would find how the program define spaces of the memory (flash and RAM).

3- In ARM microprocessor memory space, the RAM starts from address 0x20000000, in our project we needed a marker that holds the identifier of the current machine executable on the microcontroller.

4- In mem.ld we assigned two memory spaces of length 4 bytes in the RAM space named MARKER & FLAG, in these memory spaces no variables from the RAM would be assigned in them except the variables that we assign to be placed in them (shown in point 9 how to assign variables to these memory sections).
<p align="center">
 <kbd> <img img src="/Images/pic_1.PNG" width=450 height=400 /> </kbd>
</p>
5- In sections.ld, we define the attribute that would collect all the variables assigned with that attribute to be placed in the specified memory space.

6- In our project we made 2 attributes (.marker & .flag) that would collect all the variables assigned with them to be placed in the memory spaces (MARKER and FLAG defined in the mem.ld file)

7- KEEP(*(.marker*)) is the syntax for the linker script to collect these variables, and > MARKER is a command for the linker script to put these variables in MARKER memory space, and also for the FLAG part, also :ALIGN(4) addresses the linker script to align the variable to 4 bytes, that means if the variable is of size 3 or 2 bytes for example, the bytes that are not filled with data would be padded, this way we make sure that the variables would have operations done on them in one cycle.
<p align="center">
 <kbd> <img img src="/Images/pic_2.PNG" width=450 height=400 /> </kbd>
</p>
8- In the application file main.c we define the variables and give them the attributes that would place them in the specified memory sections

9- In our project we define the variable RequestedAppVersion

```c
RequestedAppVersion __attribute__ ((section(".marker"),used)) 
```

to let it be placed in the marker memory section, and we do so with the variable flagCheck 

```c
flagCheck __attribute__ ((section(".flag"),used)) 
```

in the same manner to be placed in the FLAG memory section.
<p align="center">
 <kbd> <img img src="/Images/pic_3.PNG" width=600 height=300 /> </kbd>
</p>
