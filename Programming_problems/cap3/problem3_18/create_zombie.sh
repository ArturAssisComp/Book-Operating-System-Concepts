#! /bin/bash
gcc create_zombie.c
./a.out &
sleep 2
echo Zombie process:
ps -l
rm ./a.out
