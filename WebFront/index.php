<?php
include('./Google-Voice-PHP-API/GoogleVoice.php');
//$gv = new GoogleVoice("kehnin.dyer@oit.edu", "pinkballons");
$gv = new GoogleVoice("kehnin", "");

$sms = $gv->getNewSMS();

//$gv->sendSMS('5035070650', 'Did this work?');

foreach($sms as $s)
{
	preg_match('/\+1([0-9]{3})([0-9]{3})([0-9]{4})/', $s['phoneNumber'], $match);
	$number = '(' . $match[1] . ') ' . $match[2] . '-'. $match[3];
	echo 'Message from ' . $number . ': ' . $s['message'] . "\n";
}



?>
