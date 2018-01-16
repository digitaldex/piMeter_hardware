#include "spiworker.h"
#include <unistd.h>
#include <stdint.h>
#include <QThread>
#include <time.h>
#include <thread>
#include <iostream>
#include <bcm2835.h>
#include "spi.h"
#include <sys/time.h>
#include <math.h>

#define VoltageConstant     12.6254
#define CurrentConstant     0.9488
#define PowerConstant       1.6079

using namespace std;

spiWorker::spiWorker(RingBuffer *spiDataBuffer)
{
    pBuffer = spiDataBuffer;
    initSPI();
    writeSPI(W_RUN_REGISTER_STOP);
    writeSPIlong(W_AVGAIN_REGISTER);
    writeSPIlong(W_BVGAIN_REGISTER);
    writeSPIlong(W_CVGAIN_REGISTER);
    writeSPIlong(W_VLEVEL_REGISTER);
    writeSPI(W_ACCMODE_REGISTER);
    writeSPI(W_EP_CFG_REGISTER);
    writeSPI(W_RUN_REGISTER_START);
}

spiWorker::~spiWorker() {
    
}

void spiWorker::process()
{
    QJsonObject temp;
    time_t rawtime;
    struct tm * timeinfo;
    char timebuffer[80];
    char currentTime[84];
    QString Timestamp;
    uint32_t ADCreturnValueUnsigned;
    char spiReceive[8];
    int convBuffer;

    while(1) {
        timeval curTime;
        gettimeofday(&curTime, NULL);
        int milli = curTime.tv_usec / 1000;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(timebuffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
        sprintf(currentTime, "%s:%d", timebuffer, milli);
        Timestamp = currentTime;
        
        temp.insert(QStringLiteral("Timestamp"), Timestamp);
        
        readSPI(spiReceive, R_AIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        convBuffer = (ADCreturnValueUnsigned * CurrentConstant * pow(10, -6) * 100);
        temp.insert(QStringLiteral("AIRMS"), (double)convBuffer / 100);
        
        readSPI(spiReceive, R_BIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        convBuffer = (ADCreturnValueUnsigned * CurrentConstant * pow(10, -6) * 100);
        temp.insert(QStringLiteral("BIRMS"), (double)convBuffer / 100);
        
        readSPI(spiReceive, R_CIRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        convBuffer = (ADCreturnValueUnsigned * CurrentConstant * pow(10, -6) * 100);
        temp.insert(QStringLiteral("CIRMS"), (double)convBuffer / 100);
        
        readSPI(spiReceive, R_ISUMRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        convBuffer = (ADCreturnValueUnsigned * CurrentConstant * pow(10, -6) * 100);
        temp.insert(QStringLiteral("ISUMRMS"), (double)convBuffer / 100);
        
        readSPI(spiReceive, R_AVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        convBuffer = (ADCreturnValueUnsigned * VoltageConstant * pow(10, -6) * 100);
        temp.insert(QStringLiteral("AVRMS"), (double)convBuffer / 100);
        
        readSPI(spiReceive, R_BVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        convBuffer = (ADCreturnValueUnsigned * VoltageConstant * pow(10, -6) * 100);
        temp.insert(QStringLiteral("BVRMS"), (double)convBuffer / 100);
        
        readSPI(spiReceive, R_CVRMS_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        convBuffer = (ADCreturnValueUnsigned * VoltageConstant * pow(10, -6) * 100);
        temp.insert(QStringLiteral("CVRMS"), (double)convBuffer / 100);
        
        readSPI(spiReceive, R_AWATT_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        convBuffer = (ADCreturnValueUnsigned * PowerConstant * pow(10, -3) * 100);
        temp.insert(QStringLiteral("AWATT"), (double)convBuffer / 100);
        
        readSPI(spiReceive, R_BWATT_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        convBuffer = (ADCreturnValueUnsigned * PowerConstant * pow(10, -3) * 100);
        temp.insert(QStringLiteral("BWATT"), (double)convBuffer / 100);
        
        readSPI(spiReceive, R_CWATT_REGISTER);
        ADCreturnValueUnsigned = parse32bitReturnValue(spiReceive);
        convBuffer = (ADCreturnValueUnsigned * PowerConstant * pow(10, -3) * 100);
        temp.insert(QStringLiteral("CWATT"), (double)convBuffer / 100);

        pBuffer->push(temp);
        sleep(1);
    }
}
