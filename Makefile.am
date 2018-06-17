#Include M4 Macros
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = WordConscious

#Program Sources
WordConscious_SOURCES = \
    src/bin/main.c \
    src/bin/tests.c src/bin/tests.h \
    src/bin/words.c src/bin/words.h \
    src/lib/linkedlist/linkedlist.h

#Link libsodium
WordConscious_LDADD = libWC_linkedlist.la @WC_sodium_flags@ @WC_pthread_flags@

#define data directory
WordConsciousdatadir=@datadir@/WordConscious

#Add CFLAGS
WordConscious_CFLAGS = -I./src/lib/linkedlist -DDATA_DIR=\"${WordConsciousdatadir}\"

#Add words file
WordConsciousdata_DATA = words.txt

#Linkedlist Library
lib_LTLIBRARIES = libWC_linkedlist.la

#Linkedlist Sources
libWC_linkedlist_la_SOURCES = \
    src/lib/linkedlist/linkedlist.c src/lib/linkedlist/linkedlist.h

#Linkedlist Version
libWC_linkedlist_la_LDFLAGS = -version-info 0:0:0 -no-undefined

libWC_linkedlist_la_LIBADD = @WC_sodium_flags@

#Install linked list headers
include_HEADERS = src/lib/linkedlist/linkedlist.h
