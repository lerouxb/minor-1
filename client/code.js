'use strict';

import { parseStatus, parseSetups, formatSetups } from './QT60240.mjs';


const KEY_NAMES = [
    // TODO: why are the bytes backwards? Because I swapped the line numbers in the schematic..
    'c#',
    'd#',
    'f',
    'g',
    'a',
    'b',
    'k1',
    'k3',

    'c',
    'd',
    'e',
    'f#',
    'g#',
    'a#',
    'c2',
    'k2'
];


let setups; // readSetups to load this

let port;
let reader;
let inputDone;
let outputDone;
let inputStream;
let outputStream;

async function connect () {
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: 57600 });

    document.body.classList.add('connected');

    const decoder = new TextDecoderStream();
    inputDone = port.readable.pipeTo(decoder.writable);
    inputStream = decoder.readable.pipeThrough(new TransformStream(new LineBreakTransformer()));

    const encoder = new TextEncoderStream();
    outputDone = encoder.readable.pipeTo(port.writable);
    outputStream = encoder.writable;

    reader = inputStream.getReader();

    readLoop(); // NOTE: deliberately not awaiting this
}

async function readLoop () {

    while (true) {
        try {
            var { value, done } = await reader.read();
        }
        catch (err) {
            // not sure if there's a better way to disconnect so we don't get this error
            if (err.message.includes('This readable stream reader has been released and cannot be used to read from its previous owner stream')) {
                console.log('Disconnected.');
                return;
            }

            throw err;
        }

        if (value) {
            handleLine(value);
        }

        if (done) {
            console.log('DONE');
            reader.releaseLock();
        }
    }
}

async function disconnect () {

    document.body.classList.remove('connected');

    if (reader) {
        await reader.cancel();
        await inputDone.catch(() => {});
        reader = null;
        inputDone = null;
    }

    if (outputStream) {
        await outputStream.getWriter().close();
        await outputDone;
        outputStream = null;
        outputDone = null;
    }

    await port.close();
    port = null;
}

function readStatus () {

    const writer = outputStream.getWriter();
    writer.write('RSTATUS\r');
    writer.releaseLock();
}

function readSetups () {

    const writer = outputStream.getWriter();
    writer.write('RSETUPS\r');
    writer.releaseLock();
}

function writeSetups () {

    if (!setups) {
        console.log('no setups yet :(');
        return;
    }

    // modify setups to make the modifications we want
    for (let i = 0; i < 24; ++i) {
        setups.AKS[i] = 0; // default 0
        setups.burstLength[i] = 0; // default 2
        setups.negThreshold[i] = 15; // recommended 3 to 8, default 6. can do 0 to 15
    }
    setups.driftHoldTimeout = 10; // the default is 10 meaning 1 second

    const writer = outputStream.getWriter();
    writer.write('WSETUPS\r');
    writer.write(`${formatSetups(setups)}\r`);
    writer.releaseLock();
};

function handleLine (line) {

    console.log(line);

    if (line.startsWith('KEYS ')) {
        console.log(JSON.stringify(pressedKeys(parseKeys(line.slice(5)))));
        return;
    }

    if (line.startsWith('STATUS ')) {
        console.log(parseStatus(line.slice(7)));
        return;
    }

    if (line.startsWith('SETUPS ')) {
        setups = parseSetups(line.slice(7));
        console.log(setups);
        return;
    }
}

function parseKeys (text) {

    const parts = text.split(' ');

    const keys = {};

    for (let i = 0; i < 2; ++i) {
        for (let j = 0; j < 8; ++j) {
            const index = i * 8 + j;
            const number = parseInt(parts[i], 10);

            keys[KEY_NAMES[index]] = !!(number & 1 << j) ? 1 : 0
        }
    }

    return keys;
}

function pressedKeys (keys) {

    const pressed = [];
    for (const [k, v] of Object.entries(keys)) {
        if (v) {
            pressed.push(k);
        }
    }
    return pressed;
}

class LineBreakTransformer {
  constructor() {
    // A container for holding stream data until a new line.
    this.container = '';
  }

  transform(chunk, controller) {

    this.container += chunk;
    const lines = this.container.split('\r\n');
    this.container = lines.pop();
    lines.forEach(line => controller.enqueue(line));
  }

  flush(controller) {
    controller.enqueue(this.container);
  }
}

window.addEventListener('DOMContentLoaded', (event) => {
    document.querySelector('#connect').addEventListener('click', connect);
    document.querySelector('#disconnect').addEventListener('click', disconnect);
    document.querySelector('#readStatus').addEventListener('click', readStatus);
    document.querySelector('#readSetups').addEventListener('click', readSetups);
    document.querySelector('#writeSetups').addEventListener('click', writeSetups);
});
