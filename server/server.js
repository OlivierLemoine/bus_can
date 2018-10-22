const express = require('express');
const ws = require('websocket');
// const can_lib = require('./can_lib.js');

let app = express();

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/src/index.html');
});
app.use(express.static(__dirname + '/src/statics'));

app.listen(8000, () => {
    console.log('server listening on 8000');
});

