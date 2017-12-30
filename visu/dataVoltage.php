<?php
header('Content-Type: application/json');
$db = new SQLite3('/home/pi/db.sqlite');
$data = array();
    $results = $db->query("SELECT ID, Timestamp, AVRMS, BVRMS, CVRMS, AV_PCF, BV_PCF, CV_PCF FROM Voltage WHERE Timestamp >= datetime('now', '-1 hours', 'localtime')");
while ($row = $results->fetchArray()) {
	$data[] = array(
		"ID" => $row['ID'],
		"Timestamp" => $row['Timestamp'],
		"AVRMS" => $row['AVRMS'],
		"BVRMS" => $row['BVRMS'],
		"CVRMS" => $row['CVRMS'],
		"AV_PCF" => $row['AV_PCF'],
		"BV_PCF" => $row['BV_PCF'],
		"CV_PCF" => $row['CV_PCF']
	);
}
$db->close();
print json_encode($data);
?>
