const childProcess = require('child_process');

let child = childProcess.fork('./can_read.js');

child.on('message', msg => {
    for (let i = 0; i < middlewares.length; i++) {
        middlewares[i](msg);
    }
});

let middlewares = [];

exports.use = middleware => {
    middlewares.push(middleware);

    return exports;
};

exports.toInt32 = () => {
    exports.use(msg => {
        msg.int32 = { id: msg.data.id };
        msg.int32.value =
            (msg.data.values[0] << 24) |
            (msg.data.values[1] << 16) |
            (msg.data.values[2] << 8) |
            (msg.data.values[3] << 0);
    });

    return exports;
};

exports.useProcessedData = () => {
    exports.use(msg => {
        msg.data = { id: msg.rawData[0], values: [] };
        for (let i = 0; i < msg.rawData.length - 1; i++) {
            msg.data.values[i] = msg.rawData[i + 1];
        }
    });

    return exports;
};

exports.send = (id, data) => {
    child.send({ id: id, data: data });
};
