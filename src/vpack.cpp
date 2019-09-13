#include "vpack.h"

extern zend_class_entry *vpack_ce;

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

    Vpack::Vpack()
    {
    }

    Vpack::Vpack(vp::Builder builder): builder(builder)
    {
    }

    void Vpack::set_options(HashTable* options)
    {
        zend_ulong key;
        zval* data;

        ZEND_HASH_FOREACH_NUM_KEY_VAL(options, key, data) {
            switch (key) {
                case 0:
                    this->options.buildUnindexedObjects = (Z_TYPE_P(data) == IS_TRUE);
                    break;
                case 1:
                    this->options.buildUnindexedArrays = (Z_TYPE_P(data) == IS_TRUE);
                    break;
                default:
                    // @todo exception
                    break;
            }
        } ZEND_HASH_FOREACH_END();
    }

    void Vpack::from_binary(const char* binary, size_t size)
    {
        vp::Buffer<uint8_t>* buffer = new vp::Buffer<uint8_t>();
        buffer->append(binary, size);

        this->builder = vp::Builder(*buffer);
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
        this->builder = vp::Builder(&this->options);
        Vpack::php_array_to_vpack(array, &this->builder);
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
        return vp::HexDump(this->builder.slice()).toString();
    }

    std::string Vpack::to_binary()
    {
        return this->builder.buffer()->toString();
    }

    zval* Vpack::to_array(zval* return_value)
    {
        Vpack::vpack_to_php_value(this->builder.slice(), return_value);
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

            if (tmpSlice.isObject() || tmpSlice.isArray()) {
                object_init_ex(return_value, vpack_ce);
                auto intern = Vpack::fetch_object(Z_OBJ_P(return_value));
                new (intern) Vpack(vp::Builder(tmpSlice));
            } else {
                Vpack::vpack_to_php_value(tmpSlice, return_value);
            }
        //}
        //catch(const vp::Exception& e) {
            //@todo exception
        //}
    }

    void Vpack::access(zval* return_value, const char* accessor)
    {
        //try {
            auto tmpSlice = this->builder.slice().get(accessor);

            if (tmpSlice.isObject() || tmpSlice.isArray()) {
                object_init_ex(return_value, vpack_ce);
                auto intern = Vpack::fetch_object(Z_OBJ_P(return_value));
                new (intern) Vpack(vp::Builder(tmpSlice));
            } else {
                Vpack::vpack_to_php_value(tmpSlice, return_value);
            }
        //}
        //catch(const vp::Exception& e) {
            //@todo exception
        //}
    }

    //Assoc arrays get converted to objets, therefore if the accessor is an integer and the 
    //slice is an object we convert the integer to a string and check for the property.
    void Vpack::access(zval* return_value, int accessor)
    {
        auto topLevelSlice = this->builder.slice();
        
        if(topLevelSlice.isObject()) {
            std::string key_accessor = std::to_string(accessor);
            this->access(return_value, key_accessor.c_str());
        } else if(topLevelSlice.isArray()) {
            auto tmpSlice = topLevelSlice.at(accessor);

            if (tmpSlice.isObject() || tmpSlice.isArray()) {
                object_init_ex(return_value, vpack_ce);
                auto intern = Vpack::fetch_object(Z_OBJ_P(return_value));
                new (intern) Vpack(vp::Builder(tmpSlice));
            } else {
                Vpack::vpack_to_php_value(tmpSlice, return_value);
            }
        } else {
            //@todo exception
        }
    }

    bool Vpack::exists(const char* accessor) 
    {
        return this->builder.slice().hasKey(accessor);
    }

    //Assoc arrays get converted to objets, therefore if the accessor is an integer and the 
    //slice is an object we convert the integer to a string and check for the property.
    bool Vpack::exists(int accessor) 
    {
        auto slice = this->builder.slice();
        
        if(slice.isObject()) {
            std::string key_accessor = std::to_string(accessor);
            return this->builder.slice().hasKey(key_accessor);
        } else if(slice.isArray()) {
            try {
                this->builder.slice().at(accessor);
            } catch(const vp::Exception &ex) {
                return false;
            }
            return true;
        } else {
            //@todo exception
        }
    }

    int Vpack::count()
    {
        auto slice = this->builder.slice();

        return slice.length();
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

            case vp::ValueType::Bool:
                if(slice.getBool()) {
                    ZVAL_TRUE(value);
                } else {
                    ZVAL_FALSE(value);
                }
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

            //@todo I think we need to check if the key is numeric and convert it to a number
            //Note: this is the behavior of json_decode as well
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
        if(Vpack::php_array_is_numeric(array)) {
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
                    if(Vpack::php_array_is_numeric(Z_ARRVAL_P(data))) {
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
                    if(Vpack::php_array_is_numeric(Z_ARRVAL_P(data))) {
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

    bool Vpack::php_array_is_numeric(HashTable* array)
    {
        if (zend_hash_num_elements(array) < 1) {
            return true;
        }

        // This works most of the time but not always, see vpack-from-array-recognizes-numeric-array-correctly.phpt
        if (HT_IS_PACKED(array) && HT_IS_WITHOUT_HOLES(array)) {
            return true;
        }

        zend_string *key;
        zend_ulong index;
        zend_ulong idx = 0;
        ZEND_HASH_FOREACH_KEY(array, index, key) {
            if (key) {
                return false;
            } else {
                if (index != idx) {
                    return false;
                }
            }
            idx++;
        } ZEND_HASH_FOREACH_END();

        return true;
    }
}}
