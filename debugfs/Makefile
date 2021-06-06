obj-m := test_debugfs.o

default:
	${MAKE} -C /lib/modules/`uname -r`/build M=`pwd` modules

clean:
	@rm -f *.mod *.cmd *.o *.order *.symvers *.ko *.mod.c .*.cmd
