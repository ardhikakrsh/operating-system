#ifndef __KERNEL_H__
#define __KERNEL_H__

extern void putInMemory(int segment, int address, char character);
extern int interrupt(int number, int AX, int BX, int CX, int DX);

void printString(char* str);
void readString(char* buf);
void clearScreen();

#endif // __KERNEL_H__
