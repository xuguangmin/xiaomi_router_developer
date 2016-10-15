
a.out:
	mipsel-linux-gcc -Wall -o mips-ntp-server  mips-ntp-server.c
	mipsel-linux-gcc -Wall -o mips-ntp-client  mips-ntp-client.c -lrt
	gcc -Wall -o x86-ntp-server  x86-ntp-server.c

clean:
	rm mips-ntp-server  mips-ntp-client x86-ntp-server    *.o -f
