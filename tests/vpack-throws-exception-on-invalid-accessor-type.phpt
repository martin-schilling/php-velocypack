--TEST--
It checks if an exception is thrown when a variable with an invalid type was provided as argument.

--SKIPIF--
<?php ?>

--FILE--
<?php
$vpack = \Velocypack\Vpack::fromArray([
    "a" => "111",
    "b" => 222,
    "c" => true,
    "d" => false,
    "e" => 3.2,
    10,
    20,
    "arr" => [
        "a" => "b",
        111
    ],
    [23, 58, 10],
    [0 => 10, 1 => 20, 3 => 30],
    "null" => null,
    "obj" => new \stdClass()
]);

var_dump($vpack->access('b'));
var_dump($vpack->access(['arr', 'a']));

try {
    $vpack->access(1.45);
} catch(\Throwable $e) {
    var_dump(get_class($e));
    var_dump($e->getMessage());
}

try {
    isset($vpack[1.34]);
} catch(\Throwable $e) {
    var_dump(get_class($e));
    var_dump($e->getMessage());
}


--EXPECTF--
int(222)
string(1) "b"
string(35) "Velocypack\InvalidArgumentException"
string(30) "Invalid accessor type provided"
string(35) "Velocypack\InvalidArgumentException"
string(28) "Invalid offset type provided"