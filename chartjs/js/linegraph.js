$(document).ready(function(){

    $.ajax({
        url: "https://ascraplive.000webhostapp.com/chartjs/data1.php",
        type: "GET",
        success : function(data){
            console.log(data);
            var id = [];
            var beat =[];
            var oxy = [];
            var temp =[];
            var time_1 = [];

            for (var i in data)
            {
                id.push("time_1" + data[i].id);   
                beat.push(data[i].beat);  
                oxy.push(data[i].oxy);  
                temp.push(data[i].temp);  
                time_1.push(data[i].time_1);  
            }

            var chartdata ={
                labels : time_1,
                datasets: [
                    {
                      label: "Temperature",
                      fill: false,
                      lineTension: 0.1,
                      backgroundColor: "rgba(59, 89, 152, 0.75)",
                      borderColor: "rgba(59, 89, 152, 1)",
                      pointHoverBackgroundColor: "rgba(59, 89, 152, 1)",
                      pointHoverBorderColor: "rgba(59, 89, 152, 1)",
                      data: temp
                    },
                    {
                      label: "BPM",
                      fill: false,
                      lineTension: 0.1,
                      backgroundColor: "rgba(29, 202, 255, 0.75)",
                      borderColor: "rgba(29, 202, 255, 1)",
                      pointHoverBackgroundColor: "rgba(29, 202, 255, 1)",
                      pointHoverBorderColor: "rgba(29, 202, 255, 1)",
                      data: beat
                    },
                    {
                      label: "SPO2",
                      fill: false,
                      lineTension: 0.1,
                      backgroundColor: "rgba(211, 72, 54, 0.75)",
                      borderColor: "rgba(211, 72, 54, 1)",
                      pointHoverBackgroundColor: "rgba(211, 72, 54, 1)",
                      pointHoverBorderColor: "rgba(211, 72, 54, 1)",
                      data: oxy
                    }
                  ]
            };

            var ctx = $("#mycanvas");
            var LineGraph = new Chart(ctx,{
                type: 'line',   
                data:  chartdata
                
                
            });
        },
        error : function(data)
        {

        }
    });

});