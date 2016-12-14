
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
    });

    microgear.on('connected', function() {
        printMsg('Init',"Connected to NETPIE...");
        microgear.setAlias(ALIAS);
        microgear.subscribe("/zone1");
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
