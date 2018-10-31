--TEST--
It checks that conversion from vpack to php values works properly

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

$subVpack1 = $vpack[2];
$subVpack2 = $vpack->access(3);

var_dump($vpack->access("e"));
var_dump($vpack["e"]);
var_dump($vpack->access(["e"]));

// This is the same, see vpack-array-access-get.phpt
var_dump($vpack[1]);
var_dump($vpack["1"]);

var_dump($vpack);
var_dump($subVpack1);
var_dump($subVpack2);

var_dump($vpack->toJson());
var_dump($subVpack1->toJson());
var_dump($subVpack2->toJson());

var_dump($subVpack2->toArray());
var_dump($subVpack2[3]);

?>

--EXPECTF--
float(3.2)
float(3.2)
float(3.2)
int(20)
int(20)
object(Velocypack\Vpack)#2 (0) {
}
object(Velocypack\Vpack)#1 (0) {
}
object(Velocypack\Vpack)#3 (0) {
}
string(147) "{"0":10,"1":20,"2":[23,58,10],"3":{"0":10,"1":20,"3":30},"a":"111","arr":{"0":111,"a":"b"},"b":222,"c":true,"d":false,"e":3.2,"null":null,"obj":{}}"
string(10) "[23,58,10]"
string(22) "{"0":10,"1":20,"3":30}"
array(3) {
  [0]=>
  int(10)
  [1]=>
  int(20)
  [3]=>
  int(30)
}
int(30)
