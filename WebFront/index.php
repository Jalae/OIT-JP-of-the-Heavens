<?php
include('./Google-Voice-PHP-API/GoogleVoice.php');
$gv = new GoogleVoice("kehnin", "Password");
$gv->sendSMS('5035070650', 'Did this work?');





?>
