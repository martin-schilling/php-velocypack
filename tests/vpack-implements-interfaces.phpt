--TEST--
It checks that Vpack implements all interfaces

--SKIPIF--
<?php ?>

--FILE--
<?php
$ref = new ReflectionClass(\Velocypack\Vpack::class);
var_dump(array_keys($ref->getInterfaces()));
?>

--EXPECTF--
array(3) {
  [0]=>
  string(25) "Velocypack\VpackInterface"
  [1]=>
  string(9) "Countable"
  [2]=>
  string(11) "ArrayAccess"
}
