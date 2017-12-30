#include <bcm2835.h>
#include <sqlite3.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include "db.h"
#include "spi.h"

using namespace std;

float calcUint32ToFloat(uint32_t &buffer) {
    float calculatedValue;
    if(buffer != 0) {
        calculatedValue = 0;
        calculatedValue = 52702092 / buffer;
        calculatedValue = 50 / calculatedValue;
    }
    return calculatedValue;
}

float calcInt32ToFloat(int32_t &buffer) {
    float calculatedValue;
    if(buffer != 0) {
        calculatedValue = 0;
        calculatedValue = 52702092 / buffer;
        calculatedValue = 50 / calculatedValue;
    }
    return calculatedValue;
}


int main() {
    
    sqlite3 *dbPointer;
    sqlite3_stmt *stmt;
    string statement;
    const char *dbPath = "/home/pi/db.sqlite";
    const char *sql;
    
    /* CREATE CURRENT TABLE IF NOT EXISTS */
    sql = "CREATE TABLE if not exists Current (ID INTEGER PRIMARY KEY, Timestamp TEXT, AIRMS real, BIRMS real, CIRMS real, ISUMRMS real, AI_PCF real, BI_PCF real, CI_PCF real)";
    if(createDB(dbPath, dbPointer, stmt, sql)) {
        cerr << "SQL ERROR: Could not create Current Database" << endl;
    };
    
    /* CREATE VOLTAGE TABLE IF NOT EXISTS */
    sql = "CREATE TABLE if not exists Voltage (ID INTEGER PRIMARY KEY, Timestamp TEXT, AVRMS real, BVRMS real, CVRMS real, AV_PCF real, BV_PCF real, CV_PCF real)";
    if(createDB(dbPath, dbPointer, stmt, sql)) {
        cerr << "SQL ERROR: Could not create Voltage Database" << endl;
    };
    
    /* CREATE POWER TABLE IF NOT EXISTS */
    sql = "CREATE TABLE if not exists Power (ID INTEGER PRIMARY KEY, Timestamp TEXT, AFWATT real, AFVA real, BFWATT real, BFVA real, CFWATT real, CFVA real, PWATT_ACC real, PVAR_ACC real)";
    if(createDB(dbPath, dbPointer, stmt, sql)) {
        cerr << "SQL ERROR: Could not create Power Database" << endl;
    };
    
    float currentL1RMS;
    float currentL2RMS;
    float currentL3RMS;
    float currentSummRMS;
    float currentL1;
    float currentL2;
    float currentL3;
    float voltageL1RMS;
    float voltageL2RMS;
    float voltageL3RMS;
    float voltageL1;
    float voltageL2;
    float voltageL3;
    uint32_t ADCreturnValueUnsigned;
    int32_t ADCreturnValueSigned;
    char spiReceive[8];

    initSPI();
    writeSPI(W_RUN_REGISTER_START);
    
    while(1) {
        /* Read Current Values an write to DB */
        readSPI(spiReceive, R_AIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        currentL1RMS = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_BIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        currentL2RMS = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_CIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        currentL3RMS = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_ISUMRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        currentSummRMS = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_AI_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        currentL1 = calcInt32ToFloat(ADCreturnValueSigned);
        
        readSPI(spiReceive, R_BI_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        currentL2 = calcInt32ToFloat(ADCreturnValueSigned);
        
        readSPI(spiReceive, R_CI_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        currentL3 = calcInt32ToFloat(ADCreturnValueSigned);

        statement = "INSERT INTO Current (Timestamp, AIRMS, BIRMS, CIRMS, ISUMRMS, AI_PCF, BI_PCF, CI_PCF) VALUES (datetime('now', 'localtime'), '"+ to_string(currentL1RMS) +"', '"+ to_string(currentL2RMS) +"', '"+ to_string(currentL3RMS) +"', '"+ to_string(currentSummRMS) +"', '"+ to_string(currentL1) +"', '"+ to_string(currentL2) +"', '"+ to_string(currentL3) +"')";
        sql = statement.c_str();
        writeDB(dbPath,dbPointer, stmt, sql);
        
        /* Read Voltage Values an write to DB */
        readSPI(spiReceive, R_AVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        voltageL1RMS = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_BVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        voltageL2RMS = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_CVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        voltageL3RMS = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_AV_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        voltageL1 = calcInt32ToFloat(ADCreturnValueSigned);
        
        readSPI(spiReceive, R_BV_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        voltageL2 = calcInt32ToFloat(ADCreturnValueSigned);
        
        readSPI(spiReceive, R_CV_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        voltageL3 = calcInt32ToFloat(ADCreturnValueSigned);
        
        statement = "INSERT INTO Voltage (Timestamp, AVRMS, BVRMS, CVRMS, AV_PCF, BV_PCF, CV_PCF) VALUES (datetime('now', 'localtime'), '"+ to_string(voltageL1RMS) +"', '"+ to_string(voltageL2RMS) +"', '"+ to_string(voltageL3RMS) +"', '"+ to_string(voltageL1) +"', '"+ to_string(voltageL2) +"', '"+ to_string(voltageL3) +"')";
        sql = statement.c_str();
        writeDB(dbPath,dbPointer, stmt, sql);
        
        sleep(15);
    }
    
    writeSPI(W_RUN_REGISTER_STOP);

    closeSPI();
}
