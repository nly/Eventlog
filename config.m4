dnl $Id$
dnl config.m4 for extension eventlog

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(eventlog, for eventlog support,
dnl Make sure that the comment is aligned:
[  --with-eventlog             Include eventlog support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(eventlog, whether to enable eventlog support,
dnl Make sure that the comment is aligned:
dnl [  --enable-eventlog           Enable eventlog support])

if test "$PHP_EVENTLOG" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-eventlog -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/eventlog.h"  # you most likely want to change this
  dnl if test -r $PHP_EVENTLOG/$SEARCH_FOR; then # path given as parameter
  dnl   EVENTLOG_DIR=$PHP_EVENTLOG
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for eventlog files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       EVENTLOG_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$EVENTLOG_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the eventlog distribution])
  dnl fi

  dnl # --with-eventlog -> add include path
  dnl PHP_ADD_INCLUDE($EVENTLOG_DIR/include)

  dnl # --with-eventlog -> check for lib and symbol presence
  dnl LIBNAME=eventlog # you may want to change this
  dnl LIBSYMBOL=eventlog # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $EVENTLOG_DIR/$PHP_LIBDIR, EVENTLOG_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_EVENTLOGLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong eventlog lib version or lib not found])
  dnl ],[
  dnl   -L$EVENTLOG_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(EVENTLOG_SHARED_LIBADD)

  PHP_NEW_EXTENSION(eventlog, eventlog.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
