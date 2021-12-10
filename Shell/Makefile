SOURCES = $(wildcard *.c) 
OBJS = $(patsubst %.c,%.o,$(SOURCES))

%.o : %.c
	gcc -c $^ -o $@

Myshell : $(OBJS) # 不能使用 %.o 通配符会失效
	gcc $(OBJS) -o Myshell 

clean:
	rm -f *.o Myshell