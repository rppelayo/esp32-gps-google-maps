<?php
    $inp = file_get_contents("php://input");
    if(!empty($inp)){
        $s = explode("&",$inp);
        $a = explode("=",$s[0]);
        $b = explode("=",$s[1]);
        // Replace this with another method that saves to DB
        $file = fopen("gps_data.txt", "w") or die("Unable to open file!");
        $txt = $a[1] . "," . $b[1];
        fwrite($file, $txt);
        fclose($file);
    }else{
        // Replace this with another method to reads from DB
        $file = fopen("gps_data.txt", "r") or die("Unable to open file!");
        echo fread($file,filesize("gps_data.txt"));
        fclose($file); 
    }
?>
