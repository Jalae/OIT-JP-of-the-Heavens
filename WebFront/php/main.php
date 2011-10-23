<?php

function displayMessages()
{
	$sms = $gv->getNewSMS();
	
	foreach($sms as $s)
	{
		preg_match('/\+1([0-9]{3})([0-9]{3})([0-9]{4})/', $s['phoneNumber'], $match);
		$number = '(' . $match[1] . ') ' . $match[2] . '-'. $match[3];
		echo 'Message from ' . $number . ': ' . $s['message'] . "\n";
	}
}


?>

