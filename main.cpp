#include <bcm2835.h>
#include <sqlite3.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <sys/time.h>
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
    /* ADE Readout */
    map<string, float[60]> valueMap;
    uint32_t ADCreturnValueUnsigned;
    int32_t ADCreturnValueSigned;
    char spiReceive[8];
    /* Time */
    time_t rawtime;
    struct tm * timeinfo;
    char timebuffer[80];
    char currentTime[84];
    string Timestamp[60];
    
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
    sql = "CREATE TABLE if not exists Power (ID INTEGER PRIMARY KEY, Timestamp TEXT, AWATT real, AVA real, BWATT real, BVA real, CWATT real, CVA real, PWATT_ACC real, PVAR_ACC real)";
    if(createDB(dbPath, dbPointer, stmt, sql)) {
        cerr << "SQL ERROR: Could not create Power Database" << endl;
    };

    while(1) {
        for(int i = 0; i < 60; i++) {
            /* Get Time */
            timeval curTime;
            gettimeofday(&curTime, NULL);
            int milli = curTime.tv_usec / 1000;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(timebuffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
            sprintf(currentTime, "%s:%d", timebuffer, milli);
            Timestamp[i] = currentTime;
            
            /* Read Current Values an write to DB */
            readSPI(spiReceive, R_AIRMS_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            valueMap["currentL1RMS"][i] = calcUint32ToFloat(ADCreturnValueUnsigned);
            
            readSPI(spiReceive, R_BIRMS_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            valueMap["currentL2RMS"][i] = calcUint32ToFloat(ADCreturnValueUnsigned);
            
            readSPI(spiReceive, R_CIRMS_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            valueMap["currentL3RMS"][i] = calcUint32ToFloat(ADCreturnValueUnsigned);
            
            readSPI(spiReceive, R_ISUMRMS_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            valueMap["currentSummRMS"][i] = calcUint32ToFloat(ADCreturnValueUnsigned);
            
            readSPI(spiReceive, R_AI_PCF_REGISTER);
            ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
            valueMap["currentL1"][i] = calcInt32ToFloat(ADCreturnValueSigned);
            
            readSPI(spiReceive, R_BI_PCF_REGISTER);
            ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
            valueMap["currentL2"][i] = calcInt32ToFloat(ADCreturnValueSigned);
            
            readSPI(spiReceive, R_CI_PCF_REGISTER);
            ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
            valueMap["currentL3"][i] = calcInt32ToFloat(ADCreturnValueSigned);

            if(configMap.find("UseVoltageChannel")->second == "true") {
                /* Insert ADE9000 Power Readout here */
                
                /* Read Voltage Values an write to DB */
                readSPI(spiReceive, R_AVRMS_REGISTER);
                ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
                valueMap["voltageL1RMS"][i] = calcUint32ToFloat(ADCreturnValueUnsigned);
                
                readSPI(spiReceive, R_BVRMS_REGISTER);
                ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
                valueMap["voltageL2RMS"][i] = calcUint32ToFloat(ADCreturnValueUnsigned);
                
                readSPI(spiReceive, R_CVRMS_REGISTER);
                ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
                valueMap["voltageL3RMS"][i] = calcUint32ToFloat(ADCreturnValueUnsigned);
                
                readSPI(spiReceive, R_AV_PCF_REGISTER);
                ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
                valueMap["voltageL1"][i] = calcInt32ToFloat(ADCreturnValueSigned);
                
                readSPI(spiReceive, R_BV_PCF_REGISTER);
                ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
                valueMap["voltageL2"][i] = calcInt32ToFloat(ADCreturnValueSigned);
                
                readSPI(spiReceive, R_CV_PCF_REGISTER);
                ADCreturnValueSigned = parse32bitReturnValue(spiReceive);
                valueMap["voltageL3"][i] = calcInt32ToFloat(ADCreturnValueSigned);
                
                
            }
            else {
                valueMap["powerL1"][i] = valueMap.find("currentL1RMS")->second[i] * 230;
                valueMap["powerL2"][i] = valueMap.find("currentL2RMS")->second[i] * 230;
                valueMap["powerL3"][i] = valueMap.find("currentL3RMS")->second[i] * 230;
                valueMap["powerSumm"][i] = valueMap.find("powerL1")->second[i] + valueMap.find("powerL2")->second[i] + valueMap.find("powerL3")->second[i];
                
                valueMap["voltageL1RMS"][i] = 0;
                valueMap["voltageL2RMS"][i] = 0;
                valueMap["voltageL3RMS"][i] = 0;
                valueMap["voltageL1"][i] = 0;
                valueMap["voltageL2"][i] = 0;
                valueMap["voltageL3"][i] = 0;
            }

            if(i == 59) {
                for(int y = 0; y < 60; y++) {
                    statement = "INSERT INTO Current (Timestamp, AIRMS, BIRMS, CIRMS, ISUMRMS, AI_PCF, BI_PCF, CI_PCF) VALUES ('"+ Timestamp[y] +"', '"+ to_string(valueMap.find("currentL1RMS")->second[y]) +"', '"+ to_string(valueMap.find("currentL2RMS")->second[y]) +"', '"+ to_string(valueMap.find("currentL3RMS")->second[y]) +"', '"+ to_string(valueMap.find("currentSummRMS")->second[y]) +"', '"+ to_string(valueMap.find("currentL1")->second[y]) +"', '"+ to_string(valueMap.find("currentL2")->second[y]) +"', '"+ to_string(valueMap.find("currentL3")->second[y]) +"')";
                    sql = statement.c_str();
                    writeDB(dbPath,dbPointer, stmt, sql);
                    
                    statement = "INSERT INTO Voltage (Timestamp, AVRMS, BVRMS, CVRMS, AV_PCF, BV_PCF, CV_PCF) VALUES ('"+ Timestamp[y] +"', '"+ to_string(valueMap.find("voltageL1RMS")->second[y]) +"', '"+ to_string(valueMap.find("voltageL2RMS")->second[y]) +"', '"+ to_string(valueMap.find("voltageL3RMS")->second[y]) +"', '"+ to_string(valueMap.find("voltageL1")->second[y]) +"', '"+ to_string(valueMap.find("voltageL2")->second[y]) +"', '"+ to_string(valueMap.find("voltageL3")->second[y]) +"')";
                    sql = statement.c_str();
                    writeDB(dbPath,dbPointer, stmt, sql);

                    statement = "INSERT INTO Power (Timestamp, AWATT, AVA, BWATT, BVA, CWATT, CVA, PWATT_ACC, PVAR_ACC) VALUES ('"+ Timestamp[y] +"', '"+ to_string(valueMap.find("powerL1")->second[y]) +"', 'NULL', '"+ to_string(valueMap.find("powerL2")->second[y]) +"', 'NULL', '"+ to_string(valueMap.find("powerL3")->second[y]) +"', 'NULL', '"+ to_string(valueMap.find("powerSumm")->second[y]) +"', 'NULL')";
                    sql = statement.c_str();
                    writeDB(dbPath,dbPointer, stmt, sql);
                }
                i = 0;
            }
            sleep(stoi(configMap.find("CycleTime")->second));
        }
    }
    
    writeSPI(W_RUN_REGISTER_STOP);

    closeSPI();
}
