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
#include <thread>
#include "db.h"
#include "spi.h"
#include "configParser.h"

using namespace std;

int main() {
    /* Config Readout */
    const char *configPath = "/home/pi/piMeter/config.ini";
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
    
    /* Create Tables if not exists */
    createDBTables();
    
    /* Inits SPI and start Measuring on ADE9000 */
    initSPI();
    writeSPI(W_RUN_REGISTER_START);
    
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
            
            /* Read Current Values */
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
                valueMap["powerL1"][i] = valueMap.find("currentL1RMS")->second[i] * 230;
                valueMap["powerL2"][i] = valueMap.find("currentL2RMS")->second[i] * 230;
                valueMap["powerL3"][i] = valueMap.find("currentL3RMS")->second[i] * 230;
                valueMap["powerSumm"][i] = valueMap.find("powerL1")->second[i] + valueMap.find("powerL2")->second[i] + valueMap.find("powerL3")->second[i];
                
                /* Read Voltage Values */
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
                /* Calculate Power with Fixed Voltage */
                valueMap["powerL1"][i] = valueMap.find("currentL1RMS")->second[i] * 230;
                valueMap["powerL2"][i] = valueMap.find("currentL2RMS")->second[i] * 230;
                valueMap["powerL3"][i] = valueMap.find("currentL3RMS")->second[i] * 230;
                valueMap["powerSumm"][i] = valueMap.find("powerL1")->second[i] + valueMap.find("powerL2")->second[i] + valueMap.find("powerL3")->second[i];
                
                /* Set Voltage NULL */
                valueMap["voltageL1RMS"][i] = 0;
                valueMap["voltageL2RMS"][i] = 0;
                valueMap["voltageL3RMS"][i] = 0;
                valueMap["voltageL1"][i] = 0;
                valueMap["voltageL2"][i] = 0;
                valueMap["voltageL3"][i] = 0;
            }
            
            /* Start DB Write Thread every 60 seconds */
            if(i == 59) {
                thread worker(proceedDBWrite, valueMap, Timestamp);
                worker.detach();
                i = -1;
            }
            sleep(stoi(configMap.find("CycleTime")->second));
        }
    }
    
    writeSPI(W_RUN_REGISTER_STOP);

    closeSPI();
}
