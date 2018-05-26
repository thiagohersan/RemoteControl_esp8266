var http = require('http');

var postData = JSON.stringify({
  'type': 'SAMSUNG',
  'commands': [
    'NAV_ENTER',
    'VOLUME_UP',
    'VOLUME_DOWN',
    'NAV_UP',
    'NAV_DOWN'
  ]
});

console.log(postData);

var postOptions = {
  host: '192.168.0.107',
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
