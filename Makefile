
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

small_lim=300000
big_lim=5000000
SET_ULIMIT=ulimit -s $(small_lim) >/dev/null; ulimit -b $(small_lim); ulimit -f $(big_lim); [ `ulimit -s` -eq $(small_lim) ]; [ `ulimit -b` -eq $(small_lim) ]; [ `ulimit -f` -eq $(big_lim) ];

initrd.img: root_install_net_with_swap.img


initrd.s: initrd.img root_install_net_with_swap.img bin2s
	#$(SET_ULIMIT) /u/root/bin/bash ./bin2s initrd.img > /tmp/initrd.s
	#cp /tmp/initrd.s initrd.s
	./bin2s initrd.img > initrd.s

# stuff for creating the ramdisk image initrd.o on the linux side where it's faster:
initrd.o: initrd.s
	# mingw as from debian mingw32-binutils
	uname
	if [ "`uname`" = "Linux" ]; then /usr/bin/i686-w64-mingw32-as -o initrd.o initrd.s && cp initrd.o initrd.save_o ; else $(SET_ULIMIT) as -o initrd.o initrd.s ; fi


root_install_net.img: ../root_net_disk/root_install_net.img
	-rm root_install_net.img
	cp ../root_net_disk/root_install_net.img root_install_net.img

swap_25_zeroed: swap_25_zeroed.Z
	uncompress -c swap_25_zeroed.Z > swap_25_zeroed

#root_install_net_with_swap.img: root_install_net.img swap_25_zeroed
#	cat root_install_net.img swap_25_zeroed > root_install_net_with_swap.img

root_install_net_with_swap.img: root_install_net.img actual_fd_swap root_disk_start.img
	dd bs=4096 skip=1 if=root_install_net.img | cat root_disk_start.img - actual_fd_swap > root_install_net_with_swap.img
