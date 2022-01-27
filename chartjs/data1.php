<?php
//setting header to json
header('Content-Type: application/json');

//database
define('DB_HOST', '35.192.9.19');
define('DB_USERNAME', 'inam');
define('DB_PASSWORD', 'inam9688');
define('DB_NAME', 'iot_2');
//get connection
$mysqli = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME);

if(!$mysqli){
  die("Connection failed: " . $mysqli->error);
}

//query to get data from the table
$query = sprintf("SELECT id, beat, oxy, temp,time_1 FROM atest3 WHERE date_1='2022-01-27'");

//execute query
$result = $mysqli->query($query);

//loop through the returned data
$data = array();
foreach ($result as $row) {
  $data[] = $row;
}

//free memory associated with result
$result->close();

//close connection
$mysqli->close();

//now print the data
print json_encode($data);