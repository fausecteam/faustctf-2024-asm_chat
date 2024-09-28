CC := gcc
CFLAGS :=  -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 -Wconversion

RM = rm -f

BUILDPATH = $(PWD)/bin

.PHONY: all debug clean clean-deep

all: achat

achat:
	$(MAKE) -C ./src CC="$(CC)" CFLAGS="$(CFLAGS)" RM="$(RM)" BUILDPATH=$(BUILDPATH)
	$(MAKE) -C ./tools CC="$(CC)" CFLAGS="$(CFLAGS)" RM="$(RM)" BUILDPATH=$(BUILDPATH)

debug:
	$(MAKE) -C ./src CC=$(CC) CFLAGS="$(CFLAGS) -g -D DEBUG" RM="$(RM)" BUILDPATH=$(BUILDPATH)
	$(MAKE) -C ./tools CC="$(CC)" CFLAGS="$(CFLAGS) -g -D DEBUG" RM="$(RM)" BUILDPATH=$(BUILDPATH)

clean:
	$(RM) $(BUILDPATH)/*.o $(BUILDPATH)/achat 
	sudo $(RM) $(BUILDPATH)/create_chat

clean-deep:
	$(RM) $(BUILDPATH)/*.o $(BUILDPATH)/achat 
	sudo $(RM) $(BUILDPATH)/create_chat
	sudo chattr -a $(BUILDPATH)/.achat_data
	sudo chattr -a $(BUILDPATH)/.achat_data/*
	touch $(BUILDPATH)/.achat_data/chats/123
	sudo chattr -a $(BUILDPATH)/.achat_data/chats/*
	sudo $(RM) -r $(BUILDPATH)/.achat_data
	

