// PAUSE keyword (press capslock)

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <hardware/flash.h>
#include <pico/bootrom.h>
#include <KeyboardBT.h>
#include <FatFS.h>
#include <FatFSUSB.h>

#define KEY_CTRL 128
#define KEY_SHIFT 129
#define KEY_ALT 130
#define KEY_GUI 131
#define KEY_RALT 134

typedef struct {
  uint8_t modifier;
  uint8_t keyCode;
  char key[16];
} keyMapStruct;

#define MAPSIZE 147
keyMapStruct keyMap[MAPSIZE] = {
  {0, 32, " "},
  {0, 97, "a"},
  {0, 98, "b"},
  {0, 99, "c"},
  {0, 100, "d"},
  {0, 101, "e"},
  {0, 102, "f"},
  {0, 103, "g"},
  {0, 104, "h"},
  {0, 105, "i"},
  {0, 106, "j"},
  {0, 107, "k"},
  {0, 108, "l"},
  {0, 109, "m"},
  {0, 110, "n"},
  {0, 111, "o"},
  {0, 112, "p"},
  {0, 113, "q"},
  {0, 114, "r"},
  {0, 115, "s"},
  {0, 116, "t"},
  {0, 117, "u"},
  {0, 118, "v"},
  {0, 119, "w"},
  {0, 120, "x"},
  {0, 121, "y"},
  {0, 122, "z"},
  {0, 183, "å"},
  {0, 188, "ä"},
  {0, 187, "ö"},
  {KEY_SHIFT, 97, "A"},
  {KEY_SHIFT, 98, "B"},
  {KEY_SHIFT, 99, "C"},
  {KEY_SHIFT, 100, "D"},
  {KEY_SHIFT, 101, "E"},
  {KEY_SHIFT, 102, "F"},
  {KEY_SHIFT, 103, "G"},
  {KEY_SHIFT, 104, "H"},
  {KEY_SHIFT, 105, "I"},
  {KEY_SHIFT, 106, "J"},
  {KEY_SHIFT, 107, "K"},
  {KEY_SHIFT, 108, "L"},
  {KEY_SHIFT, 109, "M"},
  {KEY_SHIFT, 110, "N"},
  {KEY_SHIFT, 111, "O"},
  {KEY_SHIFT, 112, "P"},
  {KEY_SHIFT, 113, "Q"},
  {KEY_SHIFT, 114, "R"},
  {KEY_SHIFT, 115, "S"},
  {KEY_SHIFT, 116, "T"},
  {KEY_SHIFT, 117, "U"},
  {KEY_SHIFT, 118, "V"},
  {KEY_SHIFT, 119, "W"},
  {KEY_SHIFT, 120, "X"},
  {KEY_SHIFT, 121, "Y"},
  {KEY_SHIFT, 122, "Z"},
  {KEY_SHIFT, 183, "Å"},
  {KEY_SHIFT, 188, "Ä"},
  {KEY_SHIFT, 187, "Ö"},
  {0, 49, "1"},
  {0, 50, "2"},
  {0, 51, "3"},
  {0, 52, "4"},
  {0, 53, "5"},
  {0, 54, "6"},
  {0, 55, "7"},
  {0, 56, "8"},
  {0, 57, "9"},
  {0, 48, "0"},
  {0, 192, "-"},
  {0, 184, "¨"},
  {0, 182, "´"},
  {0, 189, "§"},
  {0, 181, "+"},
  {0, 185, "'"},
  {0, 44, ","},
  {0, 46, "."},
  {0, 236, "<"},
  {KEY_SHIFT, 192, "_"},
  {KEY_SHIFT, 184, "^"},
  {KEY_SHIFT, 182, "`"},
  {KEY_SHIFT, 189, "½"},
  {KEY_SHIFT, 181, "?"},
  {KEY_SHIFT, 185, "*"},
  {KEY_SHIFT, 44, ";"},
  {KEY_SHIFT, 46, ":"},
  {KEY_SHIFT, 236, ">"},
  {KEY_SHIFT, 49, "!"},
  {KEY_SHIFT, 50, "\""},
  {KEY_SHIFT, 51, "#"},
  {KEY_SHIFT, 52, "¤"},
  {KEY_SHIFT, 53, "%"},
  {KEY_SHIFT, 54, "&"},
  {KEY_SHIFT, 55, "/"},
  {KEY_SHIFT, 56, "("},
  {KEY_SHIFT, 57, ")"},
  {KEY_SHIFT, 48, "="},
  {KEY_RALT, 101, "€"},
  {KEY_RALT, 50, "@"},
  {KEY_RALT, 51, "£"},
  {KEY_RALT, 52, "$"},
  {KEY_RALT, 55, "{"},
  {KEY_RALT, 56, "["},
  {KEY_RALT, 57, "]"},
  {KEY_RALT, 48, "}"},
  {KEY_RALT, 181, "\\"},
  {KEY_RALT, 184, "~"},
  {KEY_RALT, 236, "|"},
  {0, 194, "F1"},
  {0, 195, "F2"},
  {0, 196, "F3"},
  {0, 197, "F4"},
  {0, 198, "F5"},
  {0, 199, "F6"},
  {0, 200, "F7"},
  {0, 201, "F8"},
  {0, 202, "F9"},
  {0, 203, "F10"},
  {0, 204, "F11"},
  {0, 205, "F12"},
  {0, 240, "F13"},
  {0, 241, "F14"},
  {0, 242, "F15"},
  {0, 243, "F16"},
  {0, 244, "F17"},
  {0, 245, "F18"},
  {0, 246, "F19"},
  {0, 247, "F20"},
  {0, 248, "F21"},
  {0, 249, "F22"},
  {0, 250, "F23"},
  {0, 251, "F24"},
  {0, 10, "ENTER"},
  {0, 177, "ESC"},
  {0, 8, "BACKSPACE"},
  {0, 9, "TAB"},
  {0, 209, "INSERT"},
  {0, 210, "HOME"},
  {0, 211, "PGUP"},
  {0, 212, "DELETE"},
  {0, 213, "END"},
  {0, 214, "PGDN"},
  {0, 215, "RIGHT"},
  {0, 216, "LEFT"},
  {0, 217, "DOWN"},
  {0, 218, "UP"},
  {0, 0, ""}
};
#define SERIALNUM "0000000000000000"

int minTimePerChar = 10;
int maxTimePerChar = 50;

char selectedPayload[70] = "payload1.txt";
const char settingsFileName[] = "settings.txt";
char keyboardName[70] = "Bluetooth adapter";

bool capsState = false;

void setCaps(bool caps);
void ledCB(bool numlock, bool capslock, bool scrolllock, bool compose, bool kana, void *cbData);
void serialLock();
void processSettingsLine(char line[80]);
void readSettings();
void parseLine(char line[8192]);
void runPayload();
uint8_t charToCode(char *c);
uint8_t charToMod(char *c);
void keyParser(char *line);
void modParser(char *line);
int starts_with(const char *line, const char *prefix);
int random_number(int min, int max);

void setup() {
  //serialLock();

  FatFS.begin();
  FatFSUSB.begin();

  delay(100);
  readSettings();

  KeyboardBT.begin(keyboardName, nullptr, KeyboardLayout_sv_SE);
  KeyboardBT.onLED(ledCB);
  KeyboardBT.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  while (!capsState) {
    digitalWrite(LED_BUILTIN, 0);
    delay(500);
    digitalWrite(LED_BUILTIN, 1);
    delay(500);
  }
  delay(100);
  while (capsState) { delay(100); }
  readSettings();
  delay(50);
  runPayload();
}

void setCaps(bool caps) {
  capsState = caps;
}

void ledCB(bool numlock, bool capslock, bool scrolllock, bool compose, bool kana, void *cbData) {
  (void) numlock;
  (void) scrolllock;
  (void) compose;
  (void) kana;
  (void) cbData;
  setCaps(capslock);
}

void serialLock() { 
  char buffer[100];
  char serialNum[17] = {0};
  serialNum[16] = '\0';
  uint8_t serialNum8[8];

  flash_get_unique_id(serialNum8);
  for (int i = 0; i < 8; i++) {
    itoa(serialNum8[i], buffer, 16);
    strcat(serialNum, buffer);
  }
  if (strcmp(serialNum, SERIALNUM) != 0) {
    while (1) {;;}
  }
}

void processSettingsLine(char line[80]) {
  if (starts_with(line, "MIN ")) {
    const uint8_t len = 4;
    char* time = line + len;
    minTimePerChar = atoi(time);
  } 
  else if (starts_with(line, "MAX ")) {
    const uint8_t len = 4;
    char* time = line + len;
    maxTimePerChar = atoi(time);
  } 
  else if (starts_with(line, "PAYLOAD ")) {
    const uint8_t len = 8;
    char* filename = line + len;
    strcpy(selectedPayload, filename);
  }
  else if (starts_with(line, "NAME ")) {
    const uint8_t len = 5;
    char* kbName = line + len;
    strcpy(keyboardName, kbName);
  }

  File payloadCheck = FatFS.open(selectedPayload, "r");
  if (!payloadCheck) {
    strcpy(selectedPayload, "payload1.txt");
  } else {
    payloadCheck.close();
  }
  if (minTimePerChar < 0) {
    minTimePerChar = 10;
  }
  if (maxTimePerChar < 1) {
    maxTimePerChar = 50;
  }
}

void readSettings() {
  File file = FatFS.open(settingsFileName, "r");

  char line[80];
  int index = 0;

  while (file.available()) {
    char c = file.read();
    if (c == '\n' || index == sizeof(line) - 1) {
      line[index] = '\0';
      processSettingsLine(line);
      index = 0;
    } else {
      line[index++] = c;
    }
  }
  if (index > 0) {
    line[index] = '\0';
    processSettingsLine(line);
  }
  file.close();
}

void parseLine(char line[8192]) {
  if (!starts_with(line, "REM ")) {
    if (starts_with(line, "STRING ")) {
      char *text = line+7;
      int i = 0;
      while (i < strlen(text)) {
        char key1[2] = {text[i], '\0'};
        char key2[3] = {text[i],text[i+1], '\0'};
        char key3[4] = {text[i], text[i+1], text[i+2], '\0'};
        char key4[5] = {text[i], text[i+1], text[i+2], text[i+3], '\0'};
        uint8_t code = 0;
        uint8_t mod = 0;
        if (charToCode(key1) != 0) {
          code = charToCode(key1);
          mod = charToMod(key1);
          i += 1;
        }
        else if (charToCode(key2) != 0) {
          code = charToCode(key2);
          mod = charToMod(key2);
          i += 2;
        }
        else if (charToCode(key3) != 0) {
          code = charToCode(key3);
          mod = charToMod(key3);
          i += 3;
        }
        else if (charToCode(key4) != 0) {
          code = charToCode(key4);
          mod = charToMod(key4);
          i += 4;
        }
        else {
          i += 1;
        }
        int randDelay = random_number(minTimePerChar, maxTimePerChar);
        delay(1);
        delay(randDelay);
        while (capsState) { delay(100); }
        KeyboardBT.press(mod);
        KeyboardBT.press(code);
        delay(1);
        KeyboardBT.releaseAll();
      }
    }
    else if (starts_with(line, "DELAY ")) {
      uint delay_time;
      if(sscanf(line + 6, "%d", &delay_time) < 0) {
        delay(50);
      } else {
        delay(delay_time);
      }
    }
    else if (starts_with(line, "ENTER")) {
      KeyboardBT.press(10);
      KeyboardBT.releaseAll();
    }
    else if (starts_with(line, "GUI ") || starts_with(line, "CTRL ") || starts_with(line, "SHIFT ") || starts_with(line, "ALT ")) {
      modParser(line);
    }
    else if (starts_with(line, "GUI")) {
      KeyboardBT.press(KEY_GUI);
      KeyboardBT.releaseAll();
    }
    else if (starts_with(line, "KEY ")) {
      keyParser(line);
    }
  }
}

void runPayload() {
  File file = FatFS.open(selectedPayload, "r");
  if (!file) {
    return;
  }
  char line[8192];
  int index = 0;

  while (file.available()) {
    char c = file.read();
    if (c == '\n' || index == sizeof(line) - 1) {
      line[index] = '\0';
      parseLine(line);
      index = 0;
    } else {
      line[index++] = c;
    }
  }
  if (index > 0) {
    line[index] = '\0';
    parseLine(line);
  }
  file.close();
}

uint8_t charToCode(char *c) {
  for (int i = 0; i < MAPSIZE; i++) {
    if (strcmp(keyMap[i].key, c) == 0) {
      return keyMap[i].keyCode;
    }
  }
  return 0;
}

uint8_t charToMod(char *c) {
  for (int i = 0; i < MAPSIZE; i++) {
    if (strcmp(keyMap[i].key, c) == 0) {
      return keyMap[i].modifier;
    }
  }
  return 0;
}

void modParser(char *line) {
  uint8_t modifiers = 0;
  int modifierLength = 0;
  if (strstr(line, "GUI ") != 0) {
    KeyboardBT.press(KEY_GUI);
    modifierLength += 4;
  }
  if (strstr(line, "ALT ") != 0) {
    KeyboardBT.press(KEY_ALT);
    modifierLength += 4;
  }
  if (strstr(line, "CTRL ") != 0) {
    KeyboardBT.press(KEY_CTRL);
    modifierLength += 5;
  }
  if (strstr(line, "SHIFT ") != 0) {
    KeyboardBT.press(KEY_SHIFT);
    modifierLength += 6;
  }

  uint8_t keycode = 0;
  char* key = line + modifierLength;
  if (charToCode(key) != 0) {
    keycode = charToCode(key);
  } 
  else {
    key[strlen(key) - 1] = '\0';
    keycode = charToCode(key);
  }
  KeyboardBT.press(keycode);
  KeyboardBT.releaseAll();
}

void keyParser(char *line) {
  uint8_t keycode = 0;
  char* key = line + 4;
  if (charToCode(key) != 0) {
    keycode = charToCode(key);
  } 
  else {
    key[strlen(key) - 1] = '\0';
    keycode = charToCode(key);
  }
  KeyboardBT.press(charToMod(key));
  KeyboardBT.press(keycode);
  KeyboardBT.releaseAll();
}

int starts_with(const char *line, const char *prefix) {
  if(strncmp(line, prefix, strlen(prefix)) == 0) return 1;
    return 0;
}

int random_number(int min, int max) {
    srand(time(NULL));
    return (rand() % (max - min + 1)) + min;
}