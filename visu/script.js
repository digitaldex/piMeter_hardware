var chart = AmCharts.makeChart( "chartdiv", {
                               "type": "stock",
                               "dataDateFormat": "YYYY-MM-DD JJ:NN:SS:QQQ",
                               "addClassNames": true,
                               "categoryAxesSettings": {
                                   "minPeriod": "fff",
                                   "parseDates": true,
                                   "minorGridEnabled": true
                               },
                               "dataSets": [ {
                                            "color": "#C5FFCB",
                                            "fieldMappings": [ {
                                                              "fromField": "ISUMRMS",
                                                              "toField": "ISUMRMS"
                                                              }, {
                                                              "fromField": "AIRMS",
                                                              "toField": "AIRMS"
                                                              }, {
                                                              "fromField": "BIRMS",
                                                              "toField": "BIRMS"
                                                              }, {
                                                              "fromField": "CIRMS",
                                                              "toField": "CIRMS"
                                                              }, {
                                                              "fromField": "AI_PCF",
                                                              "toField": "AI_PCF"
                                                              }, {
                                                              "fromField": "BI_PCF",
                                                              "toField": "BI_PCF"
                                                              }, {
                                                              "fromField": "CI_PCF",
                                                              "toField": "CI_PCF"
                                                              }, {
                                                              "fromField": "PWATT_ACC",
                                                              "toField": "PWATT_ACC"
                                                              } ],
                                            "dataLoader": {
                                                "url": "dataCurrent.php",
                                                "format": "json",
                                                "async": true
                                                },
                                            "categoryField": "Timestamp",
                                            } ],
                               
                               "panels": [ {
                                          "showCategoryAxis": false,
                                          "fontFamily": "Helvetica Neue",
                                          "fontSize": 10,
                                          "color": "#999",
                                          "title": "Strom RMS",
                                          "percentHeight": 50,
                                          "stockGraphs": [ {
                                                          "id": "g1",
                                                          "lineColor": "#A5EB3C",
                                                          "title": "Gesamt",
                                                          "valueField": "ISUMRMS",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 2,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
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
                                          "title": "Strom tatsachlich",
                                          "percentHeight": 30,
                                          "stockGraphs": [ {
                                                          "id": "g5",
                                                          "lineColor": "#60C21E",
                                                          "title": "L1",
                                                          "valueField": "AI_PCF",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
                                                          "id": "g6",
                                                          "lineColor": "#159E31",
                                                          "title": "L2",
                                                          "valueField": "BI_PCF",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
                                                          "id": "g7",
                                                          "lineColor": "#53DB50",
                                                          "title": "L3",
                                                          "valueField": "CI_PCF",
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
                                          "fontFamily": "Helvetica Neue",
                                          "fontSize": 10,
                                          "color": "#999",
                                          "title": "Watt",
                                          "percentHeight": 20,
                                          "stockGraphs": [ {
                                                          "valueField": "PWATT_ACC",
                                                          "type": "column",
                                                          "cornerRadiusTop": 2,
                                                          "fillAlphas": 1
                                                          } ],
                                          
                                          "stockLegend": {
                                              "valueTextRegular": " ",
                                              "markerType": "none"
                                          },
                                          "valueAxes": [ {
                                                        "title": "Watt",
                                                        "unit": "W"
                                                        } ],
                                          } ],
                               
                               "chartScrollbarSettings": {
                               "fontFamily": "Helvetica Neue",
                               "fontSize": 8,
                               "color": "#999",
                               "graph": "g1",
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
                                           "count": 30,
                                           "label": "30 Minuten"
                                           }, {
                                           "period": "hh",
                                           "count": 1,
                                           "label": "1 Stunde"
                                           } ]
                               },
                               
                               "panelsSettings": {
                               "usePrefixes": true
                               }
} );


var chart = AmCharts.makeChart( "chartdiv2", {
                               "type": "stock",
                               "dataDateFormat": "YYYY-MM-DD JJ:NN:SS:QQQ",
                               "addClassNames": true,
                               "categoryAxesSettings": {
                                   "minPeriod": "fff",
                                   "parseDates": true,
                                   "minorGridEnabled": true
                               },
                               "dataSets": [ {
                                            "color": "#C5FFCB",
                                            "fieldMappings": [ {
                                                              "fromField": "AVRMS",
                                                              "toField": "AVRMS"
                                                              }, {
                                                              "fromField": "BVRMS",
                                                              "toField": "BVRMS"
                                                              }, {
                                                              "fromField": "CVRMS",
                                                              "toField": "CVRMS"
                                                              }, {
                                                              "fromField": "AV_PCF",
                                                              "toField": "AV_PCF"
                                                              }, {
                                                              "fromField": "BV_PCF",
                                                              "toField": "BV_PCF"
                                                              }, {
                                                              "fromField": "CV_PCF",
                                                              "toField": "CV_PCF"
                                                              } ],
                                            "dataLoader": {
                                            "url": "dataVoltage.php",
                                            "format": "json",
                                            "async": true
                                            },
                                            "categoryField": "Timestamp",
                                            } ],
                               
                               "panels": [ {
                                          "showCategoryAxis": false,
                                          "fontFamily": "Helvetica Neue",
                                          "fontSize": 10,
                                          "color": "#999",
                                          "title": "Spannung RMS",
                                          "percentHeight": 50,
                                          "stockGraphs": [ {
                                                          "id": "g1",
                                                          "lineColor": "#60C21E",
                                                          "title": "L1",
                                                          "valueField": "AVRMS",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
                                                          "id": "g2",
                                                          "lineColor": "#159E31",
                                                          "title": "L2",
                                                          "valueField": "BVRMS",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
                                                          "id": "g3",
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
                                          "showCategoryAxis": false,
                                          "fontFamily": "Helvetica Neue",
                                          "fontSize": 10,
                                          "color": "#999",
                                          "title": "Spannung tatsachlich",
                                          "percentHeight": 50,
                                          "stockGraphs": [ {
                                                          "id": "g4",
                                                          "lineColor": "#60C21E",
                                                          "title": "L1",
                                                          "valueField": "AV_PCF",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
                                                          "id": "g5",
                                                          "lineColor": "#159E31",
                                                          "title": "L2",
                                                          "valueField": "BV_PCF",
                                                          "type": "smoothedLine",
                                                          "lineThickness": 1,
                                                          "useDataSetColors": false,
                                                          "connect": false
                                                          }, {
                                                          "id": "g6",
                                                          "lineColor": "#53DB50",
                                                          "title": "L3",
                                                          "valueField": "CV_PCF",
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
                                          } ],
                               
                               "chartScrollbarSettings": {
                               "fontFamily": "Helvetica Neue",
                               "fontSize": 8,
                               "color": "#999",
                               "graph": "g1",
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
                                           "count": 30,
                                           "label": "30 Minuten"
                                           }, {
                                           "period": "hh",
                                           "count": 1,
                                           "label": "1 Stunde"
                                           } ]
                               },
                               
                               "panelsSettings": {
                               "usePrefixes": true
                               }
} );
