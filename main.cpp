#include <bcm2835.h>
#include <sqlite3.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include "db.h"
#include "spi.h"
#include "configParser.h"

using namespace std;

float calcUint32ToFloat(uint32_t &buffer) {
    /* Caluclates the uint32 ADC return value to decimal */
    float calculatedValue;
    if(buffer != 0) {
        calculatedValue = 0;
        calculatedValue = 52702092 / buffer;
        calculatedValue = 50 / calculatedValue;
    }
    return calculatedValue;
}

float calcInt32ToFloat(int32_t &buffer) {
    /* Calculates the int32 ADC return value to decimal */
    float calculatedValue;
    if(buffer != 0) {
        calculatedValue = 0;
        calculatedValue = 52702092 / buffer;
        calculatedValue = 50 / calculatedValue;
    }
    return calculatedValue;
}


int main() {
    /* SQL */
    const char *dbPath = "/home/pi/db.sqlite";
    const char *configPath = "/home/pi/piMeter/config.ini";
    sqlite3 *dbPointer;
    sqlite3_stmt *stmt;
    string statement;
    const char *sql;
    /* Config Readout */
    map<string, string> configMap;
    map<string, string> configMap_iter;
    /* ADE Readout */
    map<string, float> valueMap;
    map<string, float>::iterator valueMap_iter;
    uint32_t ADCreturnValueUnsigned;
    int32_t ADCreturnValueSigned;
    char spiReceive[8];
    
    /* Reads Out Config.ini */
    parseConfigFile(configPath, configMap);
    
    /* Inits SPI and start Measuring on ADE9000 */
    initSPI();
    writeSPI(W_RUN_REGISTER_START);
    
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
    
    while(1) {
        /* Read Current Values an write to DB */
        readSPI(spiReceive, R_AIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        valueMap["currentL1RMS"] = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_BIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        valueMap["currentL2RMS"] = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_CIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        valueMap["currentL3RMS"] = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_ISUMRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        valueMap["currentSummRMS"] = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_AI_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        valueMap["currentL1"] = calcInt32ToFloat(ADCreturnValueSigned);
        
        readSPI(spiReceive, R_BI_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        valueMap["currentL2"] = calcInt32ToFloat(ADCreturnValueSigned);
        
        readSPI(spiReceive, R_CI_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        valueMap["currentL3"] = calcInt32ToFloat(ADCreturnValueSigned);

        statement = "INSERT INTO Current (Timestamp, AIRMS, BIRMS, CIRMS, ISUMRMS, AI_PCF, BI_PCF, CI_PCF) VALUES (datetime('now', 'localtime'), '"+ to_string(valueMap.find("currentL1RMS")->second) +"', '"+ to_string(valueMap.find("currentL2RMS")->second) +"', '"+ to_string(valueMap.find("currentL3RMS")->second) +"', '"+ to_string(valueMap.find("currentSummRMS")->second) +"', '"+ to_string(valueMap.find("currentL1")->second) +"', '"+ to_string(valueMap.find("currentL2")->second) +"', '"+ to_string(valueMap.find("currentL3")->second) +"')";
        sql = statement.c_str();
        writeDB(dbPath,dbPointer, stmt, sql);
        
        /* Read Voltage Values an write to DB */
        readSPI(spiReceive, R_AVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        valueMap["voltageL1RMS"] = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_BVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        valueMap["voltageL2RMS"] = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_CVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        valueMap["voltageL3RMS"] = calcUint32ToFloat(ADCreturnValueUnsigned);
        
        readSPI(spiReceive, R_AV_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        valueMap["voltageL1"] = calcInt32ToFloat(ADCreturnValueSigned);
        
        readSPI(spiReceive, R_BV_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        valueMap["voltageL2"] = calcInt32ToFloat(ADCreturnValueSigned);
        
        readSPI(spiReceive, R_CV_PCF_REGISTER);
        ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
        valueMap["voltageL3"] = calcInt32ToFloat(ADCreturnValueSigned);
        
        statement = "INSERT INTO Voltage (Timestamp, AVRMS, BVRMS, CVRMS, AV_PCF, BV_PCF, CV_PCF) VALUES (datetime('now', 'localtime'), '"+ to_string(valueMap.find("voltageL1RMS")->second) +"', '"+ to_string(valueMap.find("voltageL2RMS")->second) +"', '"+ to_string(valueMap.find("voltageL3RMS")->second) +"', '"+ to_string(valueMap.find("voltageL1")->second) +"', '"+ to_string(valueMap.find("voltageL2")->second) +"', '"+ to_string(valueMap.find("voltageL3")->second) +"')";
        sql = statement.c_str();
        writeDB(dbPath,dbPointer, stmt, sql);
        
        sleep(stoi(configMap.find("CycleTime")->second));
    }
    
    writeSPI(W_RUN_REGISTER_STOP);

    closeSPI();
}
