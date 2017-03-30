# To create your own version, just unpack n2n tarball here
# and update the VER variable.  Or export your own from their
# svn via:
# svn export -r <REV> svn://svn.mplayerhq.hu/n2n/trunk n2n-svn-<REV>
VER = n2n-svn-3862
URL = https://svn.ntop.org/svn/ntop/trunk/n2n/

all:
	$(MAKE) -C $(VER) CC=$(CC)

clean:
	$(MAKE) -C $(VER) clean
	rm -f $(VER)/*.gdb

romfs:
	$(ROMFSINST) $(VER)/supernode /bin/supernode
	$(ROMFSINST) $(VER)/edge /bin/edge

CURR_REV = $(patsubst n2n-svn-%,%,$(VER))
LATEST_REV = 
update:
	set -e ; \
	uprev=`svn info $(URL) | awk '$$1 == "Revision:" { print $$NF }'` ; \
	if [ "$(CURR_REV)" != "$$uprev" ] ; then \
		svn export -r $$uprev $(URL) n2n-svn-$$uprev ; \
		sed -i "/^VER =/s:=.*:= n2n-svn-$$uprev:" Makefile ; \
		echo "Updated to $$uprev" ; \
	else \
		echo "Already latest rev ($$uprev)" ; \
	fi
