src := $(wildcard *.c)

main:$(patsubst %.c, %.o, $(src))
	gcc -Wall -o rcver $(src)

clean:
	rm main *.o -f
