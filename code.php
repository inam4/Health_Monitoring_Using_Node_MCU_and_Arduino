<?php


$host = "35.192.9.19";
$username = "inam";
$password = "inam9688";
// Create connection
$conn = mysqli_connect($host, $username, $password,"iot_2");


// Check if connection established successfully
if (!$conn) 
{
    die("Connection failed: " . mysqli_connect_error());
}
echo $_POST['heart'];
echo $_POST['temp'];
echo $_POST['oxy'];
echo 'Connected successfully';

$heart = strval($_POST['heart']);
$temp = strval($_POST['temp']);
$oxy = strval($_POST['oxy']);

date_default_timezone_set("Asia/Karachi");
echo "The time is " . date("H:i:s");
$time_1 = date("H:i:s");
$time_2 = date("His");
$date_2 = date("Ymd");
$date_1 = date("Y-m-d");

echo $heart;
echo $time_1;
// Update your tablename here

$sql = "INSERT INTO atest3 (date_1,date_2,time_1,time_2,temp,beat,oxy) VALUES ('".$date_1."','".(int)$date_2."','".$time_1."','".(int)$time_2."','".(int)$temp."', '".(int)$heart."', '".(int)$oxy."')"; 

    

if(!empty($_POST['heart']) && !empty($_POST['oxy']) )
{
    $status=mysqli_query($conn,$sql);
    if(!$status ) {
        die('Could not enter data: ' . mysql_error());
     }
     
     echo "Entered data successfully\n";
}



mysqli_close($conn);



?>