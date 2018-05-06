# php-velocypack performance

## Extract a value from JSON
Measures the speed of accessing a single value from a given json using 
velocypack and json_decode (to array). 
```
$ docker-compose run --rm php php performance/extractingValueFromJson.php
  
Accessing value with vpack took 2.1347763538361 seconds
Accessing value with array took 3.685986995697 seconds
```

## Extract a value from binary JSON (vpack)
Measures the speed of accessing a single value from a given json that
was saved in binary form using velocypack and from the normal json using 
json_decode (to array).
```
$ docker-compose run --rm php php performance/extractingValueFromBinaryJson.php
  
Accessing value with vpack took 0.25113558769226 seconds
Accessing value with array took 3.6604378223419 seconds

```
