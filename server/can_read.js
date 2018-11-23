let can_lib = {};
try {
    //@ts-ignore
    can_lib = require('./build/Release/can_lib.node');
    can_lib.initialize('');
    console.log('can_lib loaded');
} catch (error) {
    throw 'cant require can_lib';
}

setInterval(() => {
    let msg = {};
    msg.rawData = can_lib.getValue();
    process.send(msg);
}, 10);

process.on('message', msg => {
    can_lib.sendValue(msg.id, msg.data);
});