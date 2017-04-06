PLATFORM=$(shell uname)
CC = gcc
AR = ar

SHARED_LIB = lcoredump.so
OBJS = lcoredump.o

CFLAGS = -c -O3 -Wall -fPIC -Wno-gnu-zero-variadic-macro-arguments
LDFLAGS = -O3 -Wall --shared


ifeq ($(PLATFORM),Linux)
else
	ifeq ($(PLATFORM), Darwin)
		LDFLAGS += -dynamiclib -Wl,-undefined,dynamic_lookup
	endif
endif

all : $(SHARED_LIB)
	rm $(OBJS)

$(SHARED_LIB): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LLIBS)

$(OBJS) : %.o : %.c
	$(CC) -o $@ $(CFLAGS) $<

clean : 
	rm -f $(OBJS) $(SHARED_LIB)

.PHONY : clean

