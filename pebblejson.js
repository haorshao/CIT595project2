Pebble.addEventListener("appmessage", 
  function(e) {
    sendToServer(); }
);
function sendToServer() {
  
  var req = new XMLHttpRequest();
  var ipAddress = "10.0.0.22"; // Hard coded IP address
  var port = "3001"; // Same port specified as argument to server 
  var url = "http://" + ipAddress + ":" + port + "/";
  var method = "GET";
  var async = true;
  
  req.onload = function(e) {
    // see what came back
    var curTemp = "no response";
    var averageTemp = "no response";
    var response = JSON.parse(req.responseText); 
    if (response) {
      if (response.name) {
        curTemp = response.name;
      }
      else curTemp = "noname";
      if(response.average) {
        averageTemp = response.average;
      }
      else averageTemp = "no average";
    }
//     msg = "pineapple";
    // sends message back to pebble 
    Pebble.sendAppMessage({ "0": curTemp });
    Pebble.sendAppMessage({ "1": averageTemp });
  };
  
  req.open(method, url, async); 
  req.send(null);
}