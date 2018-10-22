const can_lib = require('./build/Release/can_lib.node');

let isInit = false;

exports.init = (busCanPath, callback, intervals = 1000) => {
    can_lib.initialize(busCanPath);

    isInit = true;

    setInterval(() => {
        let rawData = can_lib.getValue();
        let data = {id : rawData, values : []};
        for (let i = 0; i < rawData.length - 1; i++) {
            data.values[i] = rawData[i+1];
        }
        callback(data);
    }, intervals);
};

exports.send = data => {
    if (!isInit) throw 'CanLib not initialize !';

    can_lib.sendValues(data);
};
