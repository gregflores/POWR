/*
 * clientServerMode.h
 *
 *  Created on: Apr 18, 2017
 *      Author: flore
 */

#ifndef CLIENTSERVERMODE_H_
#define CLIENTSERVERMODE_H_

void runAsServer();
void runAsClient();
// used for client example
void waitForEvent();
//
// network configuration
const u_char sourceIP[4] = { 192, 168, 1, 10 }; // local IP
const u_char gatewayIP[4] = { 192, 168, 1, 1 }; // gateway IP
const u_char subnetMask[4] = { 255, 255, 255, 0 }; // subnet mask
// network configuration for client mode
const u_char destinationIP[4] = { 192, 168, 1, 148 }; // destination IP
const u_int destinationPort = 23; // destination port

#endif /* CLIENTSERVERMODE_H_ */
