/*
 * clientServerMode.c
 *
 *  Created on: Apr 18, 2017
 *      Author: flore
 */

#include "clientServerMode.h"

void runAsServer() {
	//while (1) {
	startServer(0, 80);
	// wait for connection
	// TODO in the future, we will put MCU to sleep and use interrupt to wake it up
	waitForConnection(0);
	// connection was established, now wait for data
	waitForData(0);
	// we've got data, process it
	Request request = { 0, 0, 0, 0 };
	if (parseRequest(&request)) {
		// request is OK, process request
		addHTTP200ResponseToBuffer();
		processRequest(&request);
	} else {
		// cannot parse request
		addHTTP400ResponseToBuffer();
	}
	// flush buffer, disconnect, & close
	stopServer(0);
	//}
}

extern const u_char destinationIP[4]; // destination IP
extern const u_int destinationPort; // destination port

void runAsClient(uint8_t state, uint16_t a, uint16_t b) {

	//while (1) {
		//
		waitForEvent();
		//
		startClient(0, (u_char *) destinationIP, (u_char) destinationPort);

		// send request
		if(state == 1) sendTempRequest(a,b);
		// wait for response
		//waitForData(0);
		// we've got data, process it
		//TODO we could parse the response and verify status is 200, but for now, let's assume it is OK.
		//processResponse();
		// disconnect & close
		stopClient(0);
	//}
}

// client example, wait for button press or some other event
void waitForEvent() {
	// test delay
	_delay_cycles(2*48000000);
}
