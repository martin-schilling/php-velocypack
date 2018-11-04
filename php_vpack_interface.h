#pragma once

extern "C" {
#include <php.h>
#include <Zend/zend_interfaces.h>

#if PHP_VERSION_ID < 72000
#include <ext/spl/spl_iterators.h>
#endif
}

namespace {
    ZEND_BEGIN_ARG_INFO_EX(velocypack_vpack_interface_void, 0, 0, 0)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(velocypack_vpack_interface_from_json, 0, 0, 1)
        ZEND_ARG_INFO(0, json)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(velocypack_vpack_interface_from_array, 0, 0, 1)
        ZEND_ARG_INFO(0, array)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(velocypack_vpack_interface_access, 0, 0, 1)
        ZEND_ARG_INFO(0, accessor)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(velocypack_vpack_interface_offset_get, 0, 0, 1)
        ZEND_ARG_INFO(0, key)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(velocypack_vpack_interface_offset_set, 0, 0, 2)
        ZEND_ARG_INFO(0, key)
        ZEND_ARG_INFO(0, value)
    ZEND_END_ARG_INFO()


    zend_function_entry vpack_interface_methods[] = {
        ZEND_ABSTRACT_ME(VpackInterface, __construct, velocypack_vpack_interface_void)
        ZEND_FENTRY(fromBinary, NULL, velocypack_vpack_interface_from_json, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT | ZEND_ACC_STATIC)
        ZEND_FENTRY(fromJson, NULL, velocypack_vpack_interface_from_json, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT | ZEND_ACC_STATIC)
        ZEND_FENTRY(fromArray, NULL, velocypack_vpack_interface_from_array, ZEND_ACC_PUBLIC | ZEND_ACC_ABSTRACT | ZEND_ACC_STATIC)
        ZEND_ABSTRACT_ME(VpackInterface, toJson, velocypack_vpack_interface_void)
        ZEND_ABSTRACT_ME(VpackInterface, toHex, velocypack_vpack_interface_void)
        ZEND_ABSTRACT_ME(VpackInterface, toBinary, velocypack_vpack_interface_void)
        ZEND_ABSTRACT_ME(VpackInterface, toArray, velocypack_vpack_interface_void)
        ZEND_ABSTRACT_ME(VpackInterface, access, velocypack_vpack_interface_access)
        /* ArrayAccess */
        ZEND_ABSTRACT_ME(VpackInterface, offsetExists, velocypack_vpack_interface_offset_get)
        ZEND_ABSTRACT_ME(VpackInterface, offsetGet, velocypack_vpack_interface_offset_get)
        ZEND_ABSTRACT_ME(VpackInterface, offsetSet, velocypack_vpack_interface_offset_set)
        ZEND_ABSTRACT_ME(VpackInterface, offsetUnset, velocypack_vpack_interface_offset_get)
        /* Countable */
        ZEND_ABSTRACT_ME(VpackInterface, count, velocypack_vpack_interface_void)
        PHP_FE_END
    };

    void init_vpack_interface_ce()
    {
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "Velocypack\\VpackInterface", vpack_interface_methods);
        vpack_interface_ce = zend_register_internal_interface(&ce TSRMLS_CC);

        zend_class_implements(vpack_interface_ce, 1, zend_ce_arrayaccess);

        #if PHP_VERSION_ID < 72000
        zend_class_implements(vpack_interface_ce, 1, spl_ce_Countable);
        #else
        zend_class_implements(vpack_interface_ce, 1, zend_ce_countable);
        #endif
    }
}
