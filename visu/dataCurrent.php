<?php
header('Content-Type: application/json');
$db = new SQLite3('/home/pi/db.sqlite');
$data = array();
    $results = $db->query("SELECT Current.ID, Current.Timestamp, AIRMS, BIRMS, CIRMS, ISUMRMS, AI_PCF, BI_PCF, CI_PCF, PWATT_ACC FROM Current INNER JOIN Power ON Current.ID = Power.ID WHERE Current.Timestamp >= datetime('now', '-1 hours', 'localtime')");
while ($row = $results->fetchArray()) {
	$data[] = array(
		"ID" => $row['ID'],
		"Timestamp" => $row['Timestamp'],
		"AIRMS" => $row['AIRMS'],
		"BIRMS" => $row['BIRMS'],
		"CIRMS" => $row['CIRMS'],
		"ISUMRMS" => $row['ISUMRMS'],
		"AI_PCF" => $row['AI_PCF'],
		"BI_PCF" => $row['BI_PCF'],
		"CI_PCF" => $row['CI_PCF'],
        "PWATT_ACC" => $row['PWATT_ACC']
	);
}
$db->close();
print json_encode($data);
?>
