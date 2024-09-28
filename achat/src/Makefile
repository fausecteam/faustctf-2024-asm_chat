CC := gcc
CFLAGS := -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 -Wconversion -g -D DEBUG
MV := mv 
RM = rm -f

BUILDPATH = $(PWD)/../bin


.PHONY: all debug clean

all: $(BUILDPATH)/achat

$(BUILDPATH)/achat: $(BUILDPATH)/achat.o $(BUILDPATH)/connection_handler.o $(BUILDPATH)/handle_commands.o $(BUILDPATH)/commands.o
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDPATH)/achat.o: achat.c connection_handler.h

$(BUILDPATH)/connection_handler.o: connection_handler.c connection_handler.h handle_commands.h

$(BUILDPATH)/handle_commands.o: handle_commands.c handle_commands.c ./commands/commands.h

$(BUILDPATH)/commands.o: ./commands/commands.h  ./commands/chats.c ./commands/chat_tools.c ./commands/chat_tools.h ./commands/commands.h ./commands/init_chat.c ./commands/login_out.c ./commands/misc.c ./commands/search_chats.c ./commands/send_rcv_msgs.c ./commands/commands.h
		$(MAKE) -C ./commands CC="$(CC)" CFLAGS="$(CFLAGS)" RM="$(RM)" BUILDPATH=$(BUILDPATH)

$(BUILDPATH)/%.o: %.c
	$(CC) $(CFLAGS) -c  $<
	$(MV) *.o $(BUILDPATH)

clean:
	$(RM) $(BUILDPATH)/*.o $(BUILDPATH)/achat
