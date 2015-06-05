/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: leandre.china@gmail.com                                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_EVENTLOG_H
#define PHP_EVENTLOG_H

extern zend_module_entry eventlog_module_entry;
#define phpext_eventlog_ptr &eventlog_module_entry

#define PHP_EVENTLOG_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_EVENTLOG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_EVENTLOG_API __attribute__ ((visibility("default")))
#else
#	define PHP_EVENTLOG_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#ifdef ZTS
#define EVENTLOG_G(v) ZEND_TSRMG(eventlog_globals_id, zend_eventlog_globals *, v)
#ifdef COMPILE_DL_EVENTLOG
ZEND_TSRMLS_CACHE_EXTERN();
#endif
#else
#define EVENTLOG_G(v) (eventlog_globals.v)
#endif

#define EVENTLOG_CLASS_NAME       "Eventlog"
#define EVENTLOG_VERSION          "0.1"
#define EVENTLOG_AUTHOR           "leandre.china@gmail.com"

#define EVENTLOG_GETPV          "getpv"
#define EVENTLOG_GETUV          "getuv"
#define EVENTLOG_ANALYZE_COUNT  "analyze_count"
#define EVENTLOG_ANALYZE_DETAIL "analyze_detail"

PHP_MINIT_FUNCTION(eventlog);
PHP_RINIT_FUNCTION(eventlog);
PHP_RSHUTDOWN_FUNCTION(eventlog);
PHP_MSHUTDOWN_FUNCTION(eventlog);
PHP_MINFO_FUNCTION(eventlog);

PHP_FUNCTION(eventlog_get_version);
PHP_FUNCTION(eventlog_get_author);

zend_class_entry *eventlog_ce;

PHP_METHOD(EVENTLOG_CLASS_NAME, __construct);
PHP_METHOD(EVENTLOG_CLASS_NAME, __destruct);
PHP_METHOD(EVENTLOG_CLASS_NAME, setpath);
PHP_METHOD(EVENTLOG_CLASS_NAME, getpath);
PHP_METHOD(EVENTLOG_CLASS_NAME, EVENTLOG_GETPV);
PHP_METHOD(EVENTLOG_CLASS_NAME, EVENTLOG_GETUV);
PHP_METHOD(EVENTLOG_CLASS_NAME, EVENTLOG_ANALYZE_COUNT);
PHP_METHOD(EVENTLOG_CLASS_NAME, EVENTLOG_ANALYZE_DETAIL);

ZEND_BEGIN_MODULE_GLOABLES(eventlog)
    char *default_path;
ZEND_END_MODULE_GLOABLES(eventlog)

extern ZEND_DECLARE_MODULE_GLOBALS(eventlog);

#endif	/* PHP_EVENTLOG_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
