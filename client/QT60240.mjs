'use strict';

//const example = '00 00 FE 00 00 00 00 00 20 9F 01 A0 01 00 07 03 09 03 00 B3 02 B3 02 00 B0 02 B0 02 00 E5 02 E5 02 00 44 01 43 01 00 18 01 17 01 00 00 00 00 00 00 4A 03 7D 03 12 C9 02 1F 03 12 45 01 54 01 12 3E 01 4D 01 12 4C 01 5A 01 12 A4 03 CF 03 12 E3 00 ED 00 12 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 00 00 00 00 00 00 00 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F BB 19 0A';

const statusExample = '00 FE 00 00 00 00 00 20 9F 01 A0 01 00 07 03 09 03 00 B3 02 B3 02 00 B0 02 B0 02 00 E5 02 E5 02 00 44 01 43 01 00 18 01 17 01 00 00 00 00 00 00 4A 03 7D 03 12 C9 02 1F 03 12 45 01 54 01 12 3E 01 4D 01 12 4C 01 5A 01 12 A4 03 CF 03 12 E3 00 ED 00 12 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00 04 00 04 00 00';

const setupsExample = 'A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 A6 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 4F 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 52 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 14 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F BB 19 0A';

function parse16Bits (buffer, offset) {

    const lsb = buffer[offset];
    const msb = buffer[offset + 1];

    return (msb << 4) + lsb;
}

function lowerNibble (byte) {

    return byte & 15;
}

function upperNibble (byte) {

    return byte >> 4;
}

function bits (byte, names) {

    const obj = {};

    for (const [index, name] of names.entries()) {
        obj[name.toString()] = !!(byte & 1 << index);
    }

    return obj;
}

function bit (byte, index) {

    return (byte >> index) & 1;
}

function bitsToByte (number, offset) {

    return number << offset;
}

function combineBits (numbers) {

    let result = 0;

    for (const number of numbers) {
        result = result | number;
    }

    return result;
}

function combineNibbles (lower, upper) {

    return combineBits([bitsToByte(upper, 4), lower]);
}

function bitRange (byte, start, end) {

    // this is crazy :)
    return (byte >> start) & (Math.pow(2, end - start + 1) - 1);
}

export function parseStatus (line) {

    // byte 0 is reserved and line already skips it and only contains bytes 1 to 123.
    // 1: keys 0-7
    // 2: keys 8-15
    // 3: keys 16-23
    // 4 to 123: key data

    const buffer = parseLine(line);

    const status_0_7 = bits(buffer[0], [0, 1, 2, 3, 4, 5, 6, 7]);
    const status_8_15 = bits(buffer[1], [8, 9, 10, 11, 12, 13, 14, 15]);
    const status_16_23 = bits(buffer[2], [16, 17, 18, 19, 20, 21, 22, 23]);
    const status = Object.assign({}, status_0_7, status_8_15, status_16_23);

    const keys = [];

    for (let i = 0; i<24; ++i) {
        // 5 bytes per key:

        // Signal LSB
        // Signal MSB
        // Reference LSB
        // Reference MSB
        // Detect count (lower nibble)

        keys.push({
            signal: parse16Bits(buffer, 3 + i*5),
            reference: parse16Bits(buffer, 5 + i*5),
            detect: lowerNibble(buffer[7 + i*5])
        });
    }

    return { status, keys };
}

export function parseSetups (line) {

    // setups are bytes 131 to 253, but the line already only includes those so 131 becomes 0 and so on

    const buffer = parseLine(line);

    const negThreshold = [];
    const negDriftComp = [];
    const posDriftComp = [];
    const normalDILimit = [];
    const fastDILimit = [];
    const negRecalDelay = [];
    const wakeOnTouch = [];
    const burstLength = [];
    const AKS = [];
    const scopeSync = [];

    for (let i = 0; i<24; ++i) {

        negThreshold.push(lowerNibble(buffer[0 + i]));
        negDriftComp.push(upperNibble(buffer[0 + i]));
        posDriftComp.push(upperNibble(buffer[24 + i])); // lower unused
        normalDILimit.push(lowerNibble(buffer[48 + i]));
        fastDILimit.push(upperNibble(buffer[48 + i]));
        negRecalDelay.push(buffer[72 + i]);
        wakeOnTouch.push(bit(buffer[96 + i], 3));
        burstLength.push(bitRange(buffer[96 + i], 4, 5));
        AKS.push(bit(buffer[96 + i], 6));
        scopeSync.push(bit(buffer[96 + i], 7));
    }

    return {
        negThreshold,
        negDriftComp,
        posDriftComp,
        normalDILimit,
        fastDILimit,
        negRecalDelay,
        wakeOnTouch,
        burstLength,
        AKS,
        scopeSync,

        sleepDuration: bitRange(buffer[120], 0, 2),
        mainsSync: bit(buffer[120], 6),
        awakeTimeout: buffer[121],
        driftHoldTimeout: buffer[122]
    };
}

export function formatSetups (setups) {

    const bytes = [];

    for (let i = 0; i<24; ++i) {
        bytes.push(combineNibbles(setups.negThreshold[i], setups.negDriftComp[i]));
    }

    for (let i = 0; i<24; ++i) {
        bytes.push(combineNibbles(15, setups.posDriftComp[i])); // set all unused lower nibble bits to 1
    }

    for (let i = 0; i<24; ++i) {
        bytes.push(combineNibbles(setups.normalDILimit[i], setups.fastDILimit[i]));
    }

    for (let i = 0; i<24; ++i) {
        bytes.push(setups.negRecalDelay[i]);
    }

    for (let i = 0; i<24; ++i) {
        bytes.push(combineBits([
            7, // set all unused bits to 1
            bitsToByte(setups.wakeOnTouch[i], 3),
            bitsToByte(setups.burstLength[i], 4),
            bitsToByte(setups.AKS[i], 6),
            bitsToByte(setups.scopeSync[i], 7)
        ]));
    }

    bytes.push(combineBits([
        bitsToByte(setups.sleepDuration, 0),
        bitsToByte(7, 3), // set all unused bits to 1
        bitsToByte(setups.mainsSync, 6),
        bitsToByte(1, 7), // set all unused bits to 1
    ]));
    bytes.push(setups.awakeTimeout);
    bytes.push(setups.driftHoldTimeout);

    return bytes.map((byte) => {

        const hex = byte.toString(16);
        return hex.length === 1 ? `0${hex}` : hex;
    }).join(' ');
}

export function parseLine (line) {

    const chars = line.trim().split(' ');
    const buffer = new ArrayBuffer(chars.length);

    for (const [index, char] of chars.entries()) {
        buffer[index] = parseInt(char, 16);
    }

    return buffer;
}

/*
export function parseDump (line) {

    const buffer = parseLine(line);

    const status_0_7 = bits(buffer[1], [0, 1, 2, 3, 4, 5, 6, 7]);
    const status_8_15 = bits(buffer[2], [8, 9, 10, 11, 12, 13, 14, 15]);
    const status_16_23 = bits(buffer[3], [16, 17, 18, 19, 20, 21, 22, 23]);
    const status = Object.assign({}, status_0_7, status_8_15, status_16_23);

    const keys = [];

    // setups
    const negThreshold = [];
    const negDriftComp = [];
    const posDriftComp = [];
    const normalDILimit = [];
    const fastDILimit = [];
    const negRecalDelay = [];
    const wakeOnTouch = [];
    const burstLength = [];
    const AKS = [];
    const scopeSync = [];

    for (let i = 0; i<24; ++i) {
        // 4 Signal LSB
        // 5 Signal MSB
        // 6 Reference LSB
        // 7 Reference MSB
        // 8 Detect count (lower nibble)

        keys.push({
            signal: parse16Bits(buffer, 4 + i*4),
            reference: parse16Bits(buffer, 6 + i*4),
            detect: lowerNibble(buffer[8 + i*4])
        });

        // setups

        negThreshold.push(lowerNibble(buffer[131 + i]));
        negDriftComp.push(upperNibble(buffer[131 + i]));
        posDriftComp.push(lowerNibble(buffer[155 + i]));
        normalDILimit.push(lowerNibble(buffer[179 + i]));
        fastDILimit.push(upperNibble(buffer[179 + i]));
        negRecalDelay.push(buffer[203 + i]);
        wakeOnTouch.push(bit(buffer[227 + i], 3));
        burstLength.push(bitRange(buffer[227 + i], 4, 5));
        AKS.push(bit(buffer[227 + i], 6));
        scopeSync.push(bit(buffer[227 + i], 7));
    }

    return {
        // 0: reserved

        // 1: keys 0-7
        // 2: keys 8-15
        // 3: keys 16-23
        status,

        // 4 to 123: key data
        keys,

        // 124: nothing
        // 125: recalibrate all keys (write)
        // 126 to 129: nothing
        // 130: write lock. write 0x55 immediately before writing setups

        // 131 to 253: setups
        negThreshold,
        negDriftComp,
        posDriftComp,
        normalDILimit,
        fastDILimit,
        negRecalDelay,
        wakeOnTouch,
        burstLength,
        AKS,
        scopeSync,

        sleepDuration: bitRange(buffer[251], 0, 2),
        mainsSync: bit(buffer[251], 6),
        awakeTimeout: buffer[252],
        driftHoldTimeout: buffer[253]
    };
}
*/

/*
console.log(parseStatus(statusExample));
console.log(parseSetups(setupsExample));

console.log();
console.log(formatSetups(parseSetups(setupsExample)).toUpperCase());
console.log();
console.log(setupsExample.toUpperCase());
console.log();
console.log(formatSetups(parseSetups(setupsExample)).toUpperCase() === setupsExample.toUpperCase());
*/
