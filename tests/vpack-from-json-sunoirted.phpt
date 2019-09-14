--TEST--
It creates a Vpack instance from json without sorting it

--SKIPIF--
<?php ?>

--FILE--
<?php
$vpack = \Velocypack\Vpack::fromJson('{"2":100,"1":1000000,"x":"test","d":true}', [
    Velocypack\Option\BUILD_UNINDEXED_OBJECTS => true,
    Velocypack\Option\BUILD_UNINDEXED_ARRAYS => true
]);

var_dump($vpack->toArray());
?>

--EXPECTF--
array(4) {
  [2]=>
  int(100)
  [1]=>
  int(1000000)
  ["x"]=>
  string(4) "test"
  ["d"]=>
  bool(true)
}
