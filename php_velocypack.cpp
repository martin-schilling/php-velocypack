#include "php_velocypack.h"

zend_class_entry *vpack_ce;

#define Z_OBJECT_VPACK(zo) (velocypack::php::Vpack::fetch_object(zo))
#define Z_OBJECT_VPACK_P(zo) (velocypack::php::Vpack::fetch_object(Z_OBJ_P(zo)))

#include "php_vpack.h"

PHP_MINIT_FUNCTION(velocypack)
{
    init_vpack_ce();

    return SUCCESS;
}

zend_module_entry velocypack_module_entry = {

#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_VELOCYPACK_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(velocypack),
    NULL,                  /* MSHUTDOWN */
    NULL,                  /* RINIT */
    NULL,                  /* RSHUTDOWN */
    NULL,                  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_VELOCYPACK_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_VELOCYPACK
extern "C" {
ZEND_GET_MODULE(velocypack)
}
#endif
