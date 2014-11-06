<table BORDER=0 BGCOLOR=white>
  <tr align="left" valign="top">
    <td align ="left" valign="top">
    <font size=+0><b>STARnet Groups</b></font>
    <table BORDER=0 BGCOLOR=white>
    <font size=-1>
    <tr bgcolor=black>
    <th width=80><a class=tooltip href="#"><font color=white><center><b>Callsign</b></center></font><span><b>STARnet Callsign</b></span></a></th>
    <th width=80><a class=tooltip href="#"><font color=white><center><b>LogOff</b></center></font><span><b>STARnet Logoff Callsign</b></span></a></th>
    <th width=140><a class=tooltip href="#"><font color=white><center><b>Info</b></center></font><span><b>Infotext</b></span></a></th>
    <th width=40><a class=tooltip href="#"><font color=white><center><b>UTOT</b></center></font><span><b>User TimeOut (min)</b>inactivity time after which a user will be disconnected</span></a></th>
    <th width=40><a class=tooltip href="#"><font color=white><center><b>GTOT</b></center></font><span><b>Group TimeOut (min)</b>inactivity time after which the group will be disconnected</span></a></th>
    </tr>
<?php
    $ci = 0;
    $i = 0;
    $stngrp = array();
    for($i = 1;$i < 6; $i++){
	$param="starNetCallsign" . $i;
	if(isset($configs[$param])) {
	    $gname = $configs[$param];
	    $stngrp[$gname] = $i;
	    $ci++;
	    if($ci > 1) { $ci = 0; }
	    print "<tr bgcolor=\"$col[$ci]\">";
	    print "<td align=center>$gname</td>";
	    $param="starNetLogoff" . $i;
	    if(isset($configs[$param])){ print "<td>$configs[$param]</td>";} else { print"<td>&nbsp;</td>";}
	    $param="starNetInfo" . $i;
	    if(isset($configs[$param])){ print "<td>$configs[$param]</td>";} else { print"<td>&nbsp;</td>";}
	    $param="starNetUserTimeout" . $i;
	    if(isset($configs[$param])){ print "<td>$configs[$param]</td>";} else { print"<td>&nbsp;</td>";}
	    $param="starNetGroupTimeout" . $i;
	    if(isset($configs[$param])){ print "<td>$configs[$param]</td>";} else { print"<td>&nbsp;</td>";}
	    print "</tr>";
	}
    }
?>
    </table>
    </td>

    <td align ="left" valign="top">
    <font size=+0><b>STARnet Group Members</b></font>
    <table BORDER=0 BGCOLOR=white>
    <font size=-1>
    <tr bgcolor=black>
    <th width=80><a class=tooltip href="#"><font color=white><center><b>Group</b></center></font><span><b>STARnet Callsign</b></span></a></th>
    <th width=80><a class=tooltip href="#"><font color=white><center><b>Member</b></center></font><span><b>Callsign</b></span></a></th>
    <th width=120><a class=tooltip href="#"><font color=white><center><b>Timestamp</b></center></font><span><b>Time of Login</b></span></a></th>
    </tr>
<?php
	$groupsx = array();
	if ($starLog = fopen($starLogPath,'r')) {
		
		while($logLine = fgets($starLog)) {
		        preg_match_all('/^(.{19}).*(Adding|Removing) (.{8}).*StarNet group (.{8}).*$/',$logLine,$matches);
		        $groupz = $matches[4][0];
		        $member = $matches[3][0];
		        $action = $matches[2][0];
		        $date = $matches[1][0];
		        $guid = $stngrp[$groupz];
		        if ($action == 'Adding'){ 
		    	    $groupsx[$guid][$member] = $date; } 
		        elseif ($action == 'Removing'){
		            unset($groupsx[$guid][$member]);
		        }
		}
		fclose($starLog);
	}

	$ci = 0;
	$ulist = array();
	for($i = 1;$i < 6; $i++){
	    if(isset($groupsx[$i])){
		$ulist = $groupsx[$i];
		foreach ($ulist as $ucall => $ulogin){
		    $ci++;
		    if($ci > 1) { $ci = 0; }
		    print "<tr bgcolor=\"$col[$ci]\">";
		    print "<td>$groupz</td>";
		    print "<td>$ucall</td>";
		    print "<td>$ulogin</td>";
		    print "</tr>";
		}
	    }
	}
?>
    </table>
    </td>
  </tr>
</table>