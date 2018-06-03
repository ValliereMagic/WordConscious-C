#Init the package
AC_INIT([WordConscious], [0.0.1])

#Minimum version of autoconf
AC_PREREQ([2.68])

#File must exist for the build to continue
AC_CONFIG_SRCDIR([src/main.c])

#Location for header file for make config
AC_CONFIG_HEADERS([include/config.h])

#Auxiliary build files
AC_CONFIG_AUX_DIR([build-aux])

#Init automake
AM_INIT_AUTOMAKE([1.11 -Wall -Werror foreign subdir-objects])

#Configure creates makefile
AC_CONFIG_FILES([Makefile])

#Find and probe C compiler
AC_PROG_CC

#Include the include file for libsodium
CFLAGS="-I./lib/libsodium/include"

#add various checks here

AC_OUTPUT