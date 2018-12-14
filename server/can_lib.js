const childProcess = require('child_process');

let child = null;

let middlewares = [];

exports.init = (dir, timeout) => {
    child = childProcess.fork('./can_read.js', [dir, timeout.toString()]);

    child.on('exit', err => {
        throw err;
    });

    child.on('message', msg => {
        for (let i = 0; i < middlewares.length; i++) {
            middlewares[i](msg);
        }
    });
    return exports;
};

exports.use = middleware => {
    middlewares.push(middleware);

    return exports;
};

exports.toInt32 = () => {
    exports.use(msg => {
        msg.int32 =
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
