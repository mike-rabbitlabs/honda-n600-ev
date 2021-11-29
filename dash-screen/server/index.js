const express                  = require('express');
const path                     = require('path');
const app                      = express();
const httpServer               = require('http').Server;
const server                   = httpServer(app);
const ioServer                 = require('socket.io');
const io                       = ioServer(server);

const SERVER_PORT              = process.env.SERVER_PORT || 5000;

const SerialPort = require('serialport')
const port = new SerialPort('/dev/tty-usbserial1', {
  baudRate: 57600
})

port.on('data', function (data) {
  console.log('Data:', data)
  // port.write('Hi Mom!')
})

const sampleDataString         = "1.1, 1.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.2, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1";
io.on('connection', socket => {

});

app.get("/healthcheck", (req, res) => {
  io.emit('healthcheck');

  res.json({success: true});
})

setInterval(() => {
  let parsedData = sampleDataString.split(",");
  io.emit('bms-reading', parsedData.map(d => (parseFloat(d) + 0.3 * Math.random() )));
}, 1000);

app.use(express.static(path.join(__dirname, './build')));

server.listen(SERVER_PORT);
