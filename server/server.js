const express = require('express');
const WebSocket = require('ws');
const can_lib = require('./can_lib.js');

let lastestData = {};

let transformTable = {
    1: 'pression',
    2: 'vent',
    3: 'lumiere',
    4: 'distance'
};

can_lib
    .setMockData()
    .init('test', 200)
    .useProcessedData()
    .use(msg => {
        lastestData[msg.data.id] = msg.data.values;
    })
    .use(msg => {
        msg.payload = {
            type: 'value',
            id: transformTable[msg.data.id],
            value: msg.data.values
        };
    })
    .use(msg => {
        ws.broadcast(msg.payload);
    });

let app = express();

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/src/index.html');
});
app.use(express.static(__dirname + '/src/statics'));

app.listen(8000, () => {
    console.log('server listening on 8000');
});

var ws = new WebSocket.Server({ port: 8001 });
ws.on('connection', socket => {
    socket.on('message', data => {
        if (data === 'swap') {
            console.log('capteur swaped');
            socket.send(
                JSON.stringify({
                    type: 'value',
                    id: 'pression',
                    value: 10
                })
            );
            //swap capteur
        }
    });

    socket.on('close', () => {});
});
ws.broadcast = function(data) {
    if (typeof data !== 'string') data = JSON.stringify(data);
    ws.clients.forEach(function each(client) {
        if (client.readyState === WebSocket.OPEN) {
            client.send(data);
        }
    });
};
