<?php

namespace Velocypack\Option {
    const BUILD_UNINDEXED_OBJECTS = 0;
    const BUILD_UNINDEXED_ARRAYS = 1;
}

namespace Velocypack {

    interface VpackInterface extends \Countable, \ArrayAccess {
        public static function fromBinary(string $binary): self;
        public static function fromJson(string $json, ?array $options = null): self;
        public static function fromArray(array $array, ?array $options = null): self;
        public function toJson(): string;
        public function toHex(): string;
        public function toBinary(): string;
        public function toArray(): array;
        public function access($accessor);
    }

    class Vpack implements VpackInterface{

        /**
         * @param string $binary
         * @return \Velocypack\Vpack
         */
        public static function fromBinary(string $binary): self
        {
        }

        /**
         * @param string $json
         * @param array|null $options
         * @return \Velocypack\Vpack
         */
        public static function fromJson(string $json, ?array $options = null): self
        {
        }

        /**
         * @param array $array
         * @param array|null $options
         * @return \Velocypack\Vpack
         */
        public static function fromArray(array $array, ?array $options = null): self
        {
        }

        /**
         * @return string
         */
        public function toJson(): string
        {
        }

        /**
         * @return string
         */
        public function toHex(): string
        {
        }

        /**
         * @return string
         */
        public function toBinary(): string
        {
        }

        /**
         * @return array
         */
        public function toArray(): array
        {
        }

        /**
         * @param string|int|array $accessor
         * @return mixed
         */
        public function access($accessor)
        {
        }

        /**
         * @param string|int $key
         * @return bool
         */
        public function offsetExists($key): bool
        {
        }

        /**
         * @param string|int $key
         * @return mixed
         */
        public function offsetGet($key)
        {
        }

        /**
         * @param string|int $offset
         * @param mixed $value
         */
        public function offsetSet($offset, $value)
        {
        }

        /**
         * @param string|int $offset
         */
        public function offsetUnset($offset)
        {
        }

        /**
         * @return int
         */
        public function count(): int
        {
        }
    }

    class Exception extends \Exception
    {
    }

    class InvalidArgumentException extends Exception
    {
    }
}
