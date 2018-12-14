const can_lib = require('./build/Release/can_lib.node');
can_lib.initialize(process.argv[1], parseInt(process.argv[2]));

setInterval(() => {
    let msg = {};
    msg.rawData = can_lib.getValue();
    process.send(msg);
}, parseInt(process.argv[2]));

process.on('message', msg => {
    can_lib.sendValue(msg.id, msg.data);
});
