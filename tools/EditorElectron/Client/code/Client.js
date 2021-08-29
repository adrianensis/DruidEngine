var net = require('net');
      
var client = new net.Socket();

var writeJSON = function (json) {
  var str = JSON.stringify(json);
  client.write(JSON.stringify({__size: str.length}));
  client.write(str);
}

client.connect(8080, '127.0.0.1', function() {
  console.log('Connected');
  //client.write('Hello, server! Love, Client.');
});

client.on('data', function(data) {
  console.log('Received: ' + data);
  //client.destroy(); // kill client after server's response
});

client.on('close', function() {
  console.log('Connection closed');
});

document.getElementById("send").onclick = () => {
  console.log("send!")
  var json = {
    data: "1234"
  }
  //client.write(JSON.stringify(json).length);
  writeJSON(json);
}