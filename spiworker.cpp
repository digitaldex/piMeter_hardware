#include "spiworker.h"
#include <unistd.h>
#include <stdint.h>
#include <QThread>
#include <time.h>
#include <thread>
#include <iostream>
#include <bcm2835.h>
#include <stdio.h>
#include "spi.h"
#include "mysql.h"
#include <sys/time.h>
#include <math.h>
#include <string>

#define VoltageConstant     10.74705975
#define CurrentConstant     2.683410674
#define PowerConstant       3.870674811
#define EnergyConstant      1.100991946
#define PIN 		    RPI_BPLUS_GPIO_J8_37

using namespace std;

spiWorker::spiWorker(RingBuffer *spiDataBuffer)
{
    pBuffer = spiDataBuffer;
    bcm2835_init();
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(PIN, HIGH);
    initSPI();
    writeSPI(W_RUN_REGISTER_STOP);
    writeSPI(W_PGA_GAIN_REGISTER);
    //writeSPI(W_WTHR_REGISTER);
    //writeSPI(W_VARTHR_REGISTER);
    //writeSPI(W_VATHR_REGISTER);
    //writeSPI(W_CF1DEN_REGISTER);
    //writeSPI(W_CF2DEN_REGISTER);
    //writeSPI(W_CF3DEN_REGISTER);
    //writeSPI(W_CF4DEN_REGISTER);
    writeSPIlong(W_VLEVEL_REGISTER);
    writeSPI(W_ACCMODE_REGISTER);
    writeSPI(W_EP_CFG_REGISTER);
    writeSPI(W_EGY_TIME_REGISTER);
    writeSPI(W_RUN_REGISTER_START);
}

spiWorker::~spiWorker() {
    
}

void spiWorker::process() {
    string Timestamp2;
    int timeVar = 1;
    mySQLhandler sql;
    QJsonObject temp;
    time_t rawtime;
    struct tm * timeinfo;
    char timebuffer[80];
    char currentTime[84];
    QString Timestamp;
    uint32_t ADCreturnValueUnsigned;
    char spiReceive[8];
    float AWATTHR = 0, BWATTHR = 0, CWATTHR = 0, AVARHR = 0, BVARHR = 0, CVARHR = 0, AVAHR = 0, BVAHR = 0, CVAHR = 0;
    sql.initDBconnection();
    sql.getInitialValues(AWATTHR, BWATTHR, CWATTHR, AVARHR, BVARHR, CVARHR, AVAHR, BVAHR, CVAHR);

    while(1) {
        timeval curTime;
        gettimeofday(&curTime, NULL);
        int milli = curTime.tv_usec / 1000;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(timebuffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
        sprintf(currentTime, "%s:%d", timebuffer, milli);
        Timestamp = currentTime;
	    Timestamp2 = currentTime;
        
        temp.insert(QStringLiteral("Timestamp"), Timestamp);
        
        readSPI(spiReceive, R_AIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        temp.insert(QStringLiteral("AIRMS"), (float)((int)(ADCreturnValueUnsigned * CurrentConstant * pow(10, -6) * 100)) / 100.0);
        
        readSPI(spiReceive, R_BIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        temp.insert(QStringLiteral("BIRMS"), (float)((int)(ADCreturnValueUnsigned * CurrentConstant * pow(10, -6) * 100)) / 100.0);
        
        readSPI(spiReceive, R_CIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        temp.insert(QStringLiteral("CIRMS"), (float)((int)(ADCreturnValueUnsigned * CurrentConstant * pow(10, -6) * 100)) / 100.0);
        
        readSPI(spiReceive, R_AVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        temp.insert(QStringLiteral("AVRMS"), (float)((int)(ADCreturnValueUnsigned * VoltageConstant * pow(10, -6) * 100)) / 100.0);
        
        readSPI(spiReceive, R_BVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        temp.insert(QStringLiteral("BVRMS"), (float)((int)(ADCreturnValueUnsigned * VoltageConstant * pow(10, -6) * 100)) / 100.0);
        
        readSPI(spiReceive, R_CVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        temp.insert(QStringLiteral("CVRMS"), (float)((int)(ADCreturnValueUnsigned * VoltageConstant * pow(10, -6) * 100)) / 100.0);
        
        readSPI(spiReceive, R_AWATT_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        temp.insert(QStringLiteral("AWATT"), (float)((int)(ADCreturnValueUnsigned * PowerConstant * pow(10, -3) * 100)) / 100.0);
        
        readSPI(spiReceive, R_BWATT_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        temp.insert(QStringLiteral("BWATT"), (float)((int)(ADCreturnValueUnsigned * PowerConstant * pow(10, -3) * 100)) / 100.0);
        
        readSPI(spiReceive, R_CWATT_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        temp.insert(QStringLiteral("CWATT"), (float)((int)(ADCreturnValueUnsigned * PowerConstant * pow(10, -3) * 100)) / 100.0);

        if(timeVar == 60) {
            readSPI(spiReceive, R_AWATTHR_HI_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            AWATTHR += (float)((int)(ADCreturnValueUnsigned * EnergyConstant * pow(10, -6) * 100)) / 100.0;
            
            readSPI(spiReceive, R_BWATTHR_HI_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            BWATTHR += (float)((int)(ADCreturnValueUnsigned * EnergyConstant * pow(10, -6) * 100)) / 100.0;
            
            readSPI(spiReceive, R_CWATTHR_HI_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            CWATTHR += (float)((int)(ADCreturnValueUnsigned * EnergyConstant * pow(10, -6) * 100)) / 100.0;
            
            readSPI(spiReceive, R_AVARHR_HI_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            AVARHR += (float)((int)(ADCreturnValueUnsigned * EnergyConstant * pow(10, -6) * 100)) / 100.0;
            
            readSPI(spiReceive, R_BVARHR_HI_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            BVARHR += (float)((int)(ADCreturnValueUnsigned * EnergyConstant * pow(10, -6) * 100)) / 100.0;
            
            readSPI(spiReceive, R_CVARHR_HI_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            CVARHR += (float)((int)(ADCreturnValueUnsigned * EnergyConstant * pow(10, -6) * 100)) / 100.0;
            
            readSPI(spiReceive, R_AVAHR_HI_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            AVAHR += (float)((int)(ADCreturnValueUnsigned * EnergyConstant * pow(10, -6) * 100)) / 100.0;
            
            readSPI(spiReceive, R_BVAHR_HI_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            BVAHR += (float)((int)(ADCreturnValueUnsigned * EnergyConstant * pow(10, -6) * 100)) / 100.0;
            
            readSPI(spiReceive, R_CVAHR_HI_REGISTER);
            ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
            CVAHR += (float)((int)(ADCreturnValueUnsigned * EnergyConstant * pow(10, -6) * 100)) / 100.0;
	    
            sql.writeConsumptionTable(Timestamp2, AWATTHR, BWATTHR, CWATTHR, AVARHR, BVARHR, CVARHR, AVAHR, BVAHR, CVAHR);
	    
	        timeVar = 0;
        }

        
        pBuffer->push(temp);
        timeVar++;
        sleep(1);
    }
}
