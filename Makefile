CC= gcc
CFLAGS= -Ofast 
PROG1= eyes-care
PROG2= eyes-care-controller
HELPER= common

all: $(PROG1) $(PROG2)

$(PROG1): $(PROG1).o $(HELPER).o
	$(CC) $(CFLAGS) -o $@ $?

$(PROG1).o: $(PROG1).c
	$(CC) $(CFLAGS) -c $?

$(PROG2): $(PROG2).o $(HELPER).o
	$(CC) $(CFLAGS) -o $@ $?

$(PROG2).o: $(PROG2).c 
	$(CC) $(CFLAGS) -c $?

$(HELPER).o: $(HELPER).c
	$(CC) $(CFLAGS) -c $?

install: all
	sudo cp $(PROG1) /usr/bin/$(PROG1)
	sudo cp $(PROG2) /usr/bin/$(PROG2)

uninstall:
	sudo rm -f /usr/bin/$(PROG1) /usr/bin/$(PROG2)

clean:
	rm -f $(PROG1) $(PROG1).o $(PROG2) $(PROG2).o  $(HELPER).o