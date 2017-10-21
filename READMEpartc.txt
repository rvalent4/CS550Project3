Insert the module and then run the main

1) insmod kernel/mytime.ko
2) gcc main3.c
3) ./a.out


The difference between current_kernel_time() and gettimeofday() has to do with the current_kernel_time not reading from a timer, but instead from a value which is periodically updated (so it is less accurate), and will most likely show a time earlier than the actual time since the time value will most likely be updated earlier than the funcion call.
