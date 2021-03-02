obj-m+=NF_KMOD.o

all:
	make -C /lib/modules/5.10.16-1-lts/build M=$(PWD) modules
	rm -r -f *.mod.c .*.cmd *.symvers *.o modules.order *.mod

clean:
	make -C /lib/modules/5.10.16-1-lts/build M=$(PWD) clean

