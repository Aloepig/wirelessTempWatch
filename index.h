const char MAIN_page[] PROGMEM = R"=====(
<!doctype html>
<html>
 
<head>
	<title> 무선 온도 모니터 </title>
  <meta charset="utf-8">

	<script src = "https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.3/Chart.min.js"></script>	
	<style>

  body{
    min-width: 400px;
  }

	canvas{
		-moz-user-select: none;
		-webkit-user-select: none;
		-ms-user-select: none;
	}
 
	/* Data Table Styling */
	#dataTable {
	  font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
	  border-collapse: collapse;
	  width: 100%;
	}
 
	#dataTable td, #dataTable th {
	  border: 1px solid #ddd;
	  padding: 8px;
	}
 
	#dataTable tr:nth-child(even){background-color: #f2f2f2;}
 
	#dataTable tr:hover {background-color: #ddd;}
 
	#dataTable th {
	  padding-top: 12px;
	  padding-bottom: 12px;
	  text-align: left;
	  background-color: #4CAF50;
	  color: white;
	}

  #divBtnClear {
    width: 150px;
    float: left;
  }

  #divInputTimeUpdate {
    width: 200px;
    float: right;
  }

  #Chart {
    height: 300px;
  }
	</style>
</head>
 
<body>
    <div style="text-align:center;"><br>현재 온도는 <h1><span id="strTemp"> ?? </span>˚C</h1></div>

    <div id="divInputTimeUpdate"> 업데이트 주기 : <input type="number" width="50" value="5" id="inputNumber" min="1" max="100000"> 초 </div>
    <div id="divBtnClear"> <input type="button" value="그래프/테이블 지우기" id="btnClear"></div> 
    <br>
    <br>
    <div class="chart-container" position: relative; height:250px; width:100%">
        <canvas id="Chart" width="400" height="400"></canvas>
    </div>
<div>
	<table id="dataTable">
	  <tr><th>시간</th><th>온도</th></tr>
	</table>
</div>
<br>
<br>	
 
<script>
//Graphs visit: https://www.chartjs.org
var values = [];
var timeStamp = [];
var timerCount = 0; 
var timeUpdate = 5;

function showGraph()
{
    for (i = 0; i < arguments.length; i++) {
    	values.push(arguments[i]);    
    }
 
    var ctx = document.getElementById("Chart").getContext('2d');
    var Chart2 = new Chart(ctx, {
        type: 'line',
        data: {
            labels: timeStamp,	//Bottom Labeling
            datasets: [{
                label: "온도",
                fill: 'origin',	//Try with true
                backgroundColor: 'rgba( 80, 176, 235 , 0.4)', //Dot marker color
                borderColor: 'rgba( 80, 176, 235 , 1)',	//Graph Line Color
                data: values,
            }],
        },
        options: {
            title: {
                    display: false,
                    text: "ADC Voltage"
                },
            maintainAspectRatio: false,
            elements: {
            line: {
                    tension: 0.5 //Smoothening (Curved) of data lines
                }
            },
            scales: {
                    yAxes: [{
                        ticks: {
                            beginAtZero:true
                        }
                    }]
            }
        }
    });
}

window.onload = function() {
	console.log(new Date().toLocaleTimeString());
  document.getElementById("btnClear").onclick = function() {
    values = [];
    timeStamp = [];
    timerCount = 0; 
    showGraph();
    var table = document.getElementById("dataTable");
    table.innerHTML = "<tr><th>시간</th><th>온도</th></tr>";
  };
};

setInterval(function() {
  getData();
}, 1000);
 
function getData() {
  timeUpdate = document.getElementById("inputNumber").value;
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
    //Push the data in array
      var ADCValue = this.responseText; 
      document.getElementById('strTemp').innerHTML = ADCValue;
      if (timerCount%timeUpdate==0 || timerCount == 1) {
        var time = new Date().toLocaleTimeString();
        values.push(ADCValue);
        timeStamp.push(time);
        showGraph();	//Update Graphs
        //Update Data Table
        var table = document.getElementById("dataTable");
        var row = table.insertRow(1);	//Add after headings
        var cell1 = row.insertCell(0);
        var cell2 = row.insertCell(1);
        cell1.innerHTML = time;
        cell2.innerHTML = ADCValue;
      }
    }
  };
  xhttp.open("GET", "readADC", true);	//Handle readADC server on ESP8266
  xhttp.send();
  timerCount++;
}
		
</script>
</body>
 
</html>
 
)=====";