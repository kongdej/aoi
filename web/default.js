
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

    first=0;
    microgear.on('message',function(topic,msg) {
        printMsg(topic,msg);
        if (topic == "/AOI/data") {
           var vals = msg.split(",");
           console.log(vals);
           if (vals[0] == '1') $('#r1_status').text('ON'); else $('#r1_status').text('OFF');                
           if (vals[1] == '1') $('#r2_status').text('ON'); else $('#r2_status').text('OFF');                
           if (vals[2] == '1') $('#r3_status').text('ON'); else $('#r3_status').text('OFF');                
           if (vals[3] == '1') $('#r4_status').text('ON'); else $('#r4_status').text('OFF'); 
            $('#m1').text(vals[10]+'%');               
            $('#m2').text(vals[11]+'%');
            $('#m3').text(vals[12]+'%');
            $('#volt').text(vals[9]+'V');
            if (first <= 1) {
                $('#sp_1').val(vals[4]);
                $('#sp_2').val(vals[5]);
                $('#sp_3').val(vals[6]);
                $('#ton').val(vals[7]);
                $('#toff').val(vals[8]);
                console.log('55');
                first++;
            }
            $('#sp_1o').text(vals[4]+'%');
            $('#sp_2o').text(vals[5]+'%');
            $('#sp_3o').text(vals[6]+'%');
            $('#tono').text(vals[7]+'s');
            $('#toffo').text(vals[8]+'s');
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
        console.log($('#sp_1').val()+','+$('#sp_2').val()+','+$('#sp_3').val()+','+$('#ton').val()+','+$('#toff').val());
        microgear.publish ("/sp", $('#sp_1').val()+','+$('#sp_2').val()+','+$('#sp_3').val()+','+$('#ton').val()+','+$('#toff').val());
    });
