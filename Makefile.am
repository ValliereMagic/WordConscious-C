#Include M4 Macros
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = WordConscious

#Program Sources
WordConscious_SOURCES = \
    src/bin/main.c \
    src/bin/tests.c src/bin/tests.h \
    src/bin/words.c src/bin/words.h \
    src/bin/config_usr.c src/bin/config_usr.h \
    src/bin/hints.c /src/bin/hints.h \
    src/lib/linkedlist/WC_linkedlist.h

#Link libsodium
WordConscious_LDADD = libWC_linkedlist.la @WC_sodium_flags@ @WC_pthread_flags@ @WC_libconfig_flags@

#define data directory
WordConsciousdatadir=@datadir@/WordConscious

#define configuration directory
WordConsciousconfdir=@sysconfdir@/WordConscious

#Add CFLAGS
WordConscious_CFLAGS = -I./src/lib/linkedlist -DDATA_DIR=\"${WordConsciousdatadir}\" -DCONF_DIR=\"${WordConsciousconfdir}\"

#Add words file
WordConsciousdata_DATA = words.txt

#Add Configuration file
WordConsciousconf_DATA = WordConscious.conf

#Linkedlist Library
lib_LTLIBRARIES = libWC_linkedlist.la

#Linkedlist Sources
libWC_linkedlist_la_SOURCES = \
    src/lib/linkedlist/WC_linkedlist.c src/lib/linkedlist/WC_linkedlist.h

#Linkedlist Version
libWC_linkedlist_la_LDFLAGS = -version-info 0:0:0 -no-undefined

libWC_linkedlist_la_LIBADD = @WC_sodium_flags@

#Install linked list headers
include_HEADERS = src/lib/linkedlist/WC_linkedlist.h
