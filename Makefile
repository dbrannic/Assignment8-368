WARNING = -Wall -Wshadow --pedantic 
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR)

SRCS = a8.c
OBJS = $(SRCS:%.c=%.o)

a8: $(OBJS)
		$(GCC) $(OBJS) -o a8

.c.o:
		$(GCC) -c $*.c 

clean:
	rm -f a8 *.o output? *~