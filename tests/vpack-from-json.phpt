--TEST--
It creates a Vpack instance from json

--SKIPIF--
<?php ?>

--FILE--
<?php
$vpack = \Velocypack\Vpack::fromJson('{"1":100,"2":1000000,"c":"d","test":true}');

var_dump($vpack->toArray());
?>

--EXPECTF--
array(4) {
  [1]=>
  int(100)
  [2]=>
  int(1000000)
  ["c"]=>
  string(1) "d"
  ["test"]=>
  bool(true)
}
