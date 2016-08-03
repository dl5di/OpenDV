<?php

	require_once('ircddblocal.php');

	$configs = array();

	if ($configfile = fopen($gatewayConfigPath,'r')) {
		while ($line = fgets($configfile)) {
			list($key,$value) = split("=",$line);
			$value = trim(str_replace('"','',$value));
			if ($key != 'ircddbPassword' && strlen($value) > 0)
				$configs[$key] = $value;
		}
	}
	$progname = basename($_SERVER['SCRIPT_FILENAME'],".php");
	$rev="20131011";
	$MYCALL=strtoupper($callsign);
	$col[0] = "#f0f0f0";
	$col[1] = "#f0f0a0";
?>
	<!DOCTYPE html>

	<html xmlns="http://www.w3.org/1999/xhtml" xmlns:v="urn:schemas-microsoft-com:vml">
	<head>
		<meta name="robots" content="index">
		<meta name="robots" content="follow">
		<meta name="language" content="English">
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<?php
		echo "<meta name=\"GENERATOR\" content=\"$progname $rev\">\n";
		?>
		<meta name="Author" content="Hans-J. Barthen (DL5DI)">
		<meta name="Description" content="ircDDBGateway Dashboard">
		<meta name="KeyWords" content="Hamradio,ircDDBGateway,D-Star,ircDDB,DL5DI">
		<title>ircDDB LastHeard <?php echo "$MYCALL" ?></title>
		<LINK REL="stylesheet" type="text/css" href="css/ircddb.css">
	</head>
	<body>
<?php
	// calculate size of included repeater table
	$nummod = 1;
	for($i = 1;$i < 5; $i++){
		$param="repeaterBand" . $i;
		if(isset($configs[$param])) { $nummod++; }
	}	
	$tabh = ($nummod - 1) * 36 + 230;

	print "\t<iframe src=\"http://status.ircddb.net/lhhead.php?call=$MYCALL\" frameborder=\"0\" marginwidth=\"0\" marginheight=\"0\" width=\"100%\" height=\"$tabh\" scrolling=\"no\"></iframe>\n";
?>
	<br><font size=+1><b>Configuration</b></font><br>
	<font size=-1>
	<table BORDER=0 BGCOLOR=white>
		<tr bgcolor=black>
			<th width=130><a class="tooltip" href="#"><font color=white><center><b>ircDDB-Net</b></center></font><span><b>ircDDB-Net</b></span></a></th>
			<th width=90><a class="tooltip" href="#"><font color=white><center><b>APRS-Host</b></center></font><span><b>APRS-Host</b></span></a></th>
			<th width=40><a class="tooltip" href="#"><font color=white><center><b>CCS</b></center></font><span><b>Actual status</b>- green: enabled<br>- red: disabled</span></a></th>
			<th width=40><a class="tooltip" href="#"><font color=white><center><b>DCS</b></center></font><span><b>Actual status</b>- green: enabled<br>- red: disabled</span></a></th>
			<th width=40><a class="tooltip" href="#"><font color=white><center><b>DExtra</b></center></font><span><b>Actual status</b>- green: enabled<br>- red: disabled</span></a></th>
			<th width=40><a class="tooltip" href="#"><font color=white><center><b>DPlus</b></center></font><span><b>Actual status</b>- green: enabled<br>- red: disabled</span></a></th>
			<th width=40><a class="tooltip" href="#"><font color=white><center><b>D-Rats</b></center></font><span><b>Actual status</b>- green: enabled<br>- red: disabled</span></a></th>
			<th width=40><a class="tooltip" href="#"><font color=white><center><b>Info</b></center></font><span><b>Actual status</b>- green: enabled<br>- red: disabled</span></a></th>
			<th width=40><a class="tooltip" href="#"><font color=white><center><b>ircDDB</b></center></font><span><b>Actual status</b>- green: enabled<br>- red: disabled</span></a></th>
			<th width=40><a class="tooltip" href="#"><font color=white><center><b>Echo</b></center></font><span><b>Actual status</b>- green: enabled<br>- red: disabled</span></a></th>
			<th width=40><a class="tooltip" href="#"><font color=white><center><b>Log</b></center></font><span><b>Actual status</b>- green: enabled<br>- red: disabled</span></a></th>
		</tr>
		<tr bgcolor="#f0f0a0">
<?php
	print "\t<td>$configs[ircddbHostname]</td>\n";
			if($configs['aprsEnabled'] == 1){ print "\t\t\t<td align=center width=40>$configs[aprsHostname]</td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n";}
			if($configs['ccsEnabled'] == 1){print "\t\t\t<td align=center width=40><img src=\"images/20green.png\"></td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n"; }
			if($configs['dcsEnabled'] == 1){print "\t\t\t<td align=center width=40><img src=\"images/20green.png\"></td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n"; }
			if($configs['dextraEnabled'] == 1){print "\t\t\t<td align=center width=40><img src=\"images/20green.png\"></td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n"; }
			if($configs['dplusEnabled'] == 1){print "\t\t\t<td align=center width=40><img src=\"images/20green.png\"></td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n"; }
			if($configs['dratsEnabled'] == 1){print "\t\t\t<td align=center width=40><img src=\"images/20green.png\"></td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n"; }
			if($configs['infoEnabled'] == 1){print "\t\t\t<td align=center width=40><img src=\"images/20green.png\"></td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n"; }
			if($configs['ircddbEnabled'] == 1){print "\t\t\t<td align=center width=40><img src=\"images/20green.png\"></td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n"; }
			if($configs['echoEnabled'] == 1){print "\t\t\t<td align=center width=40><img src=\"images/20green.png\"></td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n"; }
			if($configs['logEnabled'] == 1){print "\t\t\t<td align=center width=40><img src=\"images/20green.png\"></td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n"; }
?>
		</tr>
	</table>
	</font>
	<p>
	<font size=+1><b>Reflector Links</b></font>
	</p>
	<font size=-1>
	<table BORDER=0 BGCOLOR=white>
		<tr bgcolor=black>
			<th width=70><a class=tooltip href="#"><font color=white><center><b>Repeater</b></center></font><span><b>Callsign of connected repeater</b></span></a></th>
			<th width=80><a class=tooltip href="#"><font color=white><center><b>Default</b></center></font><span><b>Default Link Destination</b></span></a></th>
			<th width=40><a class=tooltip href="#"><font color=white><center><b>Auto</b></center></font><span><b>AutoLink</b>- green: enabled<br>- red: disabled</span></a></th>
			<th width=50><a class=tooltip href="#"><font color=white><center><b>Timer</b></center></font><span><b>Reset/Restart Timer</b></span></a></th>
			<th width=40><a class=tooltip href="#"><font color=white><center><b>Lnk</b></center></font><span><b>Link-Status</b>- green: enabled<br>- red: disabled</span></a></th>
			<th width=80><a class=tooltip href="#"><font color=white><center><b>Linked to</b></center></font><span><b>linked destination</b></span></a></th>
			<th width=60><a class=tooltip href="#"><font color=white><center><b>Protocol</b></center></font><span><b>Protocol</b></span></a></th>
			<th width=80><a class=tooltip href="#"><font color=white><center><b>Direction</b></center></font><span><b>Direction</b>incoming or outgoing</span></a></th>
			<th width=130><a class=tooltip href="#"><font color=white><center><b>Last Change (UTC)</b></center></font><span><b>Timestamp of last change</b>UTC</span></a></th>
		</tr>

<?php
		$tot = array(0=>"Never",1=>"Fixed",2=>"5min",3=>"10min",4=>"15min",5=>"20min",6=>"25min",7=>"30min",8=>"60min",9=>"90min",10=>"120min",11=>"180min",12=>"&nbsp;");
		$ci = 0;
		$tr = 0;

		for($i = 1;$i < 5; $i++){
		$param="repeaterBand" . $i;
		if((isset($configs[$param])) && strlen($configs[$param]) == 1) {
			$ci++;
			if($ci > 1) { $ci = 0; }
			print "\t\t<tr bgcolor=\"$col[$ci]\">\n";
			$tr = 1;
			$module = $configs[$param];
			$rcall = sprintf("%-7.7s%-1.1s",$MYCALL,$module);
			$param="repeaterCall" . $i;
			if(isset($configs[$param])) { $rptrcall=sprintf("%-7.7s%-1.1s",$configs[$param],$module); } else { $rptrcall = $rcall;}
			print "\t\t\t<td>$rptrcall</td>\n";
			$param="reflector" . $i;
			if(isset($configs[$param])) { print "\t\t\t<td>$configs[$param]</td>\n"; } else { print "\t\t\t<td>&nbsp;</td>\n";}
			$param="atStartup" . $i;
			if($configs[$param] == 1){print "\t\t\t<td align=center width=40><img src=\"images/20green.png\"></td>\n"; } else { print "\t\t\t<td align=center width=40><img src=\"images/20red.png\"></td>\n"; }
			$param="reconnect" . $i;
			if(isset($configs[$param])) { $t = $configs[$param]; } else { $t = 0; }
			if($t > 12){ $t = 12; }
			print "\t\t\t<td>$tot[$t]</td>\n";
			$j=0;
			if ($linkLog = fopen($linkLogPath,'r')) {
				while ($linkLine = fgets($linkLog)) {
					$statimg = "<img src=\"images/20red.png\">";
					$linkDate = "&nbsp;";
					$protocol = "&nbsp;";
					$linkType = "&nbsp;";
					$linkRptr = "&nbsp;";
					$linkRefl = "&nbsp;";
					// Reflector-Link, sample:
					// 2011-09-22 02:15:06: DExtra link - Type: Repeater Rptr: DB0LJ  B Refl: XRF023 A Dir: Outgoing
					// 2012-10-12 17:15:45: DCS link - Type: Repeater Rptr: DB0LJ  B Refl: DCS001 L Dir: Outgoing
					// 2012-10-12 17:56:10: DCS link - Type: Repeater Rptr: DB0RPL B Refl: DCS015 B Dir: Outgoing
					if(preg_match_all('/^(.{19}).*(D[A-Za-z]*).*Type: ([A-Za-z]*).*Rptr: (.{8}).*Refl: (.{8}).*Dir: Outgoing$/',$linkLine,$linx) > 0){
						$statimg = "<img src=\"images/20green.png\">";
						$linkDate = $linx[1][0];
						$protocol = $linx[2][0];
						$linkType = $linx[3][0];
						$linkRptr = $linx[4][0];
						$linkRefl = $linx[5][0];
						if($linkRptr == $rptrcall){
							print "\t\t\t<td align=center width=40>$statimg</td>\n";
							print "\t\t\t<td>$linkRefl</td>\n";
							print "\t\t\t<td>$protocol</td>\n";
							print "\t\t\t<td>Outgoing</td>\n";
							print "\t\t\t<td>$linkDate</td>\n";
							print "\t\t</tr>\n";
							$tr = 0;
						}
					}
				}
				fclose($linkLog);
			}

			// 2012-05-08 21:16:31: DExtra link - Type: Repeater Rptr: DB0LJ  A Refl: DB0MYK B Dir: Incoming

			if ($linkLog = fopen($linkLogPath,'r')) {
				while ($linkLine = fgets($linkLog)) {
					$statimg = "<img src=\"images/20red.png\">";
					$linkDate = "&nbsp;";
					$protocol = "&nbsp;";
					$linkType = "&nbsp;";
					$linkRptr = "&nbsp;";
					$linkRefl = "&nbsp;";
					if(preg_match_all('/^(.{19}).*(D[A-Za-z]*).*Type: ([A-Za-z]*).*Rptr: (.{8}).*Refl: (.{8}).*Dir: Incoming$/',$linkLine,$linx) > 0){
						$statimg = "<img src=\"images/20green.png\">";
						$linkDate = $linx[1][0];
						$protocol = $linx[2][0];
						$linkType = $linx[3][0];
						$linkRptr = $linx[4][0];
						$linkRefl = $linx[5][0];
						if($linkRptr == $rptrcall){
							$ci++;
							if($ci > 1) { $ci = 0; }
							print "<tr bgcolor=\"$col[$ci]\">";
							print "<td align=left>$rptrcall</td>";
							print "<td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td>";
							print "<td align=center width=40>$statimg</td>";
							print "<td>$linkRefl</td>";
							print "<td>$protocol</td>";
							print "<td>Incoming</td>";
							print "<td>$linkDate</td>";
							print "</tr>";
							$tr = 0;
						}
					}
				}
				fclose($linkLog);
			}
			if ($tr == 1){
				print"<td align=center width=40>$statimg</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>";
			}
		}
    }
?>
	</table>
	</font>
	<p>
	<font size=+1><b>CCS Connects</b></font>
	</p>
	<font size=-1>
	<table BORDER=0 BGCOLOR=white>
		<tr bgcolor=black>
			<th width=70><a class=tooltip href="#"><font color=white><center><b>Repeater</b></center></font><span><b>Callsign of connected repeater</b></span></a></th>
			<th width=80><a class=tooltip href="#"><font color=white><center><b>Linked to</b></center></font><span><b>Actual link status</b></span></a></th>
			<th width=60><a class=tooltip href="#"><font color=white><center><b>Protocol</b></center></font><span><b>Protocol</b></span></a></th>
			<th width=80><a class=tooltip href="#"><font color=white><center><b>Direction</b></center></font><span><b>Direction</b>incoming or outgoing</span></a></th>
			<th width=130><a class=tooltip href="#"><font color=white><center><b>Last Change (UTC)</b></center></font><span><b>Timestamp of last change</b>UTC</span></a></th>
		</tr>

<?php
		$tot = array(0=>"Never",1=>"Fixed",2=>"5min",3=>"10min",4=>"15",5=>"20",6=>"25min",7=>"30min",8=>"60min",9=>"90min",10=>"120min",11=>"180min");
		$ci = 0;
		if ($linkLog = fopen($linkLogPath,'r')) {
			$i=0;
			while ($linkLine = fgets($linkLog)) {
			// 2013-02-27 19:49:27: CCS link - Rptr: DB0LJ  B Remote: DL5DI    Dir: Incoming
				if(preg_match_all('/^(.{19}).*(C[A-Za-z]*).*Rptr: (.{8}).*Remote: (.{8}).*Dir: (.{8})$/',$linkLine,$linx) > 0){
					$linkDate = $linx[1][0];
					$linkType = $linx[2][0];
					$linkRptr = $linx[3][0];
					$linkRem = $linx[4][0];
					$linkDir = $linx[5][0];
				$ci++;
				if($ci > 1) { $ci = 0; }
				print "\t\t<tr bgcolor=\"$col[$ci]\">\n";
				print "\t\t\t<td align=left>$linkRptr</td>\n";
				print "\t\t\t<td>$linkRem</td>\n";
				print "\t\t\t<td>CCS</td>\n";
				print "\t\t\t<td>$linkDir</td>\n";
				print "\t\t\t<td>$linkDate</td>\n";
				print "\t\t</tr>\n";
				}
			}
			fclose($linkLog);
		}
		$ci++;
		if($ci > 1) { $ci = 0; }
		print "\t\t<tr bgcolor=\"$col[$ci]\">\n";
?>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
			<td>&nbsp;</td>
		</tr>    
	</table>
	</font>
	<p>
	<font size=+1><b>Dongle Connects</b></font>
	</p>
	<font size=-1>
	<table BORDER=0 BGCOLOR=white>
		<tr bgcolor=black>
			<th width=70><a class=tooltip href="#"><font color=white><center><b>Repeater</b></center></font><span><b>Callsign of local repeater</b></span></a></th>
			<th width=80><a class=tooltip href="#"><font color=white><center><b>Linked to</b></center></font><span><b>Callsign of Destination</b></span></a></th>
			<th width=60><a class=tooltip href="#"><font color=white><center><b>Protocol</b></center></font><span><b>Protocol</b></span></a></th>
			<th width=80><a class=tooltip href="#"><font color=white><center><b>Direction</b></center></font><span><b>Direction</b>incoming or outgoing</span></a></th>
			<th width=130><a class=tooltip href="#"><font color=white><center><b>Last Change (UTC)</b></center></font><span><b>Timestamp of last change</b>UTC</span></a></th>
		</tr>

<?php
		$tot = array(0=>"Never",1=>"Fixed",2=>"5min",3=>"10min",4=>"15",5=>"20",6=>"25min",7=>"30min",8=>"60min",9=>"90min",10=>"120min",11=>"180min");
		$ci = 0;
		if ($linkLog = fopen($linkLogPath,'r')) {
			$i=0;
			while ($linkLine = fgets($linkLog)) {
				// Dongle-Link, sample: 2011-09-24 07:26:59: DPlus link - Type: Dongle User: DC1PIA   Dir: Incoming
				//			2012-03-14 21:32:18: DPlus link - Type: Dongle User: DC1PIA Dir: Incoming
				if(preg_match_all('/^(.{19}).*(D[A-Za-z]*).*Type: ([A-Za-z]*).*User: (.{6,8}).*Dir: (.*)$/',$linkLine,$linx) > 0){
					$ci++;
					if($ci > 1) { $ci = 0; }
					print "\t\t<tr bgcolor=\"$col[$ci]\">\n";
						$linkDate = $linx[1][0];
						$protocol = $linx[2][0];
						$linkType = $linx[3][0];
						$linkRptr = $linx[4][0];
						$linkRefl = "";
						$linkDir = $linx[5][0];
						print "\t\t\t<td>$linkRptr</td>\n";
						print "\t\t\t<td>$linkRefl</td>\n";
						print "\t\t\t<td>$protocol</td>\n";
						print "\t\t\t<td>$linkDir</td>\n";
						print "\t\t\t<td>$linkDate</td>\n";
					print "\t\t</tr>\n";
				}
			}
			fclose($linkLog);
		}
		$ci++;
		if($ci > 1) { $ci = 0; }
		print "\t\t<tr bgcolor=\"$col[$ci]\">\n";
			print "\t\t\t<td>&nbsp;</td>\n";
			print "\t\t\t<td>&nbsp;</td>\n";
			print "\t\t\t<td>&nbsp;</td>\n";
			print "\t\t\t<td>&nbsp;</td>\n";
			print "\t\t\t<td>&nbsp;</td>\n";
		print "\t\t</tr>\n";    
		print "\t</table>\n";
		print "\t</font>\n";

    include_once("dhcpleases.php");

    $stn_is_set = 0;
    for($i = 1;$i < 6; $i++){
		$param="starNetCallsign" . $i;
		if(isset($configs[$param])) {
			$stn_is_set = 1;
			break;
		}
    }
    if($stn_is_set > 0){
		include_once("dashboard_stn.php");
    }

		print "\t<iframe src=\"http://status.ircddb.net/lhmain.php?call=$MYCALL\" frameborder=\"0\" marginwidth=\"0\" marginheight=\"0\" width=\"100%\" height=\"750\" scrolling=\"no\"></iframe>\n";
		print "\t<br>\n";
		print "\t<font size=1>Rev:$rev &copy;2012-2013 Hans-J. Barthen DL5DI</font>\n";
?>

	</body>
