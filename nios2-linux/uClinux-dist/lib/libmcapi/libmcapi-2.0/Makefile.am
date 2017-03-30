AUTOMAKE_OPTIONS = foreign

SUBDIRS = . tests

INCLUDES = -I$(top_srcdir)/include
lib_LTLIBRARIES = libmcapi.la

library_includedir = $(includedir)/$(PACKAGE_NAME)
library_include_HEADERS = include/mca.h include/mcapi_impl_spec.h include/mcapi_dev_impl.h  include/mcapi.h  include/mcapi_test.h  include/transport_sm.h

libmcapi_la_SOURCES  = mcapi.c mcapi_trans_stub.c trans_impl/tran_impl_dev.c

