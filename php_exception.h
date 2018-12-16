#pragma once

extern "C" {
#include <php.h>
#include <Zend/zend_exceptions.h>
}

extern zend_class_entry *vpack_exception_ce;

/*
    Exception hierarchy

    \Exception
        \Velocypack\Exception
            \Velocypack\InvalidArgumentException
*/

#define VELOCYPACK_EXCEPTION_CONVERTER_TRY \
    try {


#define VELOCYPACK_EXCEPTION_CONVERTER_CATCH \
    } catch(const std::invalid_argument& ex) { \
        zend_throw_exception_ex(vpack_invalid_argument_exception_ce, 0, ex.what()); \
        return; \
    } catch(...) { \
        zend_throw_exception_ex(vpack_exception_ce, 0, "An unknown exception occurred."); \
        return; \
    }


namespace {

    void init_vpack_exception_ce();
    void init_vpack_invalid_argument_exception_ce();

    void init_vpack_exception_ces()
    {
        init_vpack_exception_ce();
        init_vpack_invalid_argument_exception_ce();
    }

    void init_vpack_exception_ce()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "Velocypack\\Exception", NULL);
        vpack_exception_ce = zend_register_internal_class_ex(&ce, zend_exception_get_default());
    }

    void init_vpack_invalid_argument_exception_ce()
    {
        zend_class_entry ce;
        INIT_CLASS_ENTRY(ce, "Velocypack\\InvalidArgumentException", NULL);
        vpack_invalid_argument_exception_ce = zend_register_internal_class_ex(&ce, zend_exception_get_default());
    }

}
