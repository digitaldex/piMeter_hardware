using namespace std;

#define WORD_SIZE_READ          8U
#define WORD_SIZE_WRITE         4U
#define R_STATUS_REGISTER       0b0100011100101000
#define R_RUN_REGISTER          0b0100100000001000
#define R_STATUS0_REGISTER      0b0100000000101000
#define R_STATUS1_REGISTER      0b0100000000111000
#define R_CONFIG0_REGISTER      0b0000011000001000
#define R_TMPRSLT_REGISTER      0b0100101101111000
#define R_WR_LOCK_REGISTER      0b0100101111111000

#define R_AI_PCF_REGISTER       0b0010000010101000
#define R_AV_PCF_REGISTER       0b0010000010111000
#define R_AIRMS_REGISTER        0b0010000011001000
#define R_AVRMS_REGISTER        0b0010000011011000
#define R_AWATT_REGISTER        0b0010000100001000
#define R_AVAR_REGISTER         0b0010000100011000
#define R_AVA_REGISTER          0b0010000100101000
#define R_APF_REGISTER          0b0010000101101000

#define R_BI_PCF_REGISTER       0b0010001010101000
#define R_BV_PCF_REGISTER       0b0010001010111000
#define R_BIRMS_REGISTER        0b0010001011001000
#define R_BVRMS_REGISTER        0b0010001011011000
#define R_BWATT_REGISTER        0b0010001100001000
#define R_BVAR_REGISTER         0b0010001100011000
#define R_BVA_REGISTER          0b0010001100101000
#define R_BPF_REGISTER          0b0010001101101000

#define R_CI_PCF_REGISTER       0b0010010010101000
#define R_CV_PCF_REGISTER       0b0010010010111000
#define R_CIRMS_REGISTER        0b0010010011001000
#define R_CVRMS_REGISTER        0b0010010011011000
#define R_CWATT_REGISTER        0b0010010100001000
#define R_CVAR_REGISTER         0b0010010100011000
#define R_CVA_REGISTER          0b0010010100101000
#define R_CPF_REGISTER          0b0010010101101000

#define R_ISUMRMS_REGISTER      0b0010011010011000
#define R_PWATT_ACC_REGISTER    0b0011100101111000
#define R_PVAR_ACC_REGISTER     0b0011100111111000

#define W_TEMP_REGISTER         0b01001011011000000000000000001110
#define W_RUN_REGISTER_START    0b01001000000000000000000000000001
#define W_RUN_REGISTER_STOP     0b01001000000000000000000000000000
#define W_CONFIG0_REGISTER      0b00000110000000000000000000000011


int initSPI() {
    if(!bcm2835_init()) {
        return 1;
    }
    if(!bcm2835_spi_begin()) {
        return 1;
    }
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
    usleep(50000);
    return 0;
}

void closeSPI() {
    bcm2835_spi_end();
    bcm2835_close();
}

void resizeWord16(char byte[], unsigned short word) {
    /* Split 16bit SPI Commmand Word into 2 Bytes for Read Operation */
	byte[0] = word >> 8;
	byte[1] = word & 0xFF;
}

void resizeWord32(char byte[], unsigned int word) {
    /* Split 32bit SPI Commmand Word into 4 Bytes for Write Operation */
	byte[0] = word >> 24;
	byte[1] = word >> 16;
	byte[2] = word >> 8;
	byte[3] = word & 0xFF;
}

uint32_t parse32bitReturnValue(char byte[]) {
    /* Shift Char Array into uint32_t */
    /* First two Bytes are SPI Read Command Echo */
    /* Last two Bytes are CRC for Debug purpose */
    uint32_t value = 0;
    value = (uint8_t)byte[2];
    value <<= 8;
    value |= (uint8_t)byte[3];
    value <<= 8;
    value |= (uint8_t)byte[4];
    value <<= 8;
    value |= (uint8_t)byte[5];
    /* DEBUG
    uint16_t crc = 0;
    crc = (uint8_t)byte[6];
    crc <<= 8;
    crc |= (uint8_t)byte[7];
    cout << "DEBUG: " << hex << value << " CRC: " << crc << endl;
    */
    return value;
}

uint16_t parse16bitReturnValue(char byte[]) {
    /* Shift Char Array into uint16_t */
    /* First two Bytes are SPI Read Command Echo */
	uint16_t value = 0;
	value = (uint8_t)byte[2];
	value <<= 8;
	value |= (uint8_t)byte[3];
	return value;
}

void writeSPI(unsigned int word) {
    /* The Register Write WORD is shifted into 4 Bytes */
    /* and gets transfered over SPI Bus */
    char spiWriteCommand[4];
    resizeWord32(spiWriteCommand, word);
    bcm2835_spi_transfern(spiWriteCommand, WORD_SIZE_WRITE);
    usleep(50);
}

void readSPI(char spiReceive[], unsigned short word) {
    /* The Register Read WORD is shifted into 2 Bytes */
    /* and gets transfered over SPI Bus */
    /* The Response ist stored in spiReceive Array */
    char spiReadCommand[2];
    resizeWord16(spiReadCommand, word);
    bcm2835_spi_transfernb(spiReadCommand, spiReceive, WORD_SIZE_READ);
    usleep(50);
}

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
