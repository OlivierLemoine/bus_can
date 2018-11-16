let can_lib = {};
try {
    //@ts-ignore
    can_lib = require('./build/Release/can_lib.node');
    can_lib.initialize('');
    console.log('can_lib loaded');
} catch (error) {
    throw 'cant require can_lib';
    // can_lib.sendValues = a => {};
    // can_lib.initialize = a => {};
    // can_lib.getValue = () => {
    //     let res = [];
    //     for (let i = 0; i < 9; i++) {
    //         res.push(Math.floor(Math.random() * 255));
    //     }
    //     return res;
    // };
}

setInterval(() => {
    let msg = {};
    msg.rawData = can_lib.getValue();
    process.send(msg);
}, 10);

process.on('message', msg => {
    can_lib.sendValue(msg.id, msg.data);
});
