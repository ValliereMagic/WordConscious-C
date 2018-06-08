bin_PROGRAMS = WordConscious

#Program Sources
WordConscious_SOURCES = \
    include/main.h include/tests.h include/words.h \
    include/linkedlist.h lib/libsodium/include/sodium.h \
    \
    src/main.c src/linkedlist.c src/tests.c src/words.c

#Link libsodium
WordConscious_LDADD = ./lib/libsodium/libsodium.a

#Add libsodium CFLAGS
WordConscious_CFLAGS = -I./lib/libsodium/include