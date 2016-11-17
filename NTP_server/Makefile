
prev=/home/gm/下载/buildroot/output/host/usr/bin
a.out:
	/home/gm/下载/buildroot/output/host/usr/bin/mipsel-linux-gcc -Wall -o mips-ntp-server  mips-ntp-server.c
	/home/gm/下载/buildroot/output/host/usr/bin/mipsel-linux-gcc -Wall -o mips-ntp-client  mips-ntp-client.c -lrt
	gcc -Wall -o x86-ntp-server  x86-ntp-server.c

clean:
	rm mips-ntp-server  mips-ntp-client x86-ntp-server    *.o -f
