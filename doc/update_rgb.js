// A simple demo of how to update lighting RGB config via HID on a browser

// the report ID of lighting update in HID descriptor
const RGB_UPDATE_REPORT_ID = 0x07

function setBufferConfig(buffer, pushUpdateFlag, numUpdates) {
	buffer[0x00] = pushUpdateFlag | numUpdates << 4;
}

function setBufferData(buffer, pos, id, r, g, b) {
	buffer[0x01 + pos] = id;
	buffer[0x10 + pos] = r;
	buffer[0x1f + pos] = g;
	buffer[0x2e + pos] = b;
}

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
const rgbUpdateReport = new Uint8Array(0x40 - 1);

// pushUpdate, 3 updates
setBufferConfig(rgbUpdateReport, 1, 3);
// pos 0, id 0, black
setBufferData(rgbUpdateReport, 0, 0, 0xff * 0.2, 0xff * 0.2, 0xff * 0.2);
// pos 1, id 1, blue
setBufferData(rgbUpdateReport, 1, 1, 0x52 * 0.3, 0xf7 * 0.3, 0xf1 * 0.3);
// pos 2, id 4, white
setBufferData(rgbUpdateReport, 2, 4, 0xff * 0.3, 0xff * 0.3, 0xff * 0.3);

await device.sendReport(RGB_UPDATE_REPORT_ID, rgbUpdateReport);
