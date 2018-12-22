#pragma once

extern "C" {
#include <php.h>
}

#include <velocypack/vpack.h>

namespace vp = ::arangodb::velocypack;

namespace velocypack { namespace php {

    class Vpack
    {
    public:
        vp::Builder builder;
        zend_object std;

        static zend_object_handlers handler_vpack;
        static zend_object* create_object(zend_class_entry* ce TSRMLS_CC);
        static Vpack* fetch_object(zend_object* obj);

        Vpack();
        Vpack(vp::Builder builder);

        void from_binary(const char* binary, size_t size);
        void from_json(const char* json, size_t size);
        void from_array(HashTable* array);

        std::string to_json();
        std::string to_hex();
        std::string to_binary();
        zval* to_array(zval* return_value);

        void access(zval* return_value, HashTable* accessor);
        void access(zval* return_value, const char* accessor);
        void access(zval* return_value, int accessor);

        bool exists(const char* accessor);
        bool exists(int accessor);

        int count();

    private:
        static void vpack_to_php_value(const vp::Slice& slice, zval* value);
        static void php_array_to_vpack(HashTable* array, vp::Builder* builder);
        static void php_numeric_array_to_vpack(HashTable* array, vp::Builder* builder);
        static void php_assoc_array_to_vpack(HashTable* array, vp::Builder* builder);
        static bool php_array_is_numeric(HashTable* array);
    };

}}
