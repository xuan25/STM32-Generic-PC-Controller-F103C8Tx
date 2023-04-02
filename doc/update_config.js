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

// Prompt user to select a device.
const [device] = await navigator.hid.requestDevice({ filters });
// Wait for the HID connection to open before sending/receiving data.
await device.open();

// Create a buffer for HID report
const configUpdateReport = new Uint8Array(0x40 - 1);


const ACTION = 0x41
const LIGHTING = 0x4C
const SAVE_FLASH = 0x53
const RESET_FLASH = 0x52

const WRITE = 0x57
const READ = 0x52


async function saveConfigToFlash() {
    rgbUpdateReport[0] = SAVE_FLASH;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, rgbUpdateReport);
}

async function ResetConfigInFlash() {
    rgbUpdateReport[0] = RESET_FLASH;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, rgbUpdateReport);
}

async function SetRGB() {
    rgbUpdateReport[0] = LIGHTING;
    rgbUpdateReport[1] = WRITE;
    // see lighting_conf.h
    rgbUpdateReport[2] = rgbID;
    rgbUpdateReport[3] = channelR;
    rgbUpdateReport[4] = channelG;
    rgbUpdateReport[5] = channelB;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, rgbUpdateReport);
}

async function SetAction() {
    rgbUpdateReport[ 0] = ACTION;
    rgbUpdateReport[ 1] = WRITE;
    // see inputs_conf.h
    rgbUpdateReport[ 2] = actionID;
    rgbUpdateReport[ 3] = type;
    rgbUpdateReport[ 4] = byte0;
    rgbUpdateReport[ 5] = byte1;
    rgbUpdateReport[ 6] = byte2;
    rgbUpdateReport[ 7] = byte3;
    rgbUpdateReport[ 8] = byte4;
    rgbUpdateReport[ 9] = byte5;
    rgbUpdateReport[10] = byte6;
    rgbUpdateReport[11] = byte7;
    rgbUpdateReport[12] = byte8;
    await device.sendReport(GENERIC_CONTROL_REPORT_ID, rgbUpdateReport);
}
