extern int KeepThread;

void critical(void);
void uncritical(void);
void doThreads(int millis = 1);

void StartThread(void (*func)(void));
