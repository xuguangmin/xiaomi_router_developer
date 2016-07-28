src:=$(wildcard *.c)
obj:=$(patsubst %.c, %.o, $(src))


a.out:$(obj)
	$(CC) -Wall -o $@ $^

clean:
	rm -f *.o a.out
