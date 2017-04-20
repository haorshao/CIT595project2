Pebble.addEventListener("appmessage", 
  function(e) {
    if(e.payload){
      if(e.payload.curTemp){
        sendToServer(0);       
      }else if(e.payload.averageTemp){
        sendToServer(1);
      }else if(e.payload.min){
        sendToServer(2);
      }else if(e.payload.max){
        sendToServer(3);
      }else if(e.payload.arduino){
        arduino();
      }else if(e.payload.fah){
        temptype(5);
      }else if(e.payload.cel){
        temptype(6);
      }else if(e.payload.standby){
        features(7);
      }else if(e.payload.vibrate){
        features(8);
      }else if(e.payload.count_down){
        features(9);
      }else if(e.payload.standbyMOD){
        standbyRun();
      }else if(e.payload.standbyend){
        standbyEnd();
      }else if(e.payload.alert_20){
        alert(12);
      }else if(e.payload.alert_22){
        alert(13);
      }else if(e.payload.alert_24){
        alert(14);
      }else if(e.payload.alert_26){
        alert(15);
      }else if(e.payload.alert_28){
        alert(16);
      }else if(e.payload.alert_30){
        alert(17);
      }else if(e.payload.confirm_20){
        confirm(18);
      }else if(e.payload.confirm_22){
        confirm(19);
      }else if(e.payload.confirm_24){
        confirm(20);
      }else if(e.payload.confirm_26){
        confirm(21);
      }else if(e.payload.confirm_28){
        confirm(22);
      }else if(e.payload.confirm_30){
        confirm(23);
      }
    }
  }
);
function sendToServer(key) {
  var req = new XMLHttpRequest();
  var ipAddress = "130.91.141.96"; // Hard coded IP address
  var port = "3001"; // Same port specified as argument to server 
  var url = "http://" + ipAddress + ":" + port + "/";
  var method = "GET";
  var async = true;
  
  req.onload = function(e) {
    // see what came back
    var curTemp = "no response";
    var averageTemp = "no response";
    var minTemp = "no response";
    var maxTemp = "no response";
    var response = JSON.parse(req.responseText); 
    if (response) {
      if (response.curTemp) {
        curTemp = response.curTemp;
      }
      else curTemp = "no cur Temp";
      if(response.average) {
        averageTemp = response.average;
      }
      else averageTemp = "no average";
      if(response.min){
        minTemp = response.min;
      }
      else minTemp = "no min";
      if(response.max){
        maxTemp = response.max;
      }
      else maxTemp = "no max";
    }
    // sends message back to pebble 
    if(key == 0){
      Pebble.sendAppMessage({ "0": curTemp });  
    }else if(key == 1){
      Pebble.sendAppMessage({ "1": averageTemp });  
    }else if(key == 2){
      Pebble.sendAppMessage({ "2": minTemp });  
    }else if(key == 3){
      Pebble.sendAppMessage({ "3": maxTemp });  
    }
  };
  
  req.open(method, url, async); 
  req.send(null);
}
function arduino(){
  var req = new XMLHttpRequest();
  var ipAddress = "130.91.141.96"; // Hard coded IP address
  var port = "3001"; // Same port specified as argument to server 
  var url = "http://" + ipAddress + ":" + port + "/arduino";
  var method = "GET";
  var async = true;
  req.onload = function(e) {
    // see what came back
    var arduino = "arduino";
    Pebble.sendAppMessage({ "4": arduino });  
    
  };
  req.open(method, url, async); 
  req.send(null);
}
function temptype(key){
  var req = new XMLHttpRequest();
  var ipAddress = "130.91.141.96"; // Hard coded IP address
  var port = "3001"; // Same port specified as argument to server 
  if(key == 5){
     var url = "http://" + ipAddress + ":" + port + "/fahrenheit";
  }else if(key == 6){
     var url = "http://" + ipAddress + ":" + port + "/celsius";
  }
 
  var method = "GET";
  var async = true;
  req.onload = function(e) {
    // see what came back
    var fahrenheit = "fahrenheit";
    var celsius = "celsius";
    if(key == 5){
      Pebble.sendAppMessage({ "5": fahrenheit });    
    }else if(key == 6){
      Pebble.sendAppMessage({ "6": celsius });  
    }
  };
  req.open(method, url, async); 
  req.send(null);
}
function features(key){
    var req = new XMLHttpRequest();
    var ipAddress = "130.91.141.96"; // Hard coded IP address
    var port = "3001"; // Same port specified as argument to server 
    var url = "http://" + ipAddress + ":" + port + "/";
    var method = "GET";
    var async = true;
    req.onload = function(e) {
    // see what came back
    var standby = "Stand By MOD";
    var vibrate = "Temprature Alert";
    var count_down = "Count Down";
    if(key == 7){
      Pebble.sendAppMessage({ "7": standby });    
    }else if(key == 8){
      Pebble.sendAppMessage({ "8": vibrate });  
    }else if(key == 9){
      Pebble.sendAppMessage({ "9": count_down });  
    }
  };
  req.open(method, url, async); 
  req.send(null);
}
function standbyRun(){
    var req = new XMLHttpRequest();
    var ipAddress = "130.91.141.96"; // Hard coded IP address
    var port = "3001"; // Same port specified as argument to server 
    var url = "http://" + ipAddress + ":" + port + "/standby";
    var method = "GET";
    var async = true;
      req.onload = function(e) {
    // see what came back
    var standbyMOD = "Standing By";
    Pebble.sendAppMessage({ "10": standbyMOD });    
    };
    req.open(method, url, async); 
    req.send(null);
}
function standbyEnd(){
    var req = new XMLHttpRequest();
    var ipAddress = "130.91.141.96"; // Hard coded IP address
    var port = "3001"; // Same port specified as argument to server 
    var url = "http://" + ipAddress + ":" + port + "/endstandby";
    var method = "GET";
    var async = true;
    req.onload = function(e) {
    // see what came back
    var standbyEnd = "Standing By End";
    Pebble.sendAppMessage({ "11": standbyEnd });    
    };
    req.open(method, url, async); 
    req.send(null);
}
function alert(key){
    var req = new XMLHttpRequest();
    var ipAddress = "130.91.141.96"; // Hard coded IP address
    var port = "3001"; // Same port specified as argument to server 
    var url = "http://" + ipAddress + ":" + port + "/";
    var method = "GET";
    var async = true;
      req.onload = function(e) {
        // see what came back
        var AlertTemp = "Alert Temp" + ((key - 2) * 2);
        if(key == 12){
          Pebble.sendAppMessage({"12" : AlertTemp });      
        }else if(key == 13){
          Pebble.sendAppMessage({"13" : AlertTemp });      
        }else if(key == 14){
          Pebble.sendAppMessage({"14" : AlertTemp });      
        }else if(key == 15){
          Pebble.sendAppMessage({"15" : AlertTemp });      
        }else if(key == 16){
          Pebble.sendAppMessage({"16" : AlertTemp });      
        }else if(key == 17){
          Pebble.sendAppMessage({"17" : AlertTemp });      
        }
        
      };
      req.open(method, url, async); 
      req.send(null);
}

function confirm(key){
    var req = new XMLHttpRequest();
    var ipAddress = "130.91.141.96"; // Hard coded IP address
    var port = "3001"; // Same port specified as argument to server 
    var url = "http://" + ipAddress + ":" + port + "/" + key;
    var method = "GET";
    var async = true;
      req.onload = function(e) {
        // see what came back
        var AlertTemp = "Confirm AlertTemp:" + ((key - 8) * 2);
        if(key == 18){
          Pebble.sendAppMessage({"18" : AlertTemp });      
        }else if(key == 19){
          Pebble.sendAppMessage({"19" : AlertTemp });      
        }else if(key == 20){
          Pebble.sendAppMessage({"20" : AlertTemp });      
        }else if(key == 21){
          Pebble.sendAppMessage({"21" : AlertTemp });      
        }else if(key == 22){
          Pebble.sendAppMessage({"22" : AlertTemp });      
        }else if(key == 23){
          Pebble.sendAppMessage({"23" : AlertTemp });      
        }
        
      };
      req.open(method, url, async); 
      req.send(null);
}