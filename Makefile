IDIR =../include
CC=clang

libandroid-fixshebang.so: libandroid-fixshebang.c
	$(CC) -Wall -shared -o libandroid-fixshebang.so libandroid-fixshebang.c

.PHONY: install
install:
	cp fixedshe ${PREFIX}/bin/
	chmod 755 ${PREFIX}/bin/fixedshe
	cp libandroid-fixshebang.so ${PREFIX}/var/lib
	chmod 655 ${PREFIX}/var/lib/libandroid-fixshebang.so

.PHONY: clean
clean:
	rm libandroid-fixshebang.so

.PHONY: uninstall
uninstall:
	rm ${PREFIX}/bin/fixedshe
	rm ${PREFIX}/var/lib/libandroid-fixshebang.so

