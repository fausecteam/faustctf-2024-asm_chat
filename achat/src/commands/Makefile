CC := gcc

CFLAGS := -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 -Wconversion -g -D DEBUG

LD := ld

MV := mv

RM = rm -f


BUILDPATH = $(PWD)/../../bin


.PHONY: all debug clean

all: $(BUILDPATH)/commands.o commands.h

$(BUILDPATH)/commands.o: $(BUILDPATH)/chat_tools.o  $(BUILDPATH)/chats.o $(BUILDPATH)/init_chat.o $(BUILDPATH)/login_out.o $(BUILDPATH)/misc.o $(BUILDPATH)/search_chats.o $(BUILDPATH)/send_rcv_msgs.o
	$(LD) -r $^ -o $@

$(BUILDPATH)/chat_tools.o: chat_tools.c chat_tools.h

$(BUILDPATH)/chats.o: chats.o commands.h
$(BUILDPATH)/init_chat.o: init_chat.c commands.h
$(BUILDPATH)/login_out.o: login_out.c commands.h
$(BUILDPATH)/misc.o: misc.c commands.h
$(BUILDPATH)/search_chats.o: search_chats.c commands.h
$(BUILDPATH)/send_rcv_msgs.o: send_rcv_msgs.c commands.h

$(BUILDPATH)/%.o: %.c
	$(CC) $(CFLAGS) -c  $<
	$(MV) *.o $(BUILDPATH)

clean:
	$(RM) $(BUILDPATH)/*.o achat

