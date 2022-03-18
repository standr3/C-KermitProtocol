#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc, char** argv) {
	unsigned char dataPkt[250];
	int ready_to_respond = 0;
    int sett_configured = 0;
    int local_seq = 0;
    settings s;
    msg r, t;
    pkt p;
	
    init(HOST, PORT);

    // int fileoutput;
    while(1) {
    	memset(r.payload, 0, sizeof(r.payload));
    	memset(t.payload, 0, sizeof(t.payload));
    	ready_to_respond = 0;

    	if(sett_configured == 0) {
    		
    		if(recv_message(&r) < 0) {
    			// error on recieving settings
    			continue;
    		} else {
    			// managed to get a message
    			p = *((pkt *) r.payload);
				if(p.TYPE != 'S') {
					// try again; need settings pkt first!
					perror("[rcv] Wrong packet type recieved");
					continue;
	    			
				} else {
					//recieved S type pkt
					s.MAXL = p.DATA[0]; 
					s.TIME = p.DATA[1]; 
					s.NPAD = p.DATA[2]; 
					s.PADC = p.DATA[3]; 
					s.EOL  = p.DATA[4]; 
					s.QCTL = p.DATA[5]; 
					s.QBIN = p.DATA[6]; 
					s.CHKT = p.DATA[7]; 
					s.REPT = p.DATA[8]; 
					s.CAPA = p.DATA[9]; 
					s.R    = p.DATA[10];

					sett_configured = 1;
					ready_to_respond = 1; 
				}
    		}
    	} else if (sett_configured == 1) {
    		// here are managed all the other types of packages

    		if(recv_message(&r) < 0) {
    			// error on recieving msg
    			continue;
    		} else {
    			p = *((pkt *) r.payload);
				if(p.TYPE == 'F') {
					// do stuff
					return 0;
	    		}
    		}

    	}

    	if(ready_to_respond == 1) {
			if(local_seq % 2 == 0) {
				perror("[rcv] Desynchronised ");
				return -1;
			} else {
				
				//ack or nack
	    		if(local_seq == 1) {
	    			// send same pkt with SEQ + 1 back
	    			p.SEQ++;
	    			p.TYPE = 'Y';
	    			t.len = sizeof(p);
	    			memset(t.payload, 0, sizeof(t.payload));
	    			memcpy(t.payload, &p, t.len);
	    			send_message(&t);
	    		} else {
	    			//
	    		}
			}

    	}
    	
    }

	return 0;
}
