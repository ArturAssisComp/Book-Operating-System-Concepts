Project 2 - Linux Kernel Module for Task Information
Project description: "In this project, you will write a Linux kernel module that 
uses the /proc file system for displaying a task's information based on its process 
identifier calue pid."
Ex.: 
    $ echo "1395" > /proc/pid
    $ cat /proc/pid
      command = [bash]
      state   = [1]
      pid     = [1395]
