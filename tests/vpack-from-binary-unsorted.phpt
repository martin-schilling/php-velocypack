--TEST--
It creates a Vpack instance from binary velocypack data without sorting it

--SKIPIF--
<?php ?>

--FILE--
<?php
$vpack = \Velocypack\Vpack::fromBinary(file_get_contents(__DIR__ . '/_files/small-binary-vpack-unsorted.bin'));
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
