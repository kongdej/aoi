
    const APPID = "AOI";
    const KEY = "2x3jdjJoQMO1kEe";
    const SECRET = "syVDI8OtKOJG71Fqmb3GB5YMj";

    const ALIAS = "htmlgear";

    var microgear = Microgear.create({
        key: KEY,
        secret: SECRET,
        alias : ALIAS
    });

    function printMsg(topic,msg) {
        var now = new Date();
        var d = now.getDay();
        var m = now.getMonth();
        m += 1;  
        var y = now.getFullYear();
        var h = now.getHours();
        var i = now.getMinutes();
        var s = now.getSeconds();
        var datetime = d + "/" + m + "/" + y + " " + h + ":" + i + ":" + s;
       document.getElementById("data").innerHTML = '&nbsp;<i class="fa fa-bell-o"></i> '+ datetime + ' # ' +topic+' <i class="fa fa-ellipsis-h"></i> ' + msg;
    }

    microgear.on('message',function(topic,msg) {
        printMsg(topic,msg);
        if (topic == "/AOI/data") {
           var vals = msg.split(",");
           console.log(vals);
           if (vals[0] == '1') $('#r1_status').text('START'); else $('#r1_status').text('STOP');                
           if (vals[1] == '1') $('#r2_status').text('OPEN'); else $('#r2_status').text('CLOSE');                
           if (vals[2] == '1') $('#r3_status').text('OPEN'); else $('#r3_status').text('CLOSE');                
           if (vals[3] == '1') $('#r4_status').text('OPEN'); else $('#r4_status').text('CLOSE'); 
            $('#volt').text(vals[11]+'V');
            $('#m1').text(vals[12]+'%');               
            $('#m2').text(vals[13]+'%');
            $('#m3').text(vals[14]+'%');

            $('#sp_1o').text(vals[4]+'%');
            $('#sp_2o').text(vals[5]+'%');
            $('#sp_3o').text(vals[6]+'%');
            $('#tono').text(vals[7]+'s');
            $('#toffo').text(vals[8]+'s');
            $('#hstarto').text(vals[9]+':00');
            $('#hstopo').text(vals[10]+':00');
            
            gM1.refresh(vals[12]);
            gM2.refresh(vals[13]);
            gM3.refresh(vals[14]);
            gM4.refresh(vals[15]);
            gT.refresh(vals[16]);
            gH.refresh(vals[17]);

         }
 

    });

    microgear.on('connected', function() {
        printMsg('Init',"Connected to NETPIE...");
        microgear.setAlias(ALIAS);
        microgear.subscribe("/data");
        microgear.subscribe("/cmd");
        microgear.subscribe("/sp");
    });

    microgear.on('present', function(event) {
//        printMsg(event.alias,event.type);
        console.log(event);
    });

    microgear.on('absent', function(event) {
//        printMsg(event.alias,event.type);
        console.log(event);
    });

    microgear.resettoken(function(err) {
        microgear.connect(APPID);
    });

    $("#r1on").click(function () {
        console.log("on");
        microgear.publish ("/cmd", "01");
    });

    $("#r1off").click(function () {
        console.log("off");
        microgear.publish ("/cmd", "00");
    });

    $("#v1on").click(function () {
        console.log("on");
        microgear.publish ("/cmd", "11");
    });

    $("#v1off").click(function () {
        console.log("off");
        microgear.publish ("/cmd", "10");
    });


    $("#v2on").click(function () {
        console.log("on");
        microgear.publish ("/cmd", "21");
    });

    $("#v2off").click(function () {
        console.log("off");
        microgear.publish ("/cmd", "20");
    });

    $("#v3on").click(function () {
        console.log("on");
        microgear.publish ("/cmd", "31");
    });

    $("#v3off").click(function () {
        console.log("off");
        microgear.publish ("/cmd", "30");
    });

    $("#sp_submit").click(function () {
        console.log($('#sp_1').val()+','+$('#sp_2').val()+','+$('#sp_3').val()+','+$('#ton').val()+','+$('#toff').val()+','+$('#hstart').val()+','+$('#hstop').val());
        microgear.publish ("/sp", $('#sp_1').val()+','+$('#sp_2').val()+','+$('#sp_3').val()+','+$('#ton').val()+','+$('#toff').val()+','+$('#hstart').val()+','+$('#hstop').val());
    });

    var gM1 = new JustGage({
        id: "gaugeM1",
        value:0,
        min: 0,
        max: 100,
        relativeGaugeSize: true,
        gaugeWidthScale: 1,
        decimals:2,
        title: "Zone A",
        label:"%",
        titlePosition: "below",
        titleFontSize: "5px",
        titleFontFamily: "Arial"
      });

    var gM2 = new JustGage({
        id: "gaugeM2",
        value:0,
        min: 0,
        max: 100,
        relativeGaugeSize: true,
        gaugeWidthScale: 1,
        decimals:2,
        title: "Zone B",
        label:"%",
        titlePosition: "below",
        titleFontSize: "5px",
        titleFontFamily: "Arial"
      });

    var gM3 = new JustGage({
        id: "gaugeM3",
        value:0,
        min: 0,
        max: 100,
        relativeGaugeSize: true,
        gaugeWidthScale: 1,
        decimals:2,
        title: "Zone C",
        label:"%",
        titlePosition: "below",
        titleFontSize: "5px",
        titleFontFamily: "Arial"
      });

    var gM4 = new JustGage({
        id: "gaugeM4",
        value:0,
        min: 0,
        max: 100,
        relativeGaugeSize: true,
        gaugeWidthScale: 1,
        decimals:2,
        title: "Zone D",
        label:"%",
        titlePosition: "below",
        titleFontSize: "5px",
        titleFontFamily: "Arial"
      });

    var gT = new JustGage({
        id: "gaugeT",
        value:0,
        min: 0,
        max: 60,
        relativeGaugeSize: true,
        gaugeWidthScale: 1,
        decimals:2,
        title: "Temperature",
        label:"C",
        titlePosition: "below",
        titleFontSize: "5px",
        titleFontFamily: "Arial"
      });

    var gH = new JustGage({
        id: "gaugeH",
        value:0,
        min: 0,
        max: 100,
        relativeGaugeSize: true,
        gaugeWidthScale: 1,
        decimals:2,
        title: "Humidity",
        label:"%",
        titlePosition: "below",
        titleFontSize: "5px",
        titleFontFamily: "Arial"
      });



