#pragma once

extern "C" {
#include <php.h>
}

#include "src/vpack.h"

namespace {

    PHP_METHOD(Vpack, __construct)
    {
        velocypack::php::Vpack* intern;

        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        intern = Z_OBJECT_VPACK_P(getThis());
    }

    PHP_METHOD(Vpack, fromBinary)
    {
        char* binary;
        size_t len;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &binary, &len) == FAILURE) {
            return;
        }

        object_init_ex(return_value, vpack_ce);
        auto intern = Z_OBJECT_VPACK(Z_OBJ_P(return_value));

        intern->from_binary(binary, len);
    }

    PHP_METHOD(Vpack, fromJson)
    {
        char* json;
        size_t len;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &json, &len) == FAILURE) {
            return;
        }

        object_init_ex(return_value, vpack_ce);
        auto intern = Z_OBJECT_VPACK(Z_OBJ_P(return_value));

        intern->from_json(json, len);
    }

    PHP_METHOD(Vpack, fromArray)
    {
        zval *array_value;
        HashTable *array;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &array_value) == FAILURE) {
            return;
        }

        array = Z_ARRVAL_P(array_value);

        object_init_ex(return_value, vpack_ce);
        auto intern = Z_OBJECT_VPACK(Z_OBJ_P(return_value));

        intern->from_array(array);
    }

    PHP_METHOD(Vpack, toJson)
    {
        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        velocypack::php::Vpack* intern = Z_OBJECT_VPACK_P(getThis());

        auto json = intern->to_json();
        RETURN_STRING(json.c_str());
    }

    PHP_METHOD(Vpack, toHex)
    {
        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        velocypack::php::Vpack* intern = Z_OBJECT_VPACK_P(getThis());
        auto hex = intern->to_hex();

        RETURN_STRING(hex.c_str());
    }

    PHP_METHOD(Vpack, toBinary)
    {
        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        velocypack::php::Vpack* intern = Z_OBJECT_VPACK_P(getThis());
        auto binary = intern->to_binary();

        RETURN_STRING(binary.c_str());
    }

    PHP_METHOD(Vpack, toArray)
    {
        if(zend_parse_parameters_none() == FAILURE) {
            return;
        }

        velocypack::php::Vpack* intern = Z_OBJECT_VPACK_P(getThis());
        intern->to_array(return_value);
    }

    PHP_METHOD(Vpack, access)
    {
        zval *accessor;
        HashTable *accessor_ht;
        const char* accessor_string;
        int accessor_number;

        if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &accessor) == FAILURE) {
            return;
        }

        velocypack::php::Vpack* intern = Z_OBJECT_VPACK_P(getThis());

        if(Z_TYPE_P(accessor) == IS_STRING) {
            accessor_string = Z_STRVAL_P(accessor);
            intern->access(return_value, accessor_string);
        } else if(Z_TYPE_P(accessor) == IS_LONG) {
            accessor_number = Z_LVAL_P(accessor);
            intern->access(return_value, accessor_number);
        } else if(Z_TYPE_P(accessor) == IS_ARRAY) {
            accessor_ht = Z_ARRVAL_P(accessor);
            intern->access(return_value, accessor_ht);
        } else {
            //@todo exception
        }
    }

    ZEND_BEGIN_ARG_INFO_EX(velocypack_vpack_void, 0, 0, 0)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(velocypack_vpack_from_json, 0, 0, 1)
        ZEND_ARG_INFO(0, json)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(velocypack_vpack_from_array, 0, 0, 1)
        ZEND_ARG_INFO(0, array)
    ZEND_END_ARG_INFO()

    ZEND_BEGIN_ARG_INFO_EX(velocypack_vpack_access, 0, 0, 1)
        ZEND_ARG_INFO(0, accessor)
    ZEND_END_ARG_INFO()

    zend_function_entry vpack_methods[] = {
        PHP_ME(Vpack, __construct, velocypack_vpack_void, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
        PHP_ME(Vpack, fromBinary, velocypack_vpack_from_json, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_ME(Vpack, fromJson, velocypack_vpack_from_json, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_ME(Vpack, fromArray, velocypack_vpack_from_array, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_ME(Vpack, toJson, velocypack_vpack_void, ZEND_ACC_PUBLIC)
        PHP_ME(Vpack, toHex, velocypack_vpack_void, ZEND_ACC_PUBLIC)
        PHP_ME(Vpack, toBinary, velocypack_vpack_void, ZEND_ACC_PUBLIC)
        PHP_ME(Vpack, toArray, velocypack_vpack_void, ZEND_ACC_PUBLIC)
        PHP_ME(Vpack, access, velocypack_vpack_access, ZEND_ACC_PUBLIC)
        PHP_FE_END
    };

    void init_vpack_ce()
    {
        zend_class_entry ce;

        INIT_CLASS_ENTRY(ce, "Velocypack\\Vpack", vpack_methods);
        vpack_ce = zend_register_internal_class(&ce TSRMLS_CC);
        vpack_ce->create_object = velocypack::php::Vpack::create_object;

        memcpy(&velocypack::php::Vpack::handler_vpack, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
        velocypack::php::Vpack::handler_vpack.offset = XtOffsetOf(velocypack::php::Vpack, std);
    }
}