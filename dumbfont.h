#define UNISIGSIZE 32

extern const unsigned char unisig8[UNISIGSIZE];
extern const unsigned char unisig16[UNISIGSIZE];
extern const unsigned char unisig32[UNISIGSIZE];
extern const unsigned char unisig64[UNISIGSIZE];

void die(const char *msg);
unsigned int read_unisig(void);
