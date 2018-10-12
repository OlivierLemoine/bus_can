const express = require('express');
const mymodule = require('./build/Release/can_lib.node');

let app = express();

app.get('/', (req, res) => {
    res.send('hello world');
});

app.listen(8000);
