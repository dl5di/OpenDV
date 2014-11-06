<?php
#
# based on phpdhcpd
# http://www.rivetcode.com/
# Version: 0.5
#
# modified for DStar-DD by Hans-J. Barthen DL5DI (20121020)
#
    for($i = 1;$i < 5; $i++){
	$param="repeaterBand" . $i;
	if(isset($configs[$param])) {
	    $t1 = substr($configs[$param],0,2);
	    if(substr($configs[$param],1,1) == "D"){
		require_once("dhcpfunctions.php");

		//read leases file
		if (file_exists($dhcpd_leases_file) && is_readable($dhcpd_leases_file)){
		    $open_file = fopen($dhcpd_leases_file, "r") or die("Unable to open DHCP leases file.");
		    if ($open_file){
			//Create a 2-dimensional table for the dhcplease file
			$dhcptable = array(array());
			//Call the dhcplease file parser
			$dhcptable = parser($open_file,$ddmode_log);
			?>
			<p>
			<font size=+1><b>DHCP-Leases of DD-Modules</b></font>
			<table BORDER=0 BGCOLOR=white>
			<font size=-1>
			<tr bgcolor=black>
			    <th width=80><a class=tooltip href="#"><font color=white><center><b>Callsign</b></center></font><span><b>Callsign of the DD-mode user</b></span></a></th>
			    <th width=80><a class=tooltip href="#"><font color=white><center><b>IP Address</b></center></font><span><b>Dynamic IP address of the DD-Mode user</b></span></a></th>
			    <th width=130><a class=tooltip href="#"><font color=white><center><b>Start Time (UTC)</b></center></font><span><b>Start time of DHCP lease</b>(UTC)</span></a></th>
			    <th width=130><a class=tooltip href="#"><font color=white><center><b>End Time (UTC)</b></center></font><span><b>End time of DHCP Lease</b>Expire time of DHCP lease (UTC)</span></a></th>
			    <th width=130><a class=tooltip href="#"><font color=white><center><b>MAC</b></center></font><span><b>MAC address of device</b>vendor information</span></a></th>
			    <th width=80><a class=tooltip href="#"><font color=white><center><b>Lease State</b></center></font><span><b>State of DHCP Lease</b></span></a></th>
			</tr>

			<?php
			$ci = 0;
			for ($line = count($dhcptable)-1; $line >= 0; $line--){
        		    $ci++;
                    	    if($ci > 1) { $ci = 0; }
                            print "<tr bgcolor=\"$col[$ci]\">";
			    print_line($dhcptable[$line]);
			    print "</tr>";
			}			
			fclose($open_file);
			echo "</table>";
		    }
		} else	{
		    echo "<p class='error'>The DHCP leases file does not exist or does not have sufficient read privileges.</p>";
		}
	    }
	}
    }
?>
