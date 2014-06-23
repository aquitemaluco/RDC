CC := gcc
CFLAGS := -c -Wall
INCLUDES := -I../include
LIBS=-lpthread
SOURCES := sinet.c device.c hub.c switch.c message.c ethernet.c host.c \
			arp.c ip.c mac.c observer.c observable.c
OBJECTS := $(SOURCES:.c=.o)
SINETLIB := ../lib/sinetlib.a
SINETHUB := ../bin/sinethub.bin
SINETSWITCH := ../bin/sinetswitch.bin


.SUFFICES: .c

all: $(SINETLIB)

$(SINETLIB): $(OBJECTS) sinetlib sinethub sinetswitch

sinethub:
	@echo "Generating $(SINETHUB)"
	$(CC) -DHC $(INCLUDES) sinetrun.c  $(SINETLIB) -o $(SINETHUB) $(LIBS)

sinetswitch:
	@echo "Generating $(SINETSWITCH)"
	$(CC) -DSC $(INCLUDES) sinetrun.c  $(SINETLIB) -o $(SINETSWITCH) $(LIBS)

sinetlib:
	@echo "Generating $(SINETLIB)"
	ar rcs $(SINETLIB) $(OBJECTS)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(SINETLIB) $(SINETHUB) $(SINETSWITCH)
