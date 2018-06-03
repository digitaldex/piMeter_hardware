# piMeter - a SmartMeter for Raspberry Pi

## Frontend
![alt text](https://github.com/digitaldex/piMeter/blob/master/visu/visu1.png "Visu Screenshot")

![alt text](https://github.com/digitaldex/piMeter/blob/master/visu/visu2.png "Visu2 Screenshot")

## Board
![alt text](https://github.com/digitaldex/piMeter/blob/master/eagle/piMeter.png "Eagle Board Screenshot")

## Case
![alt text](https://github.com/digitaldex/piMeter/blob/master/eagle/case.png "piMeter Case Screenshot")

## Schematics
[piMeter Schematics](https://github.com/digitaldex/piMeter/blob/master/eagle/Schematics.pdf)


## Setup Raspberry Pi

### Enable SPI
```
sudo raspi-config
-> Interface Option
-> Enable SPI
```
### Install Dependencies
```
// Install lighttpd
sudo apt-get install lighttpd
// Install bcm2835 Library
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.52.tar.gz
tar zxvf bcm2835-1.52.tar.gz
./configure
make
sudo make check
sudo make install
// Install Qt
sudo apt-get install qt5-default
sudo apt install libqt5websockets5-dev
```
### Install Visu
Link visu files into lighttpd www root
```
sudo ln -s /home/pi/piMeter/visu/index.html /var/www/html/
sudo ln -s /home/pi/piMeter/visu/realtime.html /var/www/html/
sudo ln -s /home/pi/piMeter/visu/intraDay.js /var/www/html/
sudo ln -s /home/pi/piMeter/visu/realtime.js /var/www/html/
sudo ln -s /home/pi/piMeter/visu/style.css /var/www/html/
```
Change Websocket IP in intraDay.js and realtime.js to your Raspberry Pi IP

Create a file called login.h in piMeter Directory and add your MySQL/MariaDB Credentials
```
#define HOST "tcp://ip:port"
#define USER "user"
#define PASS "pass"
#define DB "dbname"
```

### Build piMeter
```
qmake
make
```

### Run piMeter
```
// bcm2835 Lib needs root
sudo ./piMeter &
```

## Ressources

[ADE9000 Datasheet](http://www.analog.com/media/en/technical-documentation/data-sheets/ADE9000.pdf)

[bcm2835 Library](http://www.airspayce.com/mikem/bcm2835/)

[lighttpd Webserver](https://www.lighttpd.net)

[amCharts Chart Library](https://www.amcharts.com)

[Qt5](https://www1.qt.io/developers/)

[www.digitaldex.de](http://www.digitaldex.de)
