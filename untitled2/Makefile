PROGS = armemu_tests
OBJS = armemu.o cache.o utils.o quadratic_c.o quadratic_s.o \
                                       fib_iter_s.o fib_iter_c.o \
                                       fib_rec_s.o fib_rec_c.o \
                                       sum_array_s.o \
                                       find_max_s.o \
                                       sort_s.o find_max_index_s.o\
                                       strlen_s.o strlen_c.o\
                                       cache_sa.o cache_fa.o

HFILES = armemu.h cache.h utils.h cache_fa.h cache_sa.h

CFLAGS = -g

# Pattern rules to avoid explicit rules
%.o : %.c
	gcc ${CFLAGS} -c -o $@ $<

%.o : %.s
	as -o $@ $<


all : ${PROGS}

armemu.o : armemu.c armemu.h
	gcc ${CFLAGS} -c -o $@ $<

cache.o : cache.c cache.h
	gcc ${CFLAGS} -c -o $@ $<

utils.o : utils.c utils.h
	gcc ${CFLAGS} -c -o $@ $<

cache_sa.o : cache_sa.c cache_sa.h
    gcc ${CFLAGS} -c -o $@ $<

cache_fa.o : cache_fa.c cache_fa.h
	gcc ${CFLAGS} -c -o $@ $<

armemu_tests : armemu_tests.c ${OBJS}
	gcc ${CFLAGS} -o $@ $^

test :
	./armemu_tests

clean :
	rm -rf ${PROGS} ${OBJS} *~

