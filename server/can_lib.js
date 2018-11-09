// @ts-ignore
let can_lib = {};
try {
    can_lib = require('./build/Release/can_lib.node');
    console.log('can_lib loaded');
} catch (error) {
    throw 'cant require can_lib';
    can_lib.sendValues = a => {};
    can_lib.initialize = a => {};
    can_lib.getValue = () => {
        let res = [];
        for (let i = 0; i < 9; i++) {
            res.push(Math.floor(Math.random() * 255));
        }
        return res;
    };
}

let isInit = false;

let middlewares = [];

exports.init = (busCanPath, intervals = 1000) => {
    can_lib.initialize(busCanPath);

    isInit = true;

    setInterval(() => {
        let msg = {};
        msg.rawData = can_lib.getValue();

        for (let i = 0; i < middlewares.length; i++) {
            middlewares[i](msg);
        }
    }, intervals);

    return exports;
};

exports.use = middleware => {
    middlewares.push(middleware);

    return exports;
};

exports.toInt32 = () => {
    exports.use(msg => {
        msg.int32 = { id: msg.data.id };
        msg.int32.value =
            (msg.data.values[0] << 28) |
            (msg.data.values[1] << 24) |
            (msg.data.values[2] << 20) |
            (msg.data.values[3] << 16) |
            (msg.data.values[4] << 12) |
            (msg.data.values[5] << 08) |
            (msg.data.values[6] << 4) |
            (msg.data.values[7] << 0);
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

exports.send = data => {
    if (!isInit) throw 'CanLib not initialize !';

    can_lib.sendValue(data);

    return exports;
};

exports.setMockData = () => {
    can_lib.sendValues = () => {};
    can_lib.initialize = () => {};
    can_lib.getValue = () => {
        let res = [];
        for (let i = 0; i < 2; i++) {
            res.push(Math.floor(Math.random() * 4) + 1);
        }
        return res;
    };

    return exports;
};
