<?php
header('Content-Type: application/json');
$db = new SQLite3('/home/pi/db.sqlite');
$data = array();
    $results = $db->query("SELECT ID, Timestamp, AWATT, AVA, BWATT, BVA, CWATT, CVA, PWATT_ACC, PVAR_ACC FROM Power WHERE Timestamp >= datetime('now', '-1 hours', 'localtime')");
while ($row = $results->fetchArray()) {
	$data[] = array(
		"ID" => $row['ID'],
		"Timestamp" => $row['Timestamp'],
		"AWATT" => $row['AWATT'],
		"AVA" => $row['AVA'],
		"BWATT" => $row['BWATT'],
		"BVA" => $row['BVA'],
		"CWATT" => $row['CWATT'],
        "CVA" => $row['CVA'],
        "PWATT_ACC" => $row['PWATT_ACC'],
        "PVAR_ACC" => $row['PVAR_ACC']
	);
}
$db->close();
print json_encode($data);
?>
