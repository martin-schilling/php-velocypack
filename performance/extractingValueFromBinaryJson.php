<?php

$json = file_get_contents(__DIR__ . '/_files/sample_001.json');
$vpackJson = file_get_contents(__DIR__ . '/_files/sample_001.bin');

$totalTimeVpack = 0.0;
$totalTimeArray = 0.0;

for($i = 0; $i < 10000; $i++) {
    if($i % 2 === 0) {
        $t1 = microtime(true);
        $vpack = \Velocypack\Vpack::fromBinary($vpackJson);
        $value = $vpack->access([5, "friends", 1, "name"]);
        $t2 = microtime(true);
        $totalTimeVpack += ($t2 - $t1);
    } else {
        $t1 = microtime(true);
        $array = json_decode($json, true);
        $value = $array[5]["friends"][1]["name"];
        $t2 = microtime(true);
        $totalTimeArray += ($t2 - $t1);
    }

    if($value !== 'Glass Pruitt') {
        throw new RuntimeException("Wrong data");
    }
}

echo "Accessing value with vpack took {$totalTimeVpack} seconds\n";
echo "Accessing value with array took {$totalTimeArray} seconds\n";
