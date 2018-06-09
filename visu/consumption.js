var chartData = [];
var newData = [];
var wsUri = "ws://10.11.12.130:8888";
var websocket = null;
var awatttotal = 0;
var bwatttotal = 0;
var cwatttotal = 0;
var wattsummtotal = 0;
var firstStart = true;

var today = new Date();
var dd = today.getDate();
var mm = today.getMonth()+1; 
var yyyy = today.getFullYear();
if(dd<10) 
{
    dd='0'+dd;
} 

if(mm<10) 
{
    mm='0'+mm;
} 
today = yyyy+'-'+mm+'-'+dd;

var chart = AmCharts.makeChart( "chartdiv", {
                               "type": "stock",
                               "dataDateFormat": "YYYY-MM-DD JJ:NN:SS",
                               "addClassNames": true,
                               "glueToTheEnd": true,
                               "categoryAxesSettings": {
                                    "minPeriod": "hh",
                                    "parseDates": true,
                                    "minorGridEnabled": true
                               },
                               "dataSets": [ {
                                            "color": "#C5FFCB",
                                            "fieldMappings": [ {
                                                              "fromField": "AWATTHR",
                                                              "toField": "AWATTHR"
                                                              }, {
                                                              "fromField": "BWATTHR",
                                                              "toField": "BWATTHR"
                                                              }, {
                                                              "fromField": "CWATTHR",
                                                              "toField": "CWATTHR"
                                                              } ],
                                            "dataProvider": chartData,
                                            "categoryField": "Timestamp",
                                            } ],
                               
                               "panels": [ {
                                          "fontFamily": "Helvetica Neue",
                                          "fontSize": 10,
                                          "color": "#999",
                                          "title": "Watt",
                                          "percentHeight": 100,
                                          "stockGraphs": [ {
                                                          "valueField": "AWATTHR",
                                                          "title": "L1",
                                                          "id": "g1",
                                                          "type": "column",
                                                          "cornerRadiusTop": 0,
                                                          "lineColor": "#60C21E",
                                                          "useDataSetColors": false,
                                                          "fillAlphas": 0.2
                                                          }, {
                                                          "valueField": "BWATTHR",
                                                          "type": "column",
                                                          "title": "L2",
                                                          "id": "g2",
                                                          "cornerRadiusTop": 0,
                                                          "lineColor": "#159E31",
                                                          "useDataSetColors": false,
                                                          "fillAlphas": 0.2
                                                          }, {
                                                          "valueField": "CWATTHR",
                                                          "title": "L3",
                                                          "id": "g3",
                                                          "type": "column",
                                                          "cornerRadiusTop": 0,
                                                          "lineColor": "#53DB50",
                                                          "useDataSetColors": false,
                                                          "fillAlphas": 0.2
                                                          } ],
                                          
                                          "stockLegend": {
                                              //"valueTextRegular": " ",
                                              //"markerType": "none"
                                          },
                                          "valueAxes": [ {
                                                        "title": "Watt",
                                                        "unit": "W",
                                                        "stackType": "regular"
                                                        } ],
                                          } ],
                               
                               "chartScrollbarSettings": {
                               "fontFamily": "Helvetica Neue",
                               "fontSize": 8,
                               "color": "#424242",
                               "backgroundColor": "#FFFFFF",
                               "graphFillColor": "#A4A4A4",
                               "graphLineColor": "#000000",
                               "selectedBackgroundColor": "#FFFFFF",
                               "selectedGraphFillColor": "#A4A4A4",
                               "graph": "g1",
                               "usePeriod": "DD",
                               "position": "top"                            
                               },
                               "chartCursorSettings": {
                               "valueBalloonsEnabled": true
                               },
                               "panelsSettings": {
                               "usePrefixes": true
                               }
} );

initWebSocket();

function initWebSocket() {
    try {
        if (typeof MozWebSocket == 'function')
            WebSocket = MozWebSocket;
        if ( websocket && websocket.readyState == 1 )
            websocket.close();
        websocket = new WebSocket( wsUri );
        websocket.onopen = function (evt) {
            websocket.send("init");
        };
        websocket.onclose = function (evt) {
        };
        websocket.onmessage = function (evt) {
            // console.log( "Message received :", evt.data );
            newData = JSON.parse(evt.data);
            if(firstStart == true) {
              awatttotal = (newData[0]["TotalL1watt"]) / 1000;
              bwatttotal = (newData[0]["TotalL2watt"]) / 1000;
              cwatttotal = (newData[0]["TotalL3watt"]) / 1000;
              wattsummtotal = (awatttotal + bwatttotal + cwatttotal);
              document.getElementById("TotalAwatt").innerHTML = awatttotal.toFixed(2) + "kW/h";
              document.getElementById("TotalBwatt").innerHTML = bwatttotal.toFixed(2) + "kW/h";
              document.getElementById("TotalCwatt").innerHTML = cwatttotal.toFixed(2) + "kW/h";
              document.getElementById("TotalWatt").innerHTML = wattsummtotal.toFixed(2) + "kW/h";
              firstStart = false;
              websocket.send(today);
            }

            chartData.length = 0;
            chartData.push.apply(chartData, newData);
            document.getElementById("actualDate").innerHTML = today;
            chart.validateData(); //call to redraw the chart with new data
        };
        websocket.onerror = function (evt) {
        };
    } catch (exception) {

    }
}

function getDay() {
  today = document.getElementById("date").value;
  websocket.send(today);
}

function getMonth() {
    today = document.getElementById("month").value;
    websocket.send(today);
}

function getYear() {
    today = document.getElementById("year").value;
    websocket.send(today);
}
