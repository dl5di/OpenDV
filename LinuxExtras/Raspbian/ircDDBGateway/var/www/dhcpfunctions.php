<?php

function parser($open_file,$logfile) 
{
	$line_number = 0;
	$row_array = array(array());
	while (!feof($open_file))
	{	
		$read_line = fgets($open_file, 4096);
		if (substr($read_line, 0, 1) != "#") //check for comment (skip)
		{
			$tok = strtok($read_line, " ");
			if ($tok == "lease")
			{
				$row_array[$line_number] = initialize_array();
				$row_array[$line_number][0] = strtok(" ")."\n";
			}
			else if ($tok == "starts")
			{
				$row_array[$line_number][1] =   intToDay(strtok(" "));
				$row_array[$line_number][1] = $row_array[$line_number][1]." - " . strtok(" ") . " ";
				$time = strtok(" ");
				$time = str_replace(";", "", $time);
				$row_array[$line_number][1] = $row_array[$line_number][1].$time ;
			}
			else if ($tok == "ends")
			{
				$row_array[$line_number][2] = intToDay(strtok(" "));
				$row_array[$line_number][2] = $row_array[$line_number][2]." - " . strtok(" ") . " ";
				$time = strtok(" ");
				$time = str_replace(";", "", $time);
				$row_array[$line_number][2] = $row_array[$line_number][2].$time ;
			}
			else if ($tok == "hardware")
			{
				$row_array[$line_number][3] = strtok(" ") . " - ";
				$MAC = strtok(" ");
				$MAC = strtoupper(str_replace(";", "", $MAC));
				$MAC = strtoupper(str_replace("ethernet - ", "", $MAC));
				$row_array[$line_number][3] = $MAC." (".getmacvendor($MAC).")";
				$row_array[$line_number][4] = getcallsign($MAC,$logfile);
			}
			else if ($tok == "binding")
			{
				$row_array[$line_number][5] = strtok(" ");
				$state = strtok(" ");
				$state = str_replace(";", "", $state);
				$row_array[$line_number][5] = strtoupper(str_replace("state", "", $state));
			}
			else if ($tok == "}\n")
			{
				$row_array[$line_number][7] = $row_array[$line_number][6];
				$line_number++;
			}
			
		}
	}
	return $row_array;
}


function intToDay($integer)
{
	if ($integer == 0)
	return "Sunday";
	else if ($integer == 1)
	return "Monday";
	else if ($integer == 2)
	return "Tuesday";
	else if ($integer == 3)
	return "Wednesday";
	else if ($integer == 4)
	return "Thursday";
	else if ($integer == 5)
	return "Friday";
	else
	return "Saturday";
}

function initialize_array()
{
	$row_array = array();
	for ($i = 0; $i < 7; $i++) {
		$row_array[$i] = "-";
	}
	return $row_array;
}

function print_line($row)
{
	for ($i = 0; $i < 6; $i++) {
		switch ($i) {
		case 0: 
			//Callsign
			echo "<td>".$row[4]."</td>\n"; 
			break;
		case 1: 
			//IP Address
			echo "<td>".$row[0]. "</td>\n"; 
			break;
		case 2: 
			//Start Time
			echo "<td>".$row[1]. "</td>\n"; 
			break;
		case 3: 
			//End Time
			echo "<td>".$row[2]. "</td>\n"; 
			break;
		case 4: 
			//MAC Address
			echo "<td>".$row[3]."</td>\n"; 
			break;
		case 5: 
			//Status
			echo "<td>".$row[5]."</td>\n"; 
			break;
		}
	}
}

function getmacvendor($mac_unformated)
{
	//Can be retrived on nmap http://nmap.org/book/nmap-mac-prefixes.html 
	//or via http://standards.ieee.org/develop/regauth/oui/oui.txt
	//Location of the mac vendor list file
	$mac_vendor_file = "./nmap-mac-prefixes";

	$mac = substr(strtoupper(str_replace(array(":"," ","-"), "", $mac_unformated)),0,6);

	$open_file = fopen($mac_vendor_file, "r") or die("Unable to open MAC VENDOR file.");
	if ($open_file)
	{
		while (!feof($open_file))
		{
			 $read_line = fgets($open_file, 4096);
			 if (substr($read_line, 0, 6) == $mac) {
				return substr($read_line, 7, -1);
			 }
		}
		
		fclose($open_file);
	}
	return "Unknown device";
}


function getcallsign($mac,$ddmode_log)
{
	//mac-add<->callsign assignment can be found in DDMode.log which is created in logpath of ircDDBGateway
	//path needs to be set in ircddblocal.php

	$open_file = fopen($ddmode_log, "r") or die("Unable to open DDMode-Logfile.");
	if ($open_file)
	{
		$mac = rtrim($mac);
		while (!feof($open_file))
		{
			 $read_line = fgets($open_file, 4096);
			 if (strtoupper(substr($read_line, 21, 17)) == $mac) {
				return substr($read_line, 39, 8);
			 }
		}
		
		fclose($open_file);
	}
#	$res = "/".$mac."/".strtoupper(substr($read_line, 21, 17))."/";
	return "********$res";
}

?>
