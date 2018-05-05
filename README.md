# php-velocypack

## Usage
```php
$vpack = \Velocypack\Vpack::fromJson('{"test":"abcde", "numbers": [1234, 5678]}');

// dump to hex
$hex = $vpack->toHex();

// access subvalue on first layer
$vpack->access("test"); // string(5) "abcde"

// access subvalue on deeper level
$vpack->access(["numbers", 1]); // int(5678)


$anotherVpack = \Velocypack\Vpack::fromArray(["a" => [123], "b" => ["c" => true]]);

// dump to json
$anotherVpack->toJson(); // string(26) "{"a":[123],"b":{"c":true}}"
```
