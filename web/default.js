
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
           if (vals[4] == '1') $('#r1_status').text('ON'); else $('#r1_status').text('OFF');                
           if (vals[5] == '1') $('#r2_status').text('ON'); else $('#r2_status').text('OFF');                
           if (vals[6] == '1') $('#r3_status').text('ON'); else $('#r3_status').text('OFF');                
           if (vals[7] == '1') $('#r4_status').text('ON'); else $('#r4_status').text('OFF');                
         }
 

    });

    microgear.on('connected', function() {
        printMsg('Init',"Connected to NETPIE...");
        microgear.setAlias(ALIAS);
        microgear.subscribe("/data");
        microgear.subscribe("/cmd");
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
