# The GPL applies to this program.
# In addition, as a special exception, the copyright holders give
# permission to link the code of portions of this program with the
# OpenSSL library under certain conditions as described in each
# individual source file, and distribute linked combinations
# including the two.
# You must obey the GNU General Public License in all respects
# for all of the code used other than OpenSSL.  If you modify
# file(s) with this exception, you may extend this exception to your
# version of the file(s), but you are not obligated to do so.  If you
# do not wish to do so, delete this exception statement from your
# version.  If you delete this exception statement from all source
# files in the program, then also delete it here.

include version

DEBUG=-g # -D_DEBUG
LDFLAGS+=-lssl -lcrypto $(DEBUG)
CFLAGS+=-O2 -Wall -DVERSION=\"$(VERSION)\" $(DEBUG)

OBJS=mem.o http.o io.o str.o error.o utils.o main.o tcp.o mssl.o res.o

all: httping

httping: $(OBJS)
	$(CC) -Wall -W $(OBJS) $(LDFLAGS) -o httping
	#
	# Oh, blatant plug: http://keetweej.vanheusden.com/wishlist.html

install: httping
	cp httping $(DESTDIR)/usr/bin
	cp httping.1 $(DESTDIR)/usr/share/man/man1/

clean:
	rm -f $(OBJS) core httping

package: clean
	# source package
	rm -rf httping-$(VERSION)*
	mkdir httping-$(VERSION)
	cp *.c *.h httping.1 Makefile* version readme.txt license.* httping-$(VERSION)
	tar cf - httping-$(VERSION) | gzip -9 > httping-$(VERSION).tgz
	rm -rf httping-$(VERSION)
