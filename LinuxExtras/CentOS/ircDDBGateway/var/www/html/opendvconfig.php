<?php
//
// Configuration program for G4KLX OpenDV
//
// (C) Hans-J. Barthen DL5DI
//
// 20130413 dl5di:  initial version
// 20131019 dl5di:  bugfix, repeater configfile does not exist
//
//

require_once('ircddblocal.php');

$configs = array();

if ($configfile = fopen($gatewayConfigPath,'r')) {
        while ($line = fgets($configfile)) {
                list($key,$value) = split("=",$line);
                $value = trim(str_replace('"','',$value));
            	$configs[$key] = $value;
		if(!isset($_POST['$key'])) $_POST[$key] = $value;
        }
}

$progname = basename($_SERVER['SCRIPT_FILENAME'],".php");
$rev="20131019";
$MYCALL=strtoupper($callsign);
$col[0] = "#f0f0f0";
$col[1] = "#f0f0a0";

$langarr=array("English (UK)","Deutsch","Dansk","Italiano","Francais","Espanol","Svnska","Polski","English (US)","Nederlands (NL)","Nederland (BE)");
$maxlang = 11;
if(!isset($_POST['language'])) $_POST['language'] = $configs['language'];
$langidx = $_POST['language'];

$helptext = array();
$helpfilename = "opendv-" . $langidx . ".hlp";
if ($helpfile = fopen($helpfilename,'r')) {
	while ($line = fgets($helpfile)) {
		list($key,$value) = split("=",$line);
		$value = trim(str_replace('"','',$value));
		$helptext[$key] = "<font size=-2>" . $value. "</font>";
	}
}
?>

<html xmlns="http://www.w3.org/1999/xhtml"xmlns:v="urn:schemas-microsoft-com:vml">
<head>
    <meta name="robots" content="index">
    <meta name="robots" content="follow">
    <meta name="language" content="English">
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
    <?php
	echo "<meta name=\"GENERATOR\" content=\"$progname $rev\">";
    ?>
    <meta name="Author" content="Hans-J. Barthen (DL5DI)">
    <meta name="Description" content="G4KLX OpenDV configuration tool">
    <meta name="KeyWords" content="Hamradio,ircDDBGateway,OpenDV,D-Star,G4KLX,DL5DI">
    <title>OpenDV Configuration<?php echo "$MYCALL" ?></title>
    <LINK REL="stylesheet" type="text/css" href="css/ircddb.css">
    </head>
<body>

    <form action="<?php $progname.php ?>" method="post">
    <br><font size=+1><b>Configuration of OpenDV Gateway <?php print $MYCALL ?></b><br></font><br>
    <table BORDER=1 BGCOLOR=white>
    <font size=-1>
    <tr <?php echo "bgcolor=$col[1]" ?> ><td align=left width=333>Software version=</td><td align=left width=333>&nbsp;</td><td align=left width=333> <?php print php_uname() ?> </td></tr>

    <tr <?php echo "bgcolor=$col[0]" ?> >
		<td align=left width=333><a class="tooltip" href="#">gatewayCallsign=<input type=text name="gatewayCallsign" maxlength=7 size=7 value="<?php if(isset($_POST['gatewayCallsign'])) echo $_POST['gatewayCallsign']; ?>"><span><?php echo $helptext[gatewayCallsign] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">gatewayAddress=<input type=text name="gatewayAddress" maxlength=15 size=15 value="<?php if(isset($_POST['gatewayAddress'])) echo $_POST['gatewayAddress']; ?>"><span><?php echo $helptext[gatewayAddress] ?></span></a></td>
		<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[1]" ?> >
		<td align=left width=333><a class="tooltip" href="#"><a class="tooltip" href="#">icomAddress=<input type=text name="icomAddress" maxlength=15 size=15 value="<?php if(isset($_POST['icomAddress'])) echo $_POST['icomAddress']; ?>"><span><?php echo $helptext[icomAddress] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#"><a class="tooltip" href="#">icomPort=<input type=text name="icomPort" maxlength=5 size=5 value="<?php if(isset($_POST['icomPort'])) echo $_POST['icomPort']; ?>"><span><?php echo $helptext[icomPort] ?></a></td>
		<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[0]" ?> >
		<td align=left width=333><a class="tooltip" href="#">hbAddress=<input type=text name="hbAddress" maxlength=15 size=15 value="<?php if(isset($_POST['hbAddress'])) echo $_POST['hbAddress']; ?>"><span><?php echo $helptext[hbAddress] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">hbPort=<input type=text name="hbPort" maxlength=5 size=5 value="<?php if(isset($_POST['hbPort'])) echo $_POST['hbPort']; ?>"><span><?php echo $helptext[hbPort] ?></span></a></td>
		<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[1]" ?> >
		<td align=left width=333><a class="tooltip" href="#">latitude=<input type=text name="latitude" maxlength=9 size=9 value="<?php if(isset($_POST['latitude'])) echo $_POST['latitude']; ?>"><span><?php echo $helptext[latitude] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">longitude=<input type=text name="longitude" maxlength=8 size=8 value="<?php if(isset($_POST['longitude'])) echo $_POST['longitude']; ?>"><span><?php echo $helptext[longitude] ?></span></a></td>
		<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[0]" ?> >
		<td align=left width=333><a class="tooltip" href="#">description1=<input type=text name="description1" maxlength=20 size=20 value="<?php if(isset($_POST['description1'])) echo $_POST['description1']; ?>"><span><?php echo $helptext[description1] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">description2=<input type=text name="description2" maxlength=20 size=20 value="<?php if(isset($_POST['description2'])) echo $_POST['description2']; ?>"><span><?php echo $helptext[description2] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">url=<input type=text name="url" maxlength=40 size=40 value="<?php if(isset($_POST['url'])) echo $_POST['url']; ?>"><span><?php echo $helptext[url] ?></span></a></td>
    </tr>

    <tr <?php echo "bgcolor=$col[1]" ?> >
		<td align=left width=333><a class="tooltip" href="#">ircddbEnabled=<input type=checkbox name="ircddbEnabled" value="<?php if(isset($_POST['ircddbEnabled'])) echo $_POST['ircddbEnabled']; ?>" <?php if(isset($_POST['ircddbEnabled']) && ($_POST['ircddbEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[ircddbEnabled] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">ircddbHostname=
		<select name="ircddbHostname">
			<option value=1>group1-irc.ircddb.net</option>
			<option value=2>group2-irc.ircddb.net</option>
		</select>
		<span><?php echo $helptext[ircddbHostname] ?></span></a></td>	
		<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[0]" ?> >
		<td align=left width=333><a class="tooltip" href="#">ircddbUsername=<input type=text name="ircddbUsername" maxlength=8 size=8 value="<?php if(isset($_POST['ircddbUsername'])) echo $_POST['ircddbUsername']; ?>"><span><?php echo $helptext[ircddbUsername] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">ircddbPassword=<input type=password name="ircddbPassword" maxlength=16 size=16 value="<?php if(isset($_POST['ircddbPassword'])) echo $_POST['ircddbPassword']; ?>"><span><?php echo $helptext[ircddbPassword] ?></span></a></td>
		<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[1]" ?> >
		<td align=left width=333><a class="tooltip" href="#">aprsEnabled=<input type=checkbox name="aprsEnabled" value="<?php if(isset($_POST['aprsEnabled'])) echo $_POST['aprsEnabled']; ?>" <?php if(isset($_POST['aprsEnabled']) && ($_POST['aprsEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[aprsEnabled] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">aprsHostname=<input type=text name="aprsHostname" maxlength=20 size=20 value="<?php if(isset($_POST['aprsHostname'])) echo $_POST['aprsHostname']; ?>"><span><?php echo $helptext[aprsHostname] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">aprsPort=<input type=text name="aprsPort" maxlength=5 size=5 value="<?php if(isset($_POST['aprsPort'])) echo $_POST['aprsPort']; ?>"><span><?php echo $helptext[aprsPort] ?></span></a></td>
    </tr>

    <tr <?php echo "bgcolor=$col[0]" ?> >
		<td align=left width=333><a class="tooltip" href="#">dextraEnabled=<input type=checkbox name="dextraEnabled" value="<?php if(isset($_POST['dextraEnabled'])) echo $_POST['dextraEnabled']; ?>" <?php if(isset($_POST['dextraEnabled']) && ($_POST['dextraEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[dextraEnabled] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">dextraMaxDongles=<input type=text name="dextraMaxDongles" maxlength=2 size=2 value="<?php if(isset($_POST['dextraMaxDongles'])) echo $_POST['dextraMaxDongles']; ?>"><span><?php echo $helptext[dextraMaxDongles] ?></span></a></td>
		<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[1]" ?> >
		<td align=left width=333><a class="tooltip" href="#">dplusEnabled=<input type=checkbox name="dplusEnabled" value="<?php if(isset($_POST['dplusEnabled'])) echo $_POST['dplusEnabled']; ?>" <?php if(isset($_POST['dplusEnabled']) && ($_POST['dplusEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[dplusEnabled] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">dplusMaxDongles=<input type=text name="dplusMaxDongles" maxlength=2 size=2 value="<?php if(isset($_POST['dplusMaxDongles'])) echo $_POST['dplusMaxDongles']; ?>"><span><?php echo $helptext[dplusMaxDongles] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">dplusLogin=<input type=text name="dplusLogin" maxlength=8 size=8 value="<?php if(isset($_POST['dplusLogin'])) echo $_POST['dplusLogin']; ?>"><span><?php echo $helptext[dplusLogin] ?></span></a></td>
    </tr>

    <tr <?php echo "bgcolor=$col[0]" ?> >
		<td align=left width=333><a class="tooltip" href="#">ccsEnabled=<input type=checkbox name="ccsEnabled" value="<?php if(isset($_POST['ccsEnabled'])) echo $_POST['ccsEnabled']; ?>" <?php if(isset($_POST['ccsEnabled']) && ($_POST['ccsEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[ccsEnabled] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">ccsHost=
		<select name="ccsHost">
<?php
		$ccshost = array();
		$ccshostfilename = "$sharedFilesPath/CCS_Hosts.txt";
		if(!isset($_POST['ccsHost'])) $_POST['ccsHost'] = $configs['ccsHost'];
		if ($ccshostfile = fopen($ccshostfilename,'r')) {
			echo "<option value=" . $_POST['ccsHost'] . ">" . $_POST['ccsHost'] . "</option>";
			while ($line = fgets($ccshostfile)) {
				list($ccshost,$junk) = split("[ \t]",$line);
				if($ccshost != $_POST['ccsHost']){
					echo "<option value=$ccshost>$ccshost</option>";
				}
			}
		}
?>
		</select>
		<span><?php echo $helptext[ccsHost] ?></span></a></td>	
		<td align=left width=333><a class="tooltip" href="#">dcsEnabled=<input type=checkbox name="dcsEnabled" value="<?php if(isset($_POST['dcsEnabled'])) echo $_POST['dcsEnabled']; ?>" <?php if(isset($_POST['dcsEnabled']) && ($_POST['dcsEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[dcsEnabled] ?></span></a></td>
    </tr>

    <tr <?php echo "bgcolor=$col[1]" ?> >
		<td align=left width=333><a class="tooltip" href="#">remoteEnabled=<input type=checkbox name="remoteEnabled" value="<?php if(isset($_POST['remoteEnabled'])) echo $_POST['remoteEnabled']; ?>" <?php if(isset($_POST['remoteEnabled']) && ($_POST['remoteEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[remoteEnabled] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">remotePort=<input type=text name="remotePort" maxlength=5 size=5 value="<?php if(isset($_POST['remotePort'])) echo $_POST['remotePort']; ?>"><span><?php echo $helptext[remotePort] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">remotePassword=<input type=password name="remotePassword" maxlength=8 size=8 value="<?php if(isset($_POST['remotePassword'])) echo $_POST['remotePassword']; ?>"><span><?php echo $helptext[remotePassword] ?></span></a></td>
    </tr>

    <tr <?php echo "bgcolor=$col[0]" ?> >
		<td align=left width=333><a class="tooltip" href="#">language=
		<select name="language">
<?php
			echo "<option value=$langidx>$langarr[$langidx]</option>";
			for($i=0;$i<=$maxlang;$i++){
				if($langidx != $i){
					echo "<option value=$i>$langarr[$i]</option>";
				}
			}
?>
		</select>
		<span><?php echo $helptext[language] ?></span></a></td>	
			<td align=left width=333>&nbsp;</td>
			<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[1]" ?> >
		<td align=left width=333><a class="tooltip" href="#">echoEnabled=<input type=checkbox name="echoEnabled" value="<?php if(isset($_POST['echoEnabled'])) echo $_POST['echoEnabled']; ?>" <?php if(isset($_POST['echoEnabled']) && ($_POST['echoEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[echoEnabled] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">infoEnabled=<input type=checkbox name="infoEnabled" value="<?php if(isset($_POST['infoEnabled'])) echo $_POST['infoEnabled']; ?>" <?php if(isset($_POST['infoEnabled']) && ($_POST['infoEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[infoEnabled] ?></span></a></td>
		<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[0]" ?> >
		<td align=left width=333><a class="tooltip" href="#">logEnabled=<input type=checkbox name="logEnabled" value="<?php if(isset($_POST['logEnabled'])) echo $_POST['logEnabled']; ?>" <?php if(isset($_POST['logEnabled']) && ($_POST['logEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[logEnabled] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">nolog=<input type=checkbox name="nolog" value="<?php if(isset($_POST['nolog'])) echo $_POST['nolog']; ?>" <?php if(isset($_POST['nolog']) && ($_POST['nolog'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[nolog] ?></span></a></td>
		<td align=left width=333><a class="tooltip" href="#">logPath=<input type=text name="logPath" maxlength=40 size=40 value="<?php if(isset($_POST['logPath'])) echo $_POST['description1']; ?>"><span><?php echo $helptext[logPath] ?></span></a></td>
    </tr>

    <tr <?php echo "bgcolor=$col[1]" ?> >
		<td align=left width=333><a class="tooltip" href="#">dratsEnabled=<input type=checkbox name="dratsEnabled" value="<?php if(isset($_POST['dratsEnabled'])) echo $_POST['dratsEnabled']; ?>" <?php if(isset($_POST['dratsEnabled']) && ($_POST['dratsEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[dratsEnabled] ?></span></a></td>
		<td align=left width=333>&nbsp;</td>
		<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[0]" ?> >
		<td align=left width=333><a class="tooltip" href="#">dtmfEnabled=<input type=checkbox name="dtmfEnabled" value="<?php if(isset($_POST['dtmfEnabled'])) echo $_POST['dtmfEnabled']; ?>" <?php if(isset($_POST['dtmfEnabled']) && ($_POST['dtmfEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[dtmfEnabled] ?></span></a></td>
		<td align=left width=333>&nbsp;</td>
		<td align=left width=333>&nbsp;</td>
    </tr>

    <tr <?php echo "bgcolor=$col[1]" ?> >
		<td align=left width=333><a class="tooltip" href="#">autostartEnabled=<input type=checkbox name="autostartEnabled" value="<?php if(isset($_POST['autostartEnabled'])) echo $_POST['autostartEnabled']; ?>" <?php if(isset($_POST['autostartEnabled']) && ($_POST['autostartEnabled'] == 1)){ echo 'checked';} ?>><span><?php echo $helptext[autostartEnabled] ?></span></a></td>
		<td align=left width=333>&nbsp;</td>
		<td align=left width=333>&nbsp;</td>
    </tr>
    </table>
    </form>

<?php
    $rptrparms=array("repeaterCall","repeaterBand","repeaterType","repeaterAddress","repeaterPort","reflector","atStartup","reconnect","frequency","offset","rangeKms","latitude","longitude","agl","description1_","description2_","url");
    $rptrparmtype=array("text","text","text","text","text","text","text","checkbox","text","text","text","text","text","text","text","text","text");
    $rptrparmlen=array("8","2","1","15","5","8","0","1","8","7","4","9","9","3","20","20","40");
// element 1-17
    $maxi = 17;

    $bandarr=array("A","B","C","D","E","AD","BD","CD","DD");
    $maxband=9;


?>    
    <table BORDER=1 BGCOLOR=white>
    <tr bgcolor=black>
		<th width=250><a class="tooltip" href="#"><font color=white><center><b>Repeater 1</b></center></font><span><b>Repeater 1</b></span></a></th>
		<th width=250><a class="tooltip" href="#"><font color=white><center><b>Repeater 2</b></center></font><span><b>Repeater 2</b></span></a></th>
		<th width=250><a class="tooltip" href="#"><font color=white><center><b>Repeater 3</b></center></font><span><b>Repeater 3</b></span></a></th>
		<th width=250><a class="tooltip" href="#"><font color=white><center><b>Repeater 4</b></center></font><span><b>Repeater 4</b></span></a></th>
    </tr>

<?php
    print"<tr bgcolor=$col[1]><td align=left width=250>SW-version=</td><td align=left width=250>SW-version=</td><td align=left width=250>SW-version=</td><td align=left width=250>SW-version=</td>";
    $k=0;
    for($i=0;$i<$maxi;$i++){
		print"<tr bgcolor=$col[$k]>";
		$k++;
		if ($k > 1) $k=0;
		for($j=1;$j<5;$j++){
			$act = "repeaterBand" . $j;
			$var = $rptrparms[$i] . $j;
			$type = $rptrparmtype[$i] . $j;
			if(strlen($configs[$act]) > 0){
				$fontcolor = "black";
			} else {
				$fontcolor = "lightgrey";
			}
			if(isset($configs[$var])){
				print "<td align=left width=250> <font color=$fontcolor>$var=$configs[$var]</td>";
			} else {
				print "<td align=left width=250> <font color=$fontcolor>$var=&nbsp;</td>";
			}
		}
		print"</tr>";
    }
?>
    </table>
    <P>

    <table BORDER=1 BGCOLOR=white>
    <tr bgcolor=black>
		<th width=250><a class="tooltip" href="#"><font color=white><center><b>StarNetServer 1</b></center></font><span><b>StarNetServer 1</b></span></a></th>
		<th width=250><a class="tooltip" href="#"><font color=white><center><b>StarNetServer 2</b></center></font><span><b>StarNetServer 2</b></span></a></th>
		<th width=250><a class="tooltip" href="#"><font color=white><center><b>StarNetServer 3</b></center></font><span><b>StarNetServer 3</b></span></a></th>
		<th width=250><a class="tooltip" href="#"><font color=white><center><b>StarNetServer 4</b></center></font><span><b>StarNetServer 4</b></span></a></th>
		<th width=250><a class="tooltip" href="#"><font color=white><center><b>StarNetServer 5</b></center></font><span><b>StarNetServer 5</b></span></a></th>
    </tr>

<?php

	$stnparms=array("starNetBand","starNetCallsign","starNetLogoff","starNetInfo","starNetPermanent","starNetUserTimeout","starNetGroupTimeout","starNetCallsignSwitch","starNetTXMsgSwitch","starNetReflector");
// element 1-10 
    $maxi=10;
    for($i=0;$i<$maxi;$i++){
		print"<tr bgcolor=$col[$k]>";
		$k++;
		if ($k > 1) $k=0;
		for($j=1;$j<6;$j++){
			$var = $stnparms[$i] . $j;
			if(isset($configs[$var])){
				print "<td align=left width=250>$var=$configs[$var]</td>";
			} else {
				print "<td align=left width=250>$var=&nbsp;</td>";
			}
		}
		print"</tr>";
	}
    print"</table>";

    for ($i=1;$i<5;$i++){
		$rptrConfigPath = "$sysConfigPath/repeater_$i";
		if (file_exists($rptrConfigPath)) {
			if ($rconfigfile = fopen($rptrConfigPath,'r')) {
				while ($line = fgets($rconfigfile)) {
					list($key,$value) = split("=",$line);
					$value = trim(str_replace('"','',$value));
					$rptrparms[$key] = $value;
				}
			}
		}
    }
?>

    <P>
    <font size=+1><b>Configuration of OpenDV Repeaters connected to Gateway <?php print $MYCALL ?></b><br></font><br>
    <table BORDER=0><tr><td valign=top>
    <font size=-1>
<?php
    $k = 0;
    for($i=1;$i<5;$i++){
	
		$param = "repeaterBand" . $i;
		if(strlen($configs[$param]) == 0){
			continue;
		}
		$sysConfigFilename = $sysConfigPath . "/" . "repeater_" . $i;
		if ($sysConfigFile = fopen($sysConfigFilename,'r')) {
			echo"<table BORDER=1>";
			while ($line = fgets($sysConfigFile)){
				# REPEATER_PATH=/usr/local/bin/dvrptrrepeaterd
				list($key,$value) = split("=",$line);
				if($key == REPEATER_PATH){
					$temp = explode("/",$value);
					end($temp);
					$rptrdaemon = rtrim(current($temp),"d\r\n");;
					$rptrConfigFilename = $configPath . "/" . $rptrdaemon . "_" . $i;
					if ($rptrConfigFile = fopen($rptrConfigFilename,'r')) {
						echo "<tr bgcolor=black> <th width=250><a class=\"tooltip\" href=\"#\"><font color=white><center><b>Repeater $i</b></center></font><span><b>Repeater $i</b></span></a></th></tr>";
						$k=0;
						while ($line = fgets($rptrConfigFile)){
							list($key,$value) = split("=",$line);
							$rptrconfigs[$key] = $value;
							if(strlen($key) < 3){
								$k++;
								continue;
							}
							if (isset($rptrconfigs[$key])){
								$color=$col[$k];
								if($key == "gatewayAddress"){
									if((trim($rptrconfigs[$key]) == $configs["hbAddress"]) || (trim($rptrconfigs[$key]) == $configs["icomAddress"])){
										$color="green";
									} else {
										$color="red";
									}
								} elseif($key == "gatewayPort"){
									if((trim($rptrconfigs[$key]) == $configs["hbPort"]) || (trim($rptrconfigs[$key]) == $configs["icomPort"])){
										$color="green";
									} else {
										$color="red";
									}
								} elseif($key == "localAddress"){
									$parm = "repeaterAddress" . $i;
									if(trim($rptrconfigs[$key]) == $configs[$parm]){
										$color="green";
									} else {
										$color="red";
									}
								} elseif($key == "localPort"){
									$parm = "repeaterPort" . $i;
									if(trim($rptrconfigs[$key]) == $configs[$parm]){
										$color="green";
									} else {
										$color="red";
									}
								}
								echo "<tr bgcolor=$color><td align=left width=250>$key=$rptrconfigs[$key]</td></tr>";				
							} else {
								echo "<tr bgcolor=$col[$k]><td align=left width=250>$key=&nbsp;</td></tr>";
							}
							$k++;
							if ($k > 1) $k=0;
						}
					}
				}
			}
			echo"</table></td><td valign=top>";
		}
    }
?>
    </td></tr></table>

    </table>
    <br><?php echo "$progname $rev"; ?>

<br>
&copy;2013 Hans-J. Barthen DL5DI
</body>
