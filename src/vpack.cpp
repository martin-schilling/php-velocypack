#include "vpack.h"

namespace velocypack { namespace php {

    zend_object_handlers Vpack::handler_vpack = zend_object_handlers();

    zend_object* Vpack::create_object(zend_class_entry* ce TSRMLS_CC)
    {
        Vpack* intern = (Vpack*)ecalloc(1, sizeof(Vpack) + zend_object_properties_size(ce));

        zend_object_std_init(&intern->std, ce TSRMLS_CC);

        intern->std.handlers = &Vpack::handler_vpack;
        return &intern->std;
    }

    Vpack* Vpack::fetch_object(zend_object* obj)
    {
        return (Vpack *)((char *)obj - XtOffsetOf(Vpack, std));
    }

    void Vpack::from_json(const char* json, size_t size)
    {
        vp::Parser parser;

        //try {
            parser.parse(json, size);
        //}
        //catch(std::bad_alloc const &e) {
            /* @todo add exception */
        //}
        //catch(vp::Exception const &e) {
            /* @todo add exception */
        //}

        this->builder = *parser.steal();
    }

    void Vpack::from_array(HashTable* array)
    {
        this->builder = vp::Builder();
        this->php_array_to_vpack(array, &this->builder);
    }

    std::string Vpack::to_json()
    {
        //try {
            std::string json;

            vp::StringSink sink(&json);
            vp::Options dumperOptions;

            vp::Dumper dumper(&sink, &dumperOptions);
            dumper.dump(this->builder.slice());

            return json;
        //}
        //catch(vp::Exception const &e) {
            /* @todo add exception */
        //}
    }

    std::string Vpack::to_hex()
    {
        return vp::HexDump(this->builder.slice()).toString().c_str();
    }

    void Vpack::access(zval* return_value, HashTable* accessor)
    {
        //try {
            zval* value;
            vp::Slice tmpSlice(this->builder.slice());

            ZEND_HASH_FOREACH_VAL(accessor, value) {

                if((Z_TYPE_P(value) == IS_LONG)) {
                    tmpSlice = vp::Slice(tmpSlice.at(Z_LVAL_P(value)));
                } else if(Z_TYPE_P(value) == IS_STRING) {
                    tmpSlice = vp::Slice(tmpSlice.get(Z_STRVAL_P(value)));
                } else {
                    //@todo exception
                }

            } ZEND_HASH_FOREACH_END();

            this->vpack_to_php_value(tmpSlice, return_value);
        //}
        //catch(const vp::Exception& e) {
            //@todo exception
        //}
    }

    void Vpack::access(zval* return_value, const char* accessor)
    {
        //try {
            this->vpack_to_php_value(this->builder.slice().get(accessor), return_value);
        //}
        //catch(const vp::Exception& e) {
            //@todo exception
        //}
    }

    void Vpack::access(zval* return_value, int accessor)
    {
        this->vpack_to_php_value(this->builder.slice().at(accessor), return_value);
    }



    void Vpack::vpack_to_php_value(const vp::Slice& slice, zval* value)
    {
        switch(slice.type()) {
            case vp::ValueType::String:
                {
                    std::string slice_string = slice.copyString();
                    ZVAL_STRING(value, slice_string.c_str());
                    break;
                }

            case vp::ValueType::Int:
            case vp::ValueType::UInt:
            case vp::ValueType::SmallInt:
                ZVAL_LONG(value, slice.getInt());
                break;

            case vp::ValueType::Double:
                ZVAL_DOUBLE(value, slice.getDouble())
                break;

            case vp::ValueType::Null:
                ZVAL_NULL(value);
                break;

            case vp::ValueType::Array:
                {
                    array_init(value);
                    for (auto const& it : vp::ArrayIterator(slice)) {
                        zval tmp;
                        Vpack::vpack_to_php_value(it, &tmp);
                        add_next_index_zval(value, &tmp);
                    }
                }
                break;

            case vp::ValueType::Object:
                {
                    array_init(value);
                    for(auto const& it : vp::ObjectIterator(slice)) {
                        vp::ValueLength key_len;
                        auto key = it.key.getString(key_len);

                        zval tmp;
                        Vpack::vpack_to_php_value(it.value, &tmp);
                        add_assoc_zval_ex(value, key, key_len, &tmp);
                    }
                }
                break;

            case vp::ValueType::UTCDate:
            case vp::ValueType::None:
            case vp::ValueType::Binary:
            case vp::ValueType::Illegal:
            case vp::ValueType::MinKey:
            case vp::ValueType::MaxKey:
                ZVAL_NULL(value);
                break;

            default:
                //@todo exception
                break;
        }
    }

    void Vpack::php_array_to_vpack(HashTable* array, vp::Builder* builder)
    {
        if(HT_IS_PACKED(array) && HT_IS_WITHOUT_HOLES(array)) {
            builder->add(vp::Value(vp::ValueType::Array));
            Vpack::php_numeric_array_to_vpack(array, builder);
            builder->close();
        } else {
            builder->add(vp::Value(vp::ValueType::Object));
            Vpack::php_assoc_array_to_vpack(array, builder);
            builder->close();
        }
    }

    void Vpack::php_numeric_array_to_vpack(HashTable* array, vp::Builder* builder)
    {
        zval* data;

        ZEND_HASH_FOREACH_VAL(array, data) {

            switch(Z_TYPE_P(data)) {
                case IS_LONG:
                    builder->add(vp::Value(Z_LVAL_P(data)));
                    break;
                case IS_STRING:
                    builder->add(vp::Value(Z_STRVAL_P(data)));
                    break;
                case IS_DOUBLE:
                    builder->add(vp::Value(Z_DVAL_P(data)));
                    break;
                case IS_TRUE:
                    builder->add(vp::Value(true));
                    break;
                case IS_FALSE:
                    builder->add(vp::Value(false));
                    break;
                case IS_NULL:
                    builder->add(vp::Value(vp::ValueType::Null));
                    break;
                case IS_ARRAY:
                    if(HT_IS_PACKED(Z_ARRVAL_P(data)) && HT_IS_WITHOUT_HOLES(Z_ARRVAL_P(data))) {
                        builder->add(vp::Value(vp::ValueType::Array));
                        Vpack::php_numeric_array_to_vpack(Z_ARRVAL_P(data), builder);
                        builder->close();
                    } else {
                        builder->add(vp::Value(vp::ValueType::Object));
                        Vpack::php_assoc_array_to_vpack(Z_ARRVAL_P(data), builder);
                        builder->close();
                    }
                    break;
                case IS_OBJECT:
                    builder->add(vp::Value(vp::ValueType::Object));
                    builder->close();
                    break;
                default:
                    break;
           }

        } ZEND_HASH_FOREACH_END();
    }

    void Vpack::php_assoc_array_to_vpack(HashTable* array, vp::Builder* builder)
    {
        zend_string* key;
        zval* data;
        zend_ulong index;

        char numberBuffer[20];
        char* vpackKey;

        ZEND_HASH_FOREACH_KEY_VAL_IND(array, index, key, data) {

            if(key) {
                vpackKey = ZSTR_VAL(key);
            } else {
                snprintf(numberBuffer, sizeof(numberBuffer), "%d", index);
                vpackKey = numberBuffer;
            }

            switch(Z_TYPE_P(data)) {
                case IS_LONG:
                    builder->add(vpackKey, vp::Value(Z_LVAL_P(data)));
                    break;
                case IS_STRING:
                    builder->add(vpackKey, vp::Value(Z_STRVAL_P(data)));
                    break;
                case IS_DOUBLE:
                    builder->add(vpackKey, vp::Value(Z_DVAL_P(data)));
                    break;
                case IS_TRUE:
                    builder->add(vpackKey, vp::Value(true));
                    break;
                case IS_FALSE:
                    builder->add(vpackKey, vp::Value(false));
                    break;
                case IS_NULL:
                    builder->add(vpackKey, vp::Value(vp::ValueType::Null));
                    break;
                case IS_ARRAY:
                    if(HT_IS_PACKED(Z_ARRVAL_P(data)) && HT_IS_WITHOUT_HOLES(Z_ARRVAL_P(data))) {
                        builder->add(vpackKey, vp::Value(vp::ValueType::Array));
                        Vpack::php_numeric_array_to_vpack(Z_ARRVAL_P(data), builder);
                        builder->close();
                    } else {
                        builder->add(vpackKey, vp::Value(vp::ValueType::Object));
                        Vpack::php_assoc_array_to_vpack(Z_ARRVAL_P(data), builder);
                        builder->close();
                    }
                    break;
                case IS_OBJECT: //for now objects will just result in an empty json object
                    builder->add(vpackKey, vp::Value(vp::ValueType::Object));
                    builder->close();
                    break;
                default:
                    break;
            }

        } ZEND_HASH_FOREACH_END();
    }
}}
