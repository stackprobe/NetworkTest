int aes128_decrypt_extend(void *block, int &size, int tweek);
void CRPC_Crypt(autoList<uchar> *data, uchar clSeed[16], uchar svSeed[16], uchar encCounter[16]);
