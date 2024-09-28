CC := gcc
CFLAGS := -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 -Wconversion

RM := rm -f

MV := mv

BUILDPATH = $(PWD)/../bin

.PHONY: all clean


all: $(BUILDPATH)/create_chat

$(BUILDPATH)/create_chat: $(BUILDPATH)/create_chat.o
	$(CC) $(CFLAGS) -o $@ $^
	#sudo chown root:root $@
	#sudo chmod u+s $@
	chown root:root $@
	chmod u+s $@

$(BUILDPATH)/%.o: %.c
	$(CC) $(CFLAGS) -c  $<
	$(MV) *.o $(BUILDPATH)

clean:
	$(RM) $(BUILDPATH)/*.o 
	sudo $(RM) $(BUILDPATH)/create_chat
