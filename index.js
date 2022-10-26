// express framwork for a basic http server
var app = require('express')();
// create the http server
var http = require('http').createServer(app);
// require the socket.io and bind it to a port 
var io = require('socket.io')(3005);


app.listen(3000, function () {
    console.log('Example app listening on port 3000.');
});
app.get('/', function (req, res) {
    res.send('<b>Hello</b> welcome to my http server made with express');
});


io.attach(http, {
    pingInterval: 10000,
    pingTimeout: 5000,
    cookie: false
});
io.on('connection', function (socket) {

    console.log('user connected');

    socket.on('disconnect', function () {
        console.log('user disconnected');
    });
    socket.on('message', function (msg) {
        console.log("message: " + msg);
    });
    timeout();
});

function timeout() {
    setTimeout(function () {
        io.emit('reply', "A message from server");
        timeout();
    }, 5000);
}
http.listen();