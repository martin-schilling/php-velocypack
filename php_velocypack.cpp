#include "php_velocypack.h"

zend_class_entry *vpack_interface_ce;
zend_class_entry *vpack_ce;
zend_class_entry *vpack_exception_ce;
zend_class_entry *vpack_invalid_argument_exception_ce;

#define Z_OBJECT_VPACK(zo) (velocypack::php::Vpack::fetch_object(zo))
#define Z_OBJECT_VPACK_P(zo) (velocypack::php::Vpack::fetch_object(Z_OBJ_P(zo)))

#include "php_exception.h"
#include "php_vpack_interface.h"
#include "php_vpack.h"

PHP_MINIT_FUNCTION(velocypack)
{
    init_vpack_exception_ces();
    init_vpack_interface_ce();
    init_vpack_ce();

    REGISTER_LONG_CONSTANT("Velocypack\\Option\\BUILD_UNINDEXED_OBJECTS", 0, CONST_PERSISTENT | CONST_CS);
    REGISTER_LONG_CONSTANT("Velocypack\\Option\\BUILD_UNINDEXED_ARRAYS", 1, CONST_PERSISTENT | CONST_CS);

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
