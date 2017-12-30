# piMeter
## A SmartMeter HAT for Raspberry Pi

## Frontend
![alt text](http://www.digitaldex.de/wp-content/uploads/2017/12/CurrentScreen.png "Visu Screenshot")

## Board
![alt text](http://www.digitaldex.de/wp-content/uploads/2017/12/Board.png "Eagle Board Screenshot")

## Schematics
[piMeter Schematics](http://www.digitaldex.de/wp-content/uploads/2017/12/Schematics.pdf)

## Setup Raspberry Pi

Enable SPI in raspi-config

### Install Dependencies
```
// Install libsqlite
apt-get install libsqlite3-dev
// Install lighttpd
apt-get install lighttpd
// Install php with Modules
apt-get install php7.0-fpm php7.0 php7.0-sqlite
// Install bcm2835 Library
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.52.tar.gz
tar zxvf bcm2835-1.52.tar.gz
./configure
make
sudo make check
sudo make install
```
### Install Visu
Just Copy the Visu Folder into your lighttpd www root

### Build piMeter
```
g++ main.cpp -o piMeter -lsqlite3 -lbcm2835
```

### Run piMeter
```
// bcm2835 Lib needs root
sudo ./piMeter &
```

## Ressources

[ADE9000 Datasheet](http://www.analog.com/media/en/technical-documentation/data-sheets/ADE9000.pdf)

[bcm2835 Library](http://www.airspayce.com/mikem/bcm2835/)

[libsqlite3 Library](https://github.com/LuaDist/libsqlite3)

[lighttpd Webserver](https://www.lighttpd.net)

[amCharts Chart Library](https://www.amcharts.com)

[www.digitaldex.de](http://www.digitaldex.de)
