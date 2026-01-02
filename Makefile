
all: rd.o

objs=ramdisk.o aix_netbsd_shims.o aix_ramdisk_config.o initrd.o
rd.o: $(objs)
	ld -r -o $@ $(objs)


clean:
	-rm *.o

# Compiler command line for Metaware High C
CC=cc -DUSE_OS_LONG_IO

# Compiler command line for GCC
#CC=gcc -Wall -Wno-comment -Werror
 
# TODO: sort out the issues with optimized builds, then add
#  -O2

# Common options
CFLAGS=-D_KERNEL -DKERNEL -Di386 -I/usr/include/sys -I. -DRAMDISK_HOOKS=1

install: rd.o
	echo Archiving ramdisk support into the kernel library...
	ar -rv /usr/sys/386/386lib.a rd.o
	echo Installing rd support in master, system and predefined files
	sh ./instal
	echo Rebuilding the kernel...
	#/usr/sys/newkernel -d -install  # Link with all output for when things are starting to fail
	/usr/sys/newkernel -install


SET_ULIMIT=ulimit -s 200000 >/dev/null; ulimit -b 200000; ulimit -f 2000000; [ `ulimit -s` -eq 200000 ]; [ `ulimit -b` -eq 200000 ]; [ `ulimit -f` -eq 2000000 ];

initrd.o: initrd.s
	$(SET_ULIMIT) as -o initrd.o initrd.s

initrd.s: initrd.img
	$(SET_ULIMIT) /u/root/bin/bash ./bin2s initrd.img > /tmp/initrd.s
	cp /tmp/initrd.s initrd.s

# stuff for creating the ramdisk image initrd.o on the linux side where it's faster:
#initrd.o: initrd.s
#	/usr/bin/i686-w64-mingw32-as -o initrd.o initrd.s  # from debian mingw32-binutils
#	cp initrd.o initrd.save_o
