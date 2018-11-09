// import './plane.js';
// import Gauge from './gauge.js';

let gauges = {
    pression: new Gauge({ opening: 270, thickness: 20 }, { min: 0, max: 4 }),
    vent: new Gauge({ opening: 180, thickness: 20 }, { min: 0, max: 4 }),
    lumiere: new Gauge({ opening: 90, thickness: 20 }, { min: 0, max: 4 }),
    distance: new Gauge({ opening: 45, thickness: 20 }, { min: 0, max: 4 })
};

let ws = new WebSocket('ws://127.0.0.1:8001');

ws.onmessage = event => {
    let data = JSON.parse(event.data);
    switch (data.type) {
        case 'value':
            gauges[data.id].updateValue(data.value);
            break;

        default:
            break;
    }
};

document.getElementById('swap_capteur').addEventListener('click', () => {
    ws.send('swap');
});
