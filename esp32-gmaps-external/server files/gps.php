<?php
    if(isset($_POST['lat']) && isset($_POST['lng'])){
        // Replace this with another method that saves to DB
        $file = fopen("gps_data.txt", "w") or die("Unable to open file!");
        $txt = $_POST['lat'] . "," . $_POST['lng'];
        fwrite($file, $txt);
        fclose($file);
    }else{
        // Replace this with another method to reads from DB
        $file = fopen("gps_data.txt", "r") or die("Unable to open file!");
        echo fread($file,filesize("gps_data.txt"));
        fclose($file); 
    }


?>