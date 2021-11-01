#! /bin/bash
echo Compiling main:
gcc main.c bit_array.c pid_table.c pid_manager.c -o main
echo file \'main\' generated.
echo Compiling debug_bit_array:
gcc debug/debug_bit_array.c bit_array.c -o debug_bit_array
echo file \'debug_bit_array\' generated.
echo Compiling debug_pid_table:
gcc debug/debug_pid_table.c bit_array.c pid_table.c -o debug_pid_table
echo file \'debug_pid_table\' generated.

