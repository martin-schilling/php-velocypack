--TEST--
It creates a Vpack instance from binary velocypack data

--SKIPIF--
<?php ?>

--FILE--
<?php
$vpack = \Velocypack\Vpack::fromBinary(file_get_contents(__DIR__ . '/_files/small-binary-vpack.bin'));
var_dump($vpack->toArray());
?>

--EXPECTF--
array(2) {
  ["numbers"]=>
  array(2) {
    [0]=>
    int(1234)
    [1]=>
    int(5678)
  }
  ["test"]=>
  string(5) "abcde"
}
