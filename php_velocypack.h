#ifndef PHP_VELOCYPACK_H
#define PHP_VELOCYPACK_H

#define PHP_VELOCYPACK_EXTNAME  "velocypack"
#define PHP_VELOCYPACK_EXTVER   "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>

extern zend_module_entry velocypack_module_entry;
#define phpext_velocypack_ptr &velocypack_module_entry;

#endif /* PHP_VELOCYPACK_H */
