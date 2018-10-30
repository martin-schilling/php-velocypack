--TEST--
It counts the length of a vpack

--SKIPIF--
<?php ?>

--FILE--
<?php
$vpack = \Velocypack\Vpack::fromArray([
    "a" => "111",
    "b" => 222,
    "c" => true
]);

var_dump(count($vpack));
var_dump($vpack->count());

$vpack = \Velocypack\Vpack::fromJson('[1, 10, 100, 1000, 10000]');

var_dump(count($vpack));
var_dump($vpack->count());
?>

--EXPECTF--
int(3)
int(3)
int(5)
int(5)
