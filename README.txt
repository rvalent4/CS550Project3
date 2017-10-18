Robert Valenti Part B


1) First I added sys_my_xtime to the system call table (with number 326)

2) Then I wrote the code for the system call function I used in kernel/my_xtime.c

3) Then I compiled kernel with make bzImage, make modules, make modules_install and make install and then reboot

4) Finally I tested with main.c which calls syscall(326)

The files I edited are on the github link:

https://github.com/rvalent4/CS550Project2

