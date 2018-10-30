# php-velocypack

[![Build Status](https://travis-ci.com/martin-schilling/php-velocypack.svg?branch=master)](https://travis-ci.com/martin-schilling/php-velocypack)

This is still under heavy development but feel free to try the extension and report any bugs you might encounter.

## Installation
### Initial Setup
```
$ git clone https://github.com/martin-schilling/php-velocypack.git
$ cd php-velocypack
$ ./clone_velocypack
$ docker-compose build extension-builder
```
### Compile Extension
```
$ docker-compose run --rm extension-builder
$ docker-compose build php 
```
### Run Tests
```
$ docker-compose run --rm php make test
```
### Run Script
```
$ docker-compose run --rm php php script_name.php
```

## Usage
### Creating a Vpack
```php
$vpack = \Velocypack\Vpack::fromJson('{"test":"abcde", "numbers": [1234, 5678]}');
$vpack = \Velocypack\Vpack::fromArray(["a" => [123], "b" => ["c" => true]]);
$vpack = \Velocypack\Vpack::fromBinary($binaryContent);
```
### Accessing the values of a Vpack
```php
$vpack = \Velocypack\Vpack::fromJson('{"test":"abcde", "numbers": [1234, 5678]}');

// access subvalue on first layer
$vpack->access("test"); // string(5) "abcde"

// access subvalue on deeper level
$vpack->access(["numbers", 1]); // int(5678)

$vpack->access("numbers"); // array(2) {[0]=>int(1234) [1]=>int(5678)}

```
### Serializing a Vpack
```php
$vpack = \Velocypack\Vpack::fromArray(["a" => [123], "b" => ["c" => true]]);

$vpack->toHex(); // 0x0b 0x13 0x02 0x41 0x61 0x02 0x04 0x20 0x7b 0x41 0x62 0x14 0x06 0x41 0x63 0x1a 0x01 0x03 0x09

$vpack->toJson(); // {"a":[123],"b":{"c":true}}

$vpack->toArray(); // ["a" => [123], "b" => ["c" => true]]

$vpack->toBinary();
```
