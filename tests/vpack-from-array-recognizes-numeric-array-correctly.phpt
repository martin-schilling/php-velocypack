--TEST--
Some internal changes implemented with PHP 7.3 lead to unexpected behavior regarding the handling of numeric arrays.
See https://github.com/martin-schilling/php-velocypack/issues/12

--SKIPIF--
<?php ?>

--FILE--
<?php
$vpack1 = \Velocypack\Vpack::fromArray(['test']);
$vpack2 = \Velocypack\Vpack::fromArray(array_merge([], ['test']));
$vpack3 = \Velocypack\Vpack::fromArray(array_merge(['test'], []));

var_dump($vpack1->toJson());
var_dump($vpack2->toJson());
var_dump($vpack3->toJson());

?>

--EXPECTF--
string(8) "["test"]"
string(8) "["test"]"
string(8) "["test"]"
