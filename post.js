var http = require('http');

var postData = JSON.stringify({
  'commands': [
    'VOLUME_UP',
    'CHANNEL_DOWN',
    'VOLUME_UP',
    'VOLUME_DOWN',
    'CHANNEL_UP',
    'CHANNEL_UP',
    'CHANNEL_DOWN',
    'CHANNEL_UP',
    'CHANNEL_UP',
    'VOLUME_DOWN',
    'CHANNEL_DOWN',
    'CHANNEL_DOWN'
  ]
});

console.log(postData);

var postOptions = {
  host: '10.75.124.74',
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
