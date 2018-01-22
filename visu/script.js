var chartData = [];
var wsUri = "ws://10.11.12.16:8888";
var websocket = null;
var interval = null;

var AWATTHR_HI = 0;
var BWATTHR_HI = 0;
var CWATTHR_HI = 0;
var AVARHR_HI = 0;
var BVARHR_HI = 0;
var CVARHR_HI = 0;
var AVAHR_HI = 0;
var BVAHR_HI = 0;
var CVAHR_HI = 0;
var WATTSUMM = 0;
var VARSUMM = 0;
var VASUMM = 0;

var chart = AmCharts.makeChart( "chartdiv", {
                               "type": "stock",
                               "dataDateFormat": "YYYY-MM-DD JJ:NN:SS:QQQ",
                               "addClassNames": true,
                               "glueToTheEnd": true,
                               "categoryAxesSettings": {
                                    "minPeriod": "ss",
                                    "parseDates": true,
                                    "minorGridEnabled": true
                               },
                               "dataSets": [ {
                                            "color": "#C5FFCB",
                                            "fieldMappings": [ {
                                                              "fromField": "AIRMS",
                                                              "toField": "AIRMS"
                                                              }, {
                                                              "fromField": "BIRMS",
                                                              "toField": "BIRMS"
                                                              }, {
                                                              "fromField": "CIRMS",
                                                              "toField": "CIRMS"
                                                              }, {
                                                              "fromField": "AVRMS",
                                                              "toField": "AVRMS"
                                                              }, {
                                                              "fromField": "BVRMS",
                                                              "toField": "BVRMS"
                                                              }, {
                                                              "fromField": "CVRMS",
                                                              "toField": "CVRMS"
                                                              }, {
                                                              "fromField": "AWATT",
                                                              "toField": "AWATT"
                                                              }, {
                                                              "fromField": "BWATT",
                                                              "toField": "BWATT"
                                                              }, {
                                                              "fromField": "CWATT",
                                                              "toField": "CWATT"
                                                              } ],
                                            "dataProvider": chartData,
                                            "categoryField": "Timestamp",
                                            } ],
                               
                               "panels": [ {
                                          "showCategoryAxis": false,
                                          "fontFamily": "Helvetica Neue",
                                          "fontSize": 10,
                                          "color": "#999",
                                          "title": "Strom RMS",
                                          "percentHeight": 40,
                                          "stockGraphs": [ {
                                                          "id": "g2",
                                                          "lineColor": "#60C21E",
                                                          "title": "L1",
                                                          "valueField": "AIRMS",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
                                                          "id": "g3",
                                                          "lineColor": "#159E31",
                                                          "title": "L2",
                                                          "valueField": "BIRMS",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
                                                          "id": "g4",
                                                          "lineColor": "#53DB50",
                                                          "title": "L3",
                                                          "valueField": "CIRMS",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          } ],
                                          "stockLegend": {
                                              //"valueTextRegular": " ",
                                              //"markerType": "none"
                                            },
                                          "valueAxes": [ {
                                                        "title": "Ampere",
                                                        "unit": "A"
                                                        } ],
                                          }, {
                                          "showCategoryAxis": false,
                                          "fontFamily": "Helvetica Neue",
                                          "fontSize": 10,
                                          "color": "#999",
                                          "title": "Spannung RMS",
                                          "percentHeight": 30,
                                          "stockGraphs": [ {
                                                          "id": "g5",
                                                          "lineColor": "#60C21E",
                                                          "title": "L1",
                                                          "valueField": "AVRMS",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
                                                          "id": "g6",
                                                          "lineColor": "#159E31",
                                                          "title": "L2",
                                                          "valueField": "BVRMS",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
                                                          "id": "g7",
                                                          "lineColor": "#53DB50",
                                                          "title": "L3",
                                                          "valueField": "CVRMS",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          } ],
                                          "stockLegend": {
                                          //"valueTextRegular": " ",
                                          //"markerType": "none"
                                          },
                                          "valueAxes": [ {
                                                        "title": "Volt",
                                                        "unit": "V"
                                                        } ],
                                          }, {
                                          "fontFamily": "Helvetica Neue",
                                          "fontSize": 10,
                                          "color": "#999",
                                          "title": "Watt",
                                          "percentHeight": 30,
                                          "stockGraphs": [ {
                                                          "valueField": "AWATT",
                                                          "title": "L1",
                                                          "id": "g8",
                                                          "type": "column",
                                                          "cornerRadiusTop": 2,
                                                          "lineColor": "#60C21E",
                                                          "useDataSetColors": false,
                                                          "fillAlphas": 0.2
                                                          }, {
                                                          "valueField": "BWATT",
                                                          "type": "column",
                                                          "title": "L2",
                                                          "id": "g9",
                                                          "cornerRadiusTop": 2,
                                                          "lineColor": "#159E31",
                                                          "useDataSetColors": false,
                                                          "fillAlphas": 0.2
                                                          }, {
                                                          "valueField": "CWATT",
                                                          "title": "L3",
                                                          "id": "g10",
                                                          "type": "column",
                                                          "cornerRadiusTop": 2,
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
                               "graph": "g2",
                               "usePeriod": "mm",
                               "position": "top"
                               },
                               
                               "chartCursorSettings": {
                               "valueBalloonsEnabled": true
                               },
                               "periodSelector": {
                               "fontFamily": "Helvetica Neue",
                               "fontSize": 10,
                               "color": "#999",
                               "position": "top",
                               "dateFormat": "YYYY-MM-DD JJ:NN:SS",
                               "inputFieldWidth": 150,
                               "periods": [ {
                                           "period": "mm",
                                           "count": 1,
                                           "selected": true,
                                           "label": "1 Minute"
                                           }, {
                                           "period": "mm",
                                           "count": 15,
                                           "label": "15 Minuten"
                                           }, {
                                           "period": "mm",
                                           "count": 30,
                                           "label": "30 Minuten"
                                           }, {
                                           "period": "MAX",
                                           "label": "MAX"
                                           } ]
                               },
                               "panelsSettings": {
                               "usePrefixes": true
                               }
} );

function initWebSocket() {
    try {
        if (typeof MozWebSocket == 'function')
            WebSocket = MozWebSocket;
        if ( websocket && websocket.readyState == 1 )
            websocket.close();
        websocket = new WebSocket( wsUri );
        websocket.onopen = function (evt) {
            websocket.send( "start" );
            interval = setInterval(refresh , 1000);
        };
        websocket.onclose = function (evt) {
        };
        websocket.onmessage = function (evt) {
            //console.log( "Message received :", evt.data );
            var newData = JSON.parse(evt.data);
            AWATTHR_HI = (newData[0]["AWATTHR_HI"]);
            BWATTHR_HI = (newData[0]["BWATTHR_HI"]);
            CWATTHR_HI = (newData[0]["CWATTHR_HI"]);
            AVARHR_HI = (newData[0]["AVARHR_HI"]);
            BVARHR_HI = (newData[0]["BVARHR_HI"]);
            CVARHR_HI = (newData[0]["CVARHR_HI"]);
            AVAHR_HI = (newData[0]["AVAHR_HI"]);
            BVAHR_HI = (newData[0]["BVAHR_HI"]);
            CVAHR_HI = (newData[0]["CVAHR_HI"]);
            WATTSUMM = (AWATTHR_HI + BWATTHR_HI + CWATTHR_HI) / 1000;
            VARSUMM = (AVARHR_HI + BVARHR_HI + CVARHR_HI) / 1000;
            VASUMM = (AVAHR_HI + BVAHR_HI + CVAHR_HI) / 1000;
            document.getElementById("AWATTHR").innerHTML = AWATTHR_HI + " W/h";
            document.getElementById("BWATTHR").innerHTML = BWATTHR_HI + " W/h";
            document.getElementById("CWATTHR").innerHTML = CWATTHR_HI + " W/h";
            document.getElementById("AVARHR").innerHTML = AVARHR_HI + " W/h";
            document.getElementById("BVARHR").innerHTML = BVARHR_HI + " W/h";
            document.getElementById("CVARHR").innerHTML = CVARHR_HI + " W/h";
            document.getElementById("AVAHR").innerHTML = AVAHR_HI + " W/h";
            document.getElementById("BVAHR").innerHTML = BVAHR_HI + " W/h";
            document.getElementById("CVAHR").innerHTML = CVAHR_HI + " W/h";
            document.getElementById("WATTSUMM").innerHTML = WATTSUMM.toFixed(2) + " kW/h";
            document.getElementById("VARSUMM").innerHTML = VARSUMM.toFixed(2) + " kW/h";
            document.getElementById("VASUMM").innerHTML = VASUMM.toFixed(2) + " kW/h";
            chartData.push.apply(chartData, newData);
            if (chartData.length > 3600) {
                chartData.splice(0, chartData.length - 3600);
            }
            chart.validateData(); //call to redraw the chart with new data
        };
        websocket.onerror = function (evt) {
        };
    } catch (exception) {

    }
}
function refresh() {
    	websocket.send( "start" );
}

function stopWebSocket() {
    clearInterval(interval);
    if (websocket)
        websocket.close();
}
