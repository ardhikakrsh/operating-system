#include "kernel.h"
#include "std_lib.h"

int main()
{
  char buf[128];
  clearScreen();
  printString("Welcome to MengOS\n");

  while (true)
  {
    printString("$ ");
    readString(buf);
    printString(buf);
    printString("\n");
  }
}

void clearScreen()
{
  // first actually clear the screen
  byte ah = 6;    // scrolling mode
  byte al = 0x00; // clears cells
  byte bh = 0x0E; // black with yellow text (dino theme!)
  byte ch = 0x00; // start at row 0
  byte cl = 0x00; // start at col 0
  byte dh = 25;   // end at row index 24
  byte dl = 80;   // end at col index
  // place in ax, bx, cx, dx
  int ax = ah * 256 + al;
  int bx = bh * 256;
  int cx = ch * 256 + cl;
  int dx = dh * 256 + dl;
  interrupt(0x10, ah * 256 + al, bh * 256, cx, dx);

  // disable weird blinking thing
  interrupt(0x10, 0x1003, 0, 0, 0);
  // full block cursor
  // interrupt(0x10, 0x0100, 0, 0x00FF, dx);
  // place cursor back at the top
  interrupt(0x10, 0x0200, 0, 0, 0); // ah=0x02, bh=page, dh=row, dl=col
}

void printString(char *str)
{
  // used as a counter
  int i = 0;
  // print until we hit the end
  while (str[i] != '\0')
  {
    char ah = 0x0E;
    // calculate ax
    int ax;
    // check for the \n or \r (need to print both to get a new line)
    if (str[i] == 0x0D || str[i] == 0x0A)
    {
      ax = ah * 256 + 0x0A;
      interrupt(0x10, ax, 0, 0, 0);
      ax = ah * 256 + 0x0D;
      interrupt(0x10, ax, 0, 0, 0);
    }
    else
    {
      // print the character
      ax = ah * 256 + str[i];
      interrupt(0x10, ax, 0, 0, 0);
    }
    i++;
  }
}

void readString(char *buf)
{
  int i = 0;
  char c = 0;
  char ah = 0x0E;
  int ax;

  while (true)
  {
    c = interrupt(0x16, 0, 0, 0, 0);
    if (c == 0x0D)
    { // Enter key
      ax = ah * 256 + 0x0A;
      interrupt(0x10, ax, 0, 0, 0);
      ax = ah * 256 + 0x0D;
      interrupt(0x10, ax, 0, 0, 0);
      buf[i] = '\0';
      break;
    }
    else if (c == 0x08 && i > 0)
    { // Backspace
      buf[i] = 0;
      i--;
      printString("\b \b"); // Erase character from screen
    }
    else if (c != 0x08)
    { // Regular character
      buf[i] = c;
      i++;
      interrupt(0x10, 0x0E00 | c, 0, 0, 0); // Echo character
    }
  }
}