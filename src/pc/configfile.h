#ifndef CONFIGFILE_H
#define CONFIGFILE_H

extern bool         configFullscreen;
extern int          configMaxSpeedupFrameRate;
extern unsigned int configKeyA;
extern unsigned int configKeyB;
extern unsigned int configKeyStart;
extern unsigned int configKeyR;
extern unsigned int configKeyL;
extern unsigned int configKeyZ;
extern unsigned int configKeyCUp;
extern unsigned int configKeyCDown;
extern unsigned int configKeyCLeft;
extern unsigned int configKeyCRight;
extern unsigned int configKeyStickUp;
extern unsigned int configKeyStickDown;
extern unsigned int configKeyStickLeft;
extern unsigned int configKeyStickRight;
extern unsigned int configKeyDUp;
extern unsigned int configKeyDDown;
extern unsigned int configKeyDLeft;
extern unsigned int configKeyDRight;
extern unsigned int configSpeedupKey;

void configfile_load(const char *filename);
void configfile_save(const char *filename);

#endif
