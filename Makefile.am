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
WordConscious_LDADD = liblinkedlist.la

#Add libsodium CFLAGS
WordConscious_CFLAGS = -I./src/lib/linkedlist

#Linkedlist Library
lib_LTLIBRARIES = liblinkedlist.la

#Linkedlist Sources
liblinkedlist_la_SOURCES = \
    src/lib/linkedlist/linkedlist.c src/lib/linkedlist/linkedlist.h

#Linkedlist Version
liblinkedlist_la_LDFLAGS = -version-info 0:0:1
