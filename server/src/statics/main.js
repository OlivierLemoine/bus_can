let values_placeholders = {
    pression: document.getElementById('pression_value'),
    vent: document.getElementById('vent_value'),
    lumiere: document.getElementById('lumiere_value'),
    distance: document.getElementById('distance_value')
};

let ws = new WebSocket('ws://127.0.0.1:8001');

ws.onmessage = event => {
    let data = JSON.parse(event.data);
    switch (data.type) {
        case 'value':
            values_placeholders[data.id].textContent = data.value;
            break;
    
        default:
            break;
    }
};

function swapCapteur() {
    ws.send('swap');
}
