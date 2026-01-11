# aix_ramdisk

A ramdisk driver for AIX PS/2 1.3. Ported from NetBSD 1.1.

This driver is experimental; it has had LIMITED TESTING. Known to work with the metaware compiler. See licenses in the source files.

To build and install the driver use:
```
make
./instal
```

To build the command line utility
```
cd rdconfig
make
```

As-is this expects you to provide an `initrd.img` in the source directory which it will build-in and automatically provide on `/dev/rd0`.
The `instal` script adds the `/etc/system` stanzas for its `/dev/rd0` and `/dev/rrd0` to appear automatically so you can access it.

But as-is the source provides 4 possible ramdisks (change `NUM_POSSIBLE_RD` in `ramdisk.c` to adjust).

The `instal` script picks a major device number automatically; you can look at your `ls -l /dev/rd0` or in the generated `rd:` section in `/etc/master` to find out what it was.

Each ramdisk has the possible usual device nodes for a disk `/dev/rd`n (block) and `/dev/rrd`n (char), with minor=ramdisk number, and a control device `/dev/rd`n`c` (char), with minor n+16, which is for use with the `rdconfig` tool

To just make the additional devices with `mknod`, do e.g.
```
mknod /dev/rd1 b m 1
mknod /dev/rrd1 c m 1
mknod /dev/rd1c c m 17
mknod /dev/rd2 b m 2
mknod /dev/rrd2 c m 2
mknod /dev/rd2c c m 18
```
...

where m is your major number.

The additional 3 ramdisks are disabled by default and you can use the `rdconfig` tool to set the size/enable:

```
rdconfig /dev/rd{n}c [size]
```
where {n} is your ramdisk number and size is the size in 512-byte blocks.
`rdconfig /dev/rd{n}c` without a size will tell you about the current configuration of that ramdisk.

Setting a ramdisk at runtime just allocates kernel space right now, as the userspace service thing from the netbsd driver didn't work immediately out of the box, and `rdconfig` provides no way to change it once initially set other than killing the userspace service so there's no way to do that at the moment.


License (from ramdisk.c):
```
/*
 * Copyright (c) 1995 Gordon W. Ross, Leo Weppelman.
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
 *      This product includes software developed by
 *                      Gordon W. Ross and Leo Weppelman.
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
```
