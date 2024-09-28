SERVICE := achat
DESTDIR ?= dist_root
SERVICEDIR ?= /srv/$(SERVICE)

.PHONY: build install

build:
	echo "Nothing to build"

install: build
	mkdir -p $(DESTDIR)$(SERVICEDIR)/
	yq -y 'del(.services."achat_deps")' docker-compose.yml > $(DESTDIR)$(SERVICEDIR)/docker-compose.yml
	#cp achat/Dockerfile* $(DESTDIR)$(SERVICEDIR)/achat/
	#cp -r achat/src $(DESTDIR)$(SERVICEDIR)/achat/src/
	#cp -r achat/tools $(DESTDIR)$(SERVICEDIR)/achat/tools/
	#cp achat/Makefile $(DESTDIR)$(SERVICEDIR)/achat/Makefile
	mkdir -p $(DESTDIR)/etc/systemd/system/faustctf.target.wants/
	ln -s /etc/systemd/system/docker-compose@.service $(DESTDIR)/etc/systemd/system/faustctf.target.wants/docker-compose@$(SERVICE).service

