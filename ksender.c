#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc, char** argv) {
    unsigned char dataPkt[250];
    int sett_configured = 0;
    settings s;
    msg r, t;
    pkt p;
   
    init(HOST, PORT);   
    
    memset(t.payload, 0, sizeof(t.payload));
    memset(dataPkt, 0, sizeof(dataPkt));
    memset(&p, 0, sizeof(p));
    /*do data*/
    dataPkt[0]  = s.MAXL = 250; 
    dataPkt[1]  = s.TIME = 5;   
    dataPkt[2]  = s.NPAD = 0x00;
    dataPkt[3]  = s.PADC = 0x00;
    dataPkt[4]  = s.EOL  = 0x0D;
    dataPkt[5]  = s.QCTL = 0x00;
    dataPkt[6]  = s.QBIN = 0x00;
    dataPkt[7]  = s.CHKT = 0x00;
    dataPkt[8]  = s.REPT = 0x00;
    dataPkt[9]  = s.CAPA = 0x00;
    dataPkt[10] = s.R    = 0x00;
    /*do pkt*/
    p.SOH = 0x01;
    p.LEN = 11 + 5;
    p.SEQ = 0x00;
    p.TYPE = 'S';
    memcpy(p.DATA, &dataPkt, sizeof(s));
    p.CHECK[0] = 0;
    p.CHECK[1] = 0;
    p.MARK = dataPkt[4];
    /*do msg*/
    t.len = sizeof(p);
    memcpy(t.payload, &p, t.len); 
    

    msg *aux = NULL;
    for (int i = 0; i < 2; i++) {
        send_message(&t);
        aux = receive_message_timeout(s.TIME * 1000);
        if(aux != NULL) {
            r = *aux;
            break;
        }
    }
        
    if (aux == NULL) {
        perror("[snd] Cannot send settings");
        return -1;
    }

    p = *((pkt *) r.payload);
    char buff[250];
    int fileinput;
    if (p.TYPE = 'Y') {
        //For every file 
        for (int i = 1; i < argc; i++) {
            fileinput = open ("sursa", O_RDONLY);
            if (fileinput < 0) {
                return -1;
            }
            memset(t.payload, 0, sizeof(t.payload));
            memset(p.DATA, 0, sizeof(p.DATA));

            for (int j = 0; j < strlen(argv[i]); j++) {
                p.DATA[j] = argv[i][j];
            }

            p.LEN = strlen(argv[i]) + 5;
            p.SEQ ++;
            p.TYPE = 'F';
            t.len = sizeof(p);
            memcpy(t.payload, &p, t.len); 

            msg *aux = NULL;
            for (int i = 0; i < 2; i++) {
                send_message(&t);
                aux = receive_message_timeout(s.TIME * 1000);
                if(aux != NULL) {
                    r = *aux;
                    break;
                }
            }
            if (aux == NULL) {
                perror("[snd] Cannot send msg");
                return -1;
            }
            return 0;

        }            
    } else {
        printf("[snd] Some error occured \n");
    }

    return 0;
}

