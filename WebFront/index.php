<?php
$gvnum = "";
$acctname = "kehnin.dyer@oit.edu";
$acctpass = "pinkballons";
$options = array(
		'return-info' => true,
		'method' =>'post'
		);
$contents = file_get_contents('http://voice.google.com', $options);

$galx = 0;

print_r($result);
?>
