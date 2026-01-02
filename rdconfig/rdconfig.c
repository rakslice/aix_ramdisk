/*	$NetBSD: rdconfig.c,v 1.1.1.1 1995/10/08 22:40:41 gwr Exp $	*/

/*
 * Copyright (c) 1995 Gordon W. Ross
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 4. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Gordon W. Ross
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This program exists for the sole purpose of providing
 * user-space memory for the new RAM-disk driver (rd).
 * The job done by this is similar to mount_mfs.
 * (But this design allows any filesystem format!)
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/param.h>

#ifdef _AIX

#include "ramdisk.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#else

#include <sys/mman.h>
#include <dev/ramdisk.h>

#endif

main(argc, argv)
	int argc;
	char **argv;
{
	struct rd_conf rd;
	int nblks, fd, is_set;

#ifndef _AIX
	int error;
#endif

	if (argc <= 1) {
		fprintf(stderr, "usage: rdconfig <device> [<%d-byte-blocks>]\n",
				DEV_BSIZE);
		exit(1);
	}

	if (argc > 2) {
		nblks = atoi(argv[2]);
		if (nblks <= 0) {
			fprintf(stderr, "invalid number of blocks\n");
			exit(1);
		}
		is_set = 1;
		rd.rd_size = nblks << DEV_BSHIFT;
	} else {
		is_set = 0;
	}

	fd = open(argv[1], O_RDWR, 0);
	if (fd < 0) {
		perror(argv[1]);
		exit(1);
	}

	if (!is_set) {
		// we're just showing the current settings
		if (ioctl(fd, RD_GETCONF, &rd)) {
			perror("ioctl");
			exit(1);
		}
		printf("%d\n", rd.rd_size);
		exit(0);
	}

#ifdef _AIX
	int shmid = shmget(IPC_PRIVATE, rd.rd_size, S_IRUSR | S_IWUSR);
	if (shmid == -1) {
		perror("shmget");
		exit(1);
	}
	rd.rd_addr = (caddr_t)shmat(shmid, 0, 0);
	if (rd.rd_addr == (caddr_t)-1) {
		perror("shmat");
		exit(1);
	}
#else
	rd.rd_addr = mmap(NULL, rd.rd_size,
				PROT_READ | PROT_WRITE,
				MAP_ANON | MAP_PRIVATE,
				-1, 0);
	if (rd.rd_addr == (caddr_t)-1) {
		perror("mmap");
		exit(1);
	}
#endif

	/* Become server! */
	rd.rd_type = RD_UMEM_SERVER;
	if (ioctl(fd, RD_SETCONF, &rd)) {
		perror("ioctl");
		exit(1);
	}

	exit(0);
}
