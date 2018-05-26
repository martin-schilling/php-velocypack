--TEST--
It checks if keys exist using ArrayAccess

Note that assoc arrays get converted to (JSON) objects and therefore there are no integer keys. 
This means that $vpack[1] and $vpack["1"] are the same. Internally integer keys are being 
converted to a string when working with objects. 
Therefore accessing by an integer behaves differently for numeric and assoc arrays and we test both.

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

var_dump(isset($vpack["null"]));
var_dump(isset($vpack["test"]));
var_dump(isset($vpack[2]));
var_dump(isset($vpack[15]));

$vpackArray = \Velocypack\Vpack::fromArray([10, "test", true]);
var_dump(isset($vpackArray[2]));
var_dump(isset($vpackArray[3]));
?>

--EXPECTF--
bool(true)
bool(false)
bool(true)
bool(false)
bool(true)
bool(false)
