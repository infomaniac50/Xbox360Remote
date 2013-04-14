/*
Xbox360Remote.ino - A sketch for the Arduino platform to control an Xbox 360 using infrared remote control protocols.
Author: Derek Chafin
Version: 1.0.1
Modified: April 6, 2013
Project Page: http://www.coding-squared.com/blog/2011/12/xbox-360-remote-control-with-an-arduino/
License
This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/.
*/

/*********Configure your setup********** */
#define SERIAL_TEST_PROGMEM 0 //Set to a non zero value to test the progmem code array.
#define USE_KEYPAD_LIB 0 //Set to a non zero value if you are using a simple non-matrix keypad.
#define SERIAL_DEBUG 0 //Set to a non zero value if you want Serial output.
#define SERIAL_INPUT 1 //Set to a non zero value if you want Serial input.
/*******************************************/

#if SERIAL_TEST_PROGMEM
//Failsafe if we are testing the progmem. Do not change.
#undef SERIAL_DEBUG
#define SERIAL_DEBUG 1 
#undef SERIAL_INPUT
#define SERIAL_INPUT 1
#else
#include <IRremote.h>

#if USE_KEYPAD_LIB
#include <KeypadSimple.h>
#endif
#endif
#include <avr/pgmspace.h>

enum CodeIndex {
OpenClose = 0,
XboxFancyButton,
OnOff,
Stop,
Pause,
Rewind,
FastForward,
Prev,
Next,
Play,
Display,
Title,
DVD_Menu,
Back,
Info,
UpArrow,
LeftArrow,
RightArrow,
DownArrow,
OK,
Y,
X,
A,
B,
ChUp,
ChDown,
VolDown,
VolUp,
Mute,
Start,
Play2,
Enter,
Record,
Clear,
Zero,
One,
Two,
Three,
Four,
Five,
Six,
Seven,
Eight,
Nine,
Hundred,
Reload
};

uint64_t codes[] PROGMEM = {
  0xc800f7428LL,  //OpenClose
  0xc800ff464LL,  //XboxFancyButton
  0xc800f740cLL,  //OnOff
  0xc800ff419LL,  //Stop
  0xc800f7418LL,  //Pause
  0xc800ff415LL,  //Rewind
  0xc800f7414LL,  //FastForward
  0xc800ff41bLL,  //Prev
  0xc800f741aLL,  //Next
  0xc800ff416LL,  //Play
  0xc800f744fLL,  //Display
  0xc800ff451LL,  //Title
  0xc800f7424LL,  //DVD_Menu
  0xc800ff423LL,  //Back
  0xc800f740fLL,  //Info
  0xc800ff41eLL,  //UpArrow
  0xc800f7420LL,  //LeftArrow
  0xc800ff421LL,  //RightArrow
  0xc800f741fLL,  //DownArrow
  0xc800ff422LL,  //OK
  0xc800f7426LL,  //Y
  0xc800ff468LL,  //X
  0xc800f7466LL,  //A
  0xc800ff425LL,  //B
  0xc800f7412LL,  //ChUp
  0xc800ff413LL,  //ChDown
  0xc800ff411LL,  //VolDown
  0xc800ff410LL,  //VolUp
  0xc800ff40eLL,  //Mute
  0xc800ff40dLL,  //Start
  0xc800f7416LL,  //Play2
  0xc800ff40bLL,  //Enter
  0xc800f7417LL,  //Record
  0xc800ff40aLL,  //Clear
  0xc800f7400LL,  //Zero
  0xc800f7401LL,  //One
  0xc800ff402LL,  //Two
  0xc800f7403LL,  //Three
  0xc800ff404LL,  //Four
  0xc800f7405LL,  //Five
  0xc800ff406LL,  //Six
  0xc800f7407LL,  //Seven
  0xc800ff408LL,  //Eight
  0xc800f7409LL,  //Nine
  0xc800ff41dLL,  //Hundred
  0xc800f741cLL   //Reload
};

#ifdef KEYPADSIMPLE_H
const byte KEYS = 12; //number of keys

char keys[KEYS] = {
  '1','2','3','4',
  '5','6','7','8',
  '9','0','*','#'
};
byte keyPins[KEYS] = {31, 32, 33, 34, 35, 36, 37, 38, 39, 30, 40, 41}; //connect to the row pinouts of the keypad
#else
  const char NO_KEY = '\0';
#endif
#ifdef IRremote_h
IRsend irsend;
#endif
#ifdef KEYPADSIMPLE_H
KeypadSimple keypad = KeypadSimple( makeKeymap(keys), keyPins, KEYS);
#endif

void setup(){
#if SERIAL_DEBUG || SERIAL_INPUT
  Serial.begin(9600);
#endif
#ifdef KEYPADSIMPLE_H
  keypad.setDebounceTime(500);
#endif
}

#if SERIAL_DEBUG
size_t printInt64Hex(unsigned long long n, uint8_t base = DEC)
{
  //stole this code from the printNumber function in Print.h :P

  char buf[8 * sizeof(long long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    unsigned long m = n;
    n /= base;
    char c = m - base * n;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return Serial.write(str);
}
#endif

void loop(){
  char key;
#ifdef KEYPADSIMPLE_H
  key = keypad.getKey();
#elif SERIAL_INPUT
  if (Serial.available())
  {
    key = Serial.read();
  }
  else
  {
    key = NO_KEY;
  }
#else
#error There is no input method defined.
#endif

  if (key != NO_KEY){
    sendKey(key);
  }
}

unsigned long long getCode(int code)
{
  unsigned long long out = 0ULL;
  
  memcpy_P(&out, (PGM_VOID_P)(&(codes[code])), sizeof(uint64_t));
  
  return out;
}

void sendKey(char key)
{
  static boolean toggle = false;
  static uint64_t last = 0LL;
  uint64_t code;
  
  switch(key)
  {
    case '4':
    code = getCode(UpArrow);
    break;
    case '8':
    code = getCode(DownArrow);
    break;    
    case '*':
    code = getCode(LeftArrow);
    break;
    case '#':
    code = getCode(RightArrow);
    break;
    case '1':
    code = getCode(Back);
    break;
    case '3':
    code = getCode(Enter);
    break;
    case '2':
    code = getCode(Play);
    break;
    case '6':
    code = getCode(Pause);
    break;
    case '5':
    code = getCode(Rewind);
    break;
    case '7':
    code = getCode(FastForward);
    break;
    case '0':
    code = getCode(Stop);
    break;
    case '9':
    code = getCode(OnOff);
    break;
  }
  
#if SERIAL_DEBUG
  printInt64Hex(code);
#endif
  
#ifdef IRremote_h
  if (last != code)
  {
    toggle = false;
  }
  
  if (!toggle) {
    irsend.sendRC6(code, 36);
  } else {
    irsend.sendRC6(code ^ 0x8000, 36);
  }
  
  last = code;
  toggle = !toggle;
#endif
}

