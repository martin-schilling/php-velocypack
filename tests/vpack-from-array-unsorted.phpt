--TEST--
It creates a Vpack instance from a PHP array without sorting it

--SKIPIF--
<?php ?>

--FILE--
<?php
$vpack = \Velocypack\Vpack::fromArray(
    [
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
        ]
    ],
    [
        Velocypack\Option\BUILD_UNINDEXED_OBJECTS => true,
        Velocypack\Option\BUILD_UNINDEXED_ARRAYS => true
    ]
);

var_dump($vpack->toJson());
?>

--EXPECTF--
string(84) "{"a":"111","b":222,"c":true,"d":false,"e":3.2,"0":10,"1":20,"arr":{"a":"b","0":111}}"
