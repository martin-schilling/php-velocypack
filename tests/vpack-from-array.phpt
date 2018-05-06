--TEST--
It creates a Vpack instance from a PHP array

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

var_dump($vpack->toJson());
?>

--EXPECTF--
string(147) "{"0":10,"1":20,"2":[23,58,10],"3":{"0":10,"1":20,"3":30},"a":"111","arr":{"0":111,"a":"b"},"b":222,"c":true,"d":false,"e":3.2,"null":null,"obj":{}}"
