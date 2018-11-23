// import './plane.js';
// import Gauge from './gauge.js';

let gauges = {
    pression: new Gauge({ opening: 360, thickness: 20 }, { min: 0, max: 1000 }),
    vent: new Gauge({ opening: 360, thickness: 20 }, { min: 0, max: 1000 }),
    lumiere: new Gauge({ opening: 360, thickness: 20 }, { min: 0, max: 1000 }),
    distance: new Gauge({ opening: 360, thickness: 20 }, { min: 0, max: 200 })
};

let ws = new WebSocket('ws://127.0.0.1:8001');

ws.onmessage = event => {
    let data = JSON.parse(event.data);
    for (const i in data) 
        if (gauges[i]) gauges[i].updateValue(data[i]);
    // switch (data.type) {
    //     case 'value':
    //         gauges[data.id].updateValue(data.value);
    //         break;

    //     default:
    //         break;
    // }
};

document.getElementById('swap_capteur').addEventListener('click', () => {
    ws.send('swap');
});
