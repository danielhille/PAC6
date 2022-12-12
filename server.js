const express = require('express');
const app = express();
const mqtt = require('mqtt')
let returnedState;
app.use(express.static(__dirname + '/public'));

// start the express web server listening on 8080
app.listen(8080, () => {
    console.log('listening on 8080');
});

// serve the homepage
app.get('/', (req, res) => {
    res.sendFile('index.html');
});

app.get('/ligar', (req, res) => {
    client.publish(pubTopic, 'true', { qos: 0, retain: false }, (error) => {
        if (error) {
            console.error(error)
        }
    })
    res.sendStatus(201);
});
app.get('/desligar', (req, res) => {
    client.publish(pubTopic, 'false', { qos: 0, retain: false }, (error) => {
        if (error) {
            console.error(error)
        }
    })
    res.sendStatus(201);
});
app.get('/query', (req, res) => {

    client.publish(pubTopic, 'pinQuery', { qos: 0, retain: false }, (error) => {
        if (error) {
            console.error(error)
        }
    })
    let retorno;
    console.log(returnedState);
    setTimeout(() => {

        if (returnedState === 'on') {
            retorno = 'on';
            console.log('chegou on no backend');
        } else if (returnedState === 'off') {
            retorno = 'off';
            console.log('chegou off no backend');
        }
        else {
            res.status(500).send('timeout da query');
        }
        this.returnedState = '';
        res.status(201).send(retorno);
    }, 1000);

});

const host = 'broker.hivemq.com'
const port = '1883'
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`

const connectUrl = `mqtt://${host}:${port}`
const client = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    connectTimeout: 4000,
    //username: 'emqx',
    //password: 'public',
    reconnectPeriod: 1000,
})

const pubTopic = '/lampadapac'
const subTopic = '/lampadapac'
client.on('connect', () => {
    console.log('Connected')
    client.subscribe([subTopic], () => {
        console.log(`Subscribed to topic '${subTopic}'`)

    })
    client.publish(pubTopic, 'pinQuery', { qos: 0, retain: false }, (error) => {
        if (error) {
            console.error(error)
        }
    })
})


client.on('message', (subTopic, payload) => {
    if (payload.toString() === 'HIGH') {
        returnedState = 'on';
    } else if (payload.toString() === 'LOW') {
        returnedState = 'off';
    }
    console.log('Received Message:', subTopic, payload.toString())
})

