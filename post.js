var http = require('http');

var postData = JSON.stringify({
  'pressDuration' : 300,
  'commands': [
    'VOLUME_UP',
    'VOLUME_DOWN',
    'POWER'
  ]
});

console.log(postData);

var postOptions = {
  host: '10.75.124.2',
  // host: 'rc-server.local',
  port: '80',
  path: '/',
  method: 'POST',
  headers: {
    'Content-Type': 'application/x-www-form-urlencoded',
    'Content-Length': Buffer.byteLength(postData)
  }
};

var post_req = http.request(postOptions, function(res) {
  res.setEncoding('utf8');
  res.on('data', function (chunk) {
    console.log('Response: ' + chunk);
  });
});

post_req.write(postData);
post_req.end();
