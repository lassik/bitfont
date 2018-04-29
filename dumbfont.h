#define UNISIGSIZE 32

extern const unsigned char unisig8[UNISIGSIZE];
extern const unsigned char unisig16[UNISIGSIZE];
extern const unsigned char unisig32[UNISIGSIZE];
extern const unsigned char unisig64[UNISIGSIZE];

typedef void (*voidfun_t)(void);

void die(const char *msg);
unsigned int read_unisig(void);
void binary_stdin(void);
void binary_stdout(void);
