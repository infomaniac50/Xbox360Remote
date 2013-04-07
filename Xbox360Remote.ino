#include <IRremote.h>
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

PROGMEM prog_uint64_t codes[] = {
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

int RECV_PIN = 11;


IRsend irsend;
IRrecv irrecv(RECV_PIN);

decode_results results;

#define SERIAL_DEBUG 0

void setup(){
#if SERIAL_DEBUG
  Serial.begin(9600);
#endif
  irrecv.enableIRIn();
}

#if SERIAL_DEBUG
void printInt64Hex(unsigned long long number)
{
  unsigned long upper = 0UL;
  unsigned long lower = 0UL;
  upper = number >> 32;
  lower = number & 0xFFFFFFFF;
  Serial.print(upper, HEX);
  Serial.println(lower, HEX);
}
#endif

void loop(){
  if (irrecv.decode(&results)) {
    sendKey(results.value);
    delay(500);
    irrecv.resume(); // Receive the next value
  }
}

unsigned long long getCode(int code)
{
  unsigned long long out = 0ULL;
  
  memcpy_P(&out, codes + code, sizeof(unsigned long long));
  
  return out;
}

void sendKey(unsigned long key)
{
  static boolean toggle = false;
  static uint64_t last = 0LL;
  uint64_t code;
  
  switch(key)
  {
    case 3786778906UL:
    //Display
    code = getCode(Display);
    break;  
    case 1785214238UL:
    //Power
    code = getCode(OnOff);
    break;
    case 2464142367UL:
    //Mute
    code = getCode(Mute);
    break;
    case 3569241655UL:
    //Prev Ch
    code = getCode(Info);
    break;
    case 2420009735UL:
    //Chan Up
    code = getCode(UpArrow);
    break;    
    case 2003594874UL:
    //Vol Down
    code = getCode(LeftArrow);
    break;
    case 584651451UL:
    //Vol Up
    code = getCode(RightArrow);
    break;
    case 1907287990UL:
    //Chan Down
    code = getCode(DownArrow);
    break;
//    case 2123630850UL:
//    Clear
//    code = getCode(Back);
//    break;
    case 1635659550UL:
    //Menu
    code = getCode(XboxFancyButton);
    break;
//    case 1927151938UL:
//    Reset
//    code = getCode(DownArrow);
//    break;
    case 1808161147UL:
    //One
    code = getCode(Back);
    break;
    case 1935374718UL:
    //Two
    code = getCode(Play);
    break;
    case 516431295UL:
    //Three
    code = getCode(Enter);
    break;
    case 1158108118UL:
    //Four
    code = getCode(Rewind);
    break;
    case 3121565407UL:
    //Five
    code = getCode(Pause);
    break;
    case 1254415002UL:
    //Six
    code = getCode(FastForward);
    break;
    case 4130438875UL:
    //Seven
    code = getCode(X);
    break;
    case 4177530494UL:
    //Eight
    code = getCode(Y);
    break;
    case 3341359991UL:
    //Nine
    code = getCode(B);
    break;
    case 1086421151UL:
    //Input
    code = getCode(Prev);
    break;
    case 2307907450UL:
    //Zero
    code = getCode(A);
    break;
    case 216716127UL:
    //Skip
    code = getCode(Next);
    break;
    default:
    return;
  }
  
#if SERIAL_DEBUG
  printInt64Hex(code);
#endif
  
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
}


