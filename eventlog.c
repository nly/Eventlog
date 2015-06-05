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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

#include "php_eventlog.h"
#include "ext/date/php_date.h"
#include "zend_extensions.h"
#include <stdlib.h>
#include <unistd.h>

ZEND_DECLARE_MODULE_GLOBALS(eventlog)

/* True global resources - no need for thread safety here */
static int le_eventlog;

/**
 * local functions define
 */
static long get_count(char *log_path, char *key_word, int ip_pos TSRMLS_DC);
static int check_dir(char *log_dir TSRMLS_DC);
static char * format(char *str TSRMLS_DC);

/* {{{ eventlog_functions[]
 *
 * Every user visible function must have an entry in eventlog_functions[].
 */
const zend_function_entry eventlog_functions[] = {
    PHP_FE(eventlog_get_version, NULL)
    PHP_FE(eventlog_get_author, NULL)

    PHP_FE_END  /* Must be the last line in eventlog_functions[] */
};
/* }}} */

// event_methods[]
const zend_function_entry eventlog_methods[] = {
    PHP_ME(EVENTLOG_CLASS_NAME, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(EVENTLOG_CLASS_NAME, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)

    PHP_ME(EVENTLOG_CLASS_NAME, setpath, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(EVENTLOG_CLASS_NAME, getpath, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(EVENTLOG_CLASS_NAME, setlogger, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(EVENTLOG_CLASS_NAME, getlogger, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)

    PHP_ME(EVENTLOG_CLASS_NAME, getpv, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(EVENTLOG_CLASS_NAME, getuv, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(EVENTLOG_CLASS_NAME, EVENTLOG_ANALYZE_COUNT, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(EVENTLOG_CLASS_NAME, EVENTLOG_ANALYZE_DETAIL, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)

    PHP_FE_END
};

/* {{{ eventlog_module_entry
 */
zend_module_entry eventlog_module_entry = {
    STANDARD_MODULE_HEADER,
    EVENTLOG_CLASS_NAME,
    eventlog_functions,
    PHP_MINIT(eventlog),
    PHP_MSHUTDOWN(eventlog),
    PHP_RINIT(eventlog),        /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(eventlog),    /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(eventlog),
    EVENTLOG_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_EVENTLOG
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(eventlog)
#endif

/* {{{ PHP_INI
 */

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("eventlog.base_path", "/log", PHP_INI_ALL, OnUpdateString, base_path, zend_eventlog_globals, eventlog_globals)
    STD_PHP_INI_ENTRY("eventlog.logger", "default", PHP_INI_ALL, OnUpdateString, logger, zend_eventlog_globals, eventlog_globals)
PHP_INI_END()

/* }}} */

/* }}} */

/* {{{ php_eventlog_init_globals
 */

static void php_eventlog_init_globals(zend_eventlog_globals *eventlog_globals)
{
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(eventlog)
{
	REGISTER_INI_ENTRIES();
    zend_class_entry eventlog;
    ZEND_INIT_MODULE_GLOBALS(eventlog, php_eventlog_init_globals, NULL);

    REGISTER_STRINGL_CONSTANT("EVENTLOG_VERSION", EVENTLOG_VERSION, sizeof(EVENTLOG_VERSION) - 1, CONST_PERSISTENT | CONST_CS);

    INIT_CLASS_ENTRY(eventlog, EVENTLOG_CLASS_NAME, eventlog_methods);
    eventlog_ce = zend_register_internal_class_ex(&eventlog, NULL, NULL TSRMLS_CC);
    eventlog_ce->ce_flags = ZEND_ACC_IMPLICIT_PUBLIC;
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(eventlog)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(eventlog)
{
#if defined(COMPILE_DL_EVENTLOG) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(eventlog)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(eventlog)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Eventlog support", "enabled");
    php_info_print_table_header(2, "Version", EVENTLOG_VERSION);
    php_info_print_table_header(2, "Author", EVENTLOG_AUTHOR);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

PHP_FUNCTION(eventlog_get_version) {
    char * str;
    int len = 0;
    len = spprintf(&str, 0, "%s", EVENTLOG_VERSION);
    RETURN_STRINGL(str, len, 0);
}

PHP_FUNCTION(eventlog_get_author) {
    char * str;
    int len = 0;
    len = spprintf(&str, 0, "%s", EVENTLOG_AUTHOR);
    RETURN_STRINGL(str, len, 0);
}

PHP_METHOD(EVENTLOG_CLASS_NAME, __construct) {
    return;
}

PHP_METHOD(EVENTLOG_CLASS_NAME, __destruct) {
    return;
}

PHP_METHOD(EVENTLOG_CLASS_NAME, setpath) {
    int argc = ZEND_NUM_ARGS();
    char *path;
    int path_len;
    if(zend_parse_parameters(argc TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
        RETURN_FALSE;
    }

    if(argc > 0) {
        EVENTLOG_G(base_path) = pemalloc(path_len + 1, 1);
        memcpy(EVENTLOG_G(base_path), path, path_len + 1);
        RETURN_TRUE;
    }

    RETURN_FALSE;
}

PHP_METHOD(EVENTLOG_CLASS_NAME, getpath) {
    char *str;
    int len;
    len = spprintf(&str, 0, "%s", EVENTLOG_G(base_path));
    RETURN_STRINGL(str, len, 0);
}

PHP_METHOD(EVENTLOG_CLASS_NAME, setlogger) {
    int argc = ZEND_NUM_ARGS();
    char *path;
    int path_len;
    if(zend_parse_parameters(argc TSRMLS_CC, "s", &path, &path_len) == FAILURE) {
        RETURN_FALSE;
    }

    if(argc > 0) {
        EVENTLOG_G(logger) = pemalloc(path_len + 1, 1);
        memcpy(EVENTLOG_G(logger), path, path_len + 1);
        RETURN_TRUE;
    }

    RETURN_FALSE;
}

PHP_METHOD(EVENTLOG_CLASS_NAME, getlogger) {
    char *str;
    int len;
    len = spprintf(&str, 0, "%s", EVENTLOG_G(logger));
    RETURN_STRINGL(str, len, 0);
}

PHP_METHOD(EVENTLOG_CLASS_NAME, getpv) {
    int argc = ZEND_NUM_ARGS();
    char *log_path = "", *key_word = "", *log_dir;
    int log_path_len, key_word_len;
    long count;

    if(zend_parse_parameters(argc TSRMLS_CC, "|ss", &log_path, &log_path_len, &key_word, &key_word_len) == FAILURE) {
        RETURN_FALSE;
    }

    spprintf(&log_dir, 0, "%s/%s", EVENTLOG_G(base_path), EVENTLOG_G(logger));
    if(check_dir(log_dir TSRMLS_CC) == 0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Log dir [%s] is not readable", log_dir);
    }
    efree(log_dir);

    count = get_count(log_path, key_word, 0 TSRMLS_CC);
    RETURN_LONG(count);
}

PHP_METHOD(EVENTLOG_CLASS_NAME, getuv) {
    int argc = ZEND_NUM_ARGS();
    char *log_path = "", *key_word = "", *log_dir;
    int log_path_len, key_word_len;
    long count, ip_pos = 1;

    if(zend_parse_parameters(argc TSRMLS_CC, "|sls", &log_path, &log_path_len, &ip_pos, &key_word, &key_word_len) == FAILURE) {
        RETURN_FALSE;
    }

    spprintf(&log_dir, 0, "%s/%s", EVENTLOG_G(base_path), EVENTLOG_G(logger));
    if(check_dir(log_dir TSRMLS_CC) == 0) {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "Log dir [%s] is not readable", log_dir);
    }
    efree(log_dir);

    count = get_count(log_path, key_word, ip_pos TSRMLS_CC);
    RETURN_LONG(count);
}

PHP_METHOD(EVENTLOG_CLASS_NAME, EVENTLOG_ANALYZE_COUNT) {

    RETURN_FALSE;
}

PHP_METHOD(EVENTLOG_CLASS_NAME, EVENTLOG_ANALYZE_DETAIL) {
    RETURN_FALSE;
}

/**
 * local functions
 */
static long get_count(char *log_path, char *key_word, int ip_pos TSRMLS_DC) {
    FILE *fp;
    char buffer[BUFSIZ];
    char *path, *sh;
    long count;
    int is_dir;

    spprintf(&path, 0, "%s/%s/%s*", EVENTLOG_G(base_path), EVENTLOG_G(logger), log_path);

    if (key_word[0]) {
        spprintf(&sh, 0, "more %s | grep '%s' -c", path, key_word);
        if(ip_pos) {
            spprintf(&sh, 0, "more %s | grep '%s' | awk '{print $%d}' | sort -n | uniq | wc -l", path, key_word, ip_pos);
        }
    } else {
        spprintf(&sh, 0, "more %s | wc -l ", path);
        if(ip_pos) {
            spprintf(&sh, 0, "more %s | awk '{print $%d}' | sort -n | uniq | wc -l ", path, ip_pos);
        }
    }

    fp = VCWD_POPEN(sh, "r");
    if(!fp) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to fork pipe [%s]", sh);
    } else {
        char *temp_p = fgets(buffer, sizeof(buffer), fp);
        pclose(fp);
    }

    count = atoi(format(buffer TSRMLS_CC));

    efree(path);
    efree(sh);

    return count;
}

/**
 * check if the log dir can readable
 * @param  log_dir
 * @return boolean
 */
static int check_dir(char *log_dir TSRMLS_DC) {
    return (access(log_dir, R_OK) == 0) ? 1 : 0;
}

/**
 * format pipe result
 * @param  str
 * @return str
 */
static char * format(char *str TSRMLS_DC) {
    int len;
    len = strlen(str);
    str[len - 1] = '\0';
    return str;
}