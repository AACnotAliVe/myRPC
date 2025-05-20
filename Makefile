.PHONY: all clean deb

all:
	$(MAKE) -C src/mysyslog
	$(MAKE) -C src/client
	$(MAKE) -C src/server

clean:
	$(MAKE) -C src/mysyslog clean
	$(MAKE) -C src/client clean
	$(MAKE) -C src/server clean
	rm -rf build-deb
	rm -rf build
	rm -rf bin
	rm -rf repo
	rm -rf deb
	bash clean.sh

deb:
	$(MAKE) -C src/mysyslog deb
	$(MAKE) -C src/client deb
	$(MAKE) -C src/server deb

repo:
	mkdir -p repo
	cp deb/*.deb repo/
	dpkg-scanpackages repo /dev/null | gzip -9c > repo/Packages.gz
