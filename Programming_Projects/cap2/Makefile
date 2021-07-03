obj-m += simple.o
obj-m += simple_exercise1.o
obj-m += simple_exercise2.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
