const express = require('express');
const WebSocket = require('ws');
const can_lib = require('./can_lib.js');

let lastestData = {
    vent: 0,
    lumiere: 0,
    distance: 0,
    pression: 0,
    quat: {
        x: 0,
        y: 0,
        z: 0,
        w: 0
    }
};

let transformTable = {
    83: 'pression',
    84: 'vent',

    85: 'lumiere',
    86: 'distance',

    87: 'quat-x',
    88: 'quat-y',
    89: 'quat-z',
    90: 'quat-w'
};

can_lib
    .init('', 10)
    .useProcessedData()
    .toInt32()
    .use(msg => {
        let id = transformTable[msg.data.id];
        if (id) {
            if (id.match(/quat/)) {
                lastestData.quat[id[id.length - 1]] = msg.int32;
            } else lastestData[id] = msg.int32;
        }
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
            can_lib.send(1, [0, 0, 0, 0, 0, 0, 0, 0]);
            console.log('capteur swaped');
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

setInterval(() => {
    // console.log(lastestData);
    ws.broadcast(lastestData);
}, 200);
