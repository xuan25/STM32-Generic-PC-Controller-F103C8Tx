// A simple demo of how to update config via HID on a browser

// the report ID of config update in HID descriptor
const GENERIC_CONTROL_REPORT_ID = 0x01

const filters = [
	{
		vendorId: 1155,
		productId: 22352
	}
];

if ("hid" in navigator) {
	// HID available on this browser
}

// Create a buffer for HID report
const reportBuffer = new Uint8Array(0x40 - 1);

// Prompt user to select a device.
const [device] = await navigator.hid.requestDevice({ filters });
// Wait for the HID connection to open before sending/receiving data.
await device.open();

const SUCCESS = 0x53
const ERROR = 0x45
const RAW = 0x00
const TEXT = 0x3A



// handle input report
device.addEventListener("inputreport", event => {
    const { data, device, reportId } = event;

    if (device.productId !== 22352 && reportId !== GENERIC_CONTROL_REPORT_ID) return;

    console.log(data)

    const arr = new Uint8Array(data.buffer)

    const responseFlag = arr[0]
    const dataType = arr[1]

    if (responseFlag == SUCCESS) {
        console.log("success");
    } else if (responseFlag == ERROR) {
        console.log("error");
    } else {
        console.log("unknown response");
    }

    if (dataType == RAW) {
        console.log(arr.slice(2));
    } else if (dataType == TEXT) {
        console.log(new TextDecoder().decode(arr.slice(2)));
        
    } else {
        console.log("unknown datatype");
    }

});


const ACTION = 0x41
const LIGHTING = 0x4C
const LIGHT_MIDICC_MAPPING = 0x4D
const SAVE_FLASH = 0x53
const RESET_FLASH = 0x52

const WRITE = 0x57
const READ = 0x52


async function saveConfigToFlash() {
    reportBuffer[0] = SAVE_FLASH;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, reportBuffer);
}

async function resetConfigInFlash() {
    reportBuffer[0] = RESET_FLASH;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, reportBuffer);
}

async function setRGB() {
    reportBuffer[0] = LIGHTING;
    reportBuffer[1] = WRITE;
    // see lighting_conf.h
    reportBuffer[2] = rgbID;
    reportBuffer[3] = channelR;
    reportBuffer[4] = channelG;
    reportBuffer[5] = channelB;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, reportBuffer);
}

async function setLightMIDICCMapping() {
    reportBuffer[ 0] = LIGHT_MIDICC_MAPPING;
    reportBuffer[ 1] = WRITE;
    // see lighting_conf.h
    reportBuffer[ 2] = controllerID;
    reportBuffer[ 3] = channelID;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, reportBuffer);
}


async function setAction() {
    reportBuffer[ 0] = ACTION;
    reportBuffer[ 1] = WRITE;
    // see inputs_conf.h
    reportBuffer[ 2] = actionID;
    reportBuffer[ 3] = type;
    reportBuffer[ 4] = byte0;
    reportBuffer[ 5] = byte1;
    reportBuffer[ 6] = byte2;
    reportBuffer[ 7] = byte3;
    reportBuffer[ 8] = byte4;
    reportBuffer[ 9] = byte5;
    reportBuffer[10] = byte6;
    reportBuffer[11] = byte7;
    reportBuffer[12] = byte8;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, reportBuffer);
}





async function getRGB() {
    reportBuffer[0] = LIGHTING;
    reportBuffer[1] = READ;
    // see lighting_conf.h
    reportBuffer[2] = rgbID;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, reportBuffer);
}

async function getLightMIDICCMapping() {
    reportBuffer[ 0] = LIGHT_MIDICC_MAPPING;
    reportBuffer[ 1] = READ;
    // see lighting_conf.h
    reportBuffer[ 2] = controllerID;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, reportBuffer);
}


async function getAction() {
    reportBuffer[ 0] = ACTION;
    reportBuffer[ 1] = READ;
    // see inputs_conf.h
    reportBuffer[ 2] = actionID;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, reportBuffer);
}
