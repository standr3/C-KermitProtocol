#ifndef LIB
#define LIB
/*
+------+------+------+------+-----+-----+------+------+-----+------+-----+
| MAXL | TIME | NPAD | PADC | EOL | QCTL| QBIN | CHKT | REPT| CAPA | R |
+------+------+------+------+-----+-----+------+------+-----+------+-----+
*/
typedef struct {
	unsigned char MAXL;
	unsigned char TIME;
	unsigned char NPAD; 
	unsigned char PADC;
	unsigned char EOL;
	unsigned char QCTL;
	unsigned char QBIN;
	unsigned char CHKT;
	unsigned char REPT;
	unsigned char CAPA;
	unsigned char R;
} settings;

/*
+------+-----+-----+------+------+-------+------+
| SOH | LEN | SEQ | TYPE | DATA | CHECK | MARK |
+------+-----+-----+------+------+-------+------+
*/
typedef struct {
	unsigned char  SOH;
	unsigned char  LEN;
	unsigned char  SEQ;
	unsigned char  TYPE;
	unsigned char  DATA[250];
	unsigned char  CHECK[2];
	unsigned char  MARK;
} pkt;
/*===========================================================//
// – ’S’: Send-init (primul pachet care se transmite)
// – ’F’: File Header
// – ’D’: Data
// – ’Z’: EOF (End Of File)
// – ’B’: EOT (End Of Transaction) - intreruperea transmisiei
// – ’Y’: ACK
// – ’N’: NAK
// – ’E’: eroare
//===========================================================*/

typedef struct {
    int len;
    char payload[1400];
} msg;

void init(char* remote, int remote_port);
void set_local_port(int port);
void set_remote(char* ip, int port);
int send_message(const msg* m);
int recv_message(msg* r);
msg* receive_message_timeout(int timeout); //timeout in milliseconds
unsigned short crc16_ccitt(const void *buf, int len);

#endif

