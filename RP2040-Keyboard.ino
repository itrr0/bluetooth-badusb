/* TODO:
  - Storage toggle
  - Toggle duckyscript
*/
#include "PluggableUSBHID.h"
#include "USBKeyboard.h"
#include "FlashIAPBlockDevice.h"
#include "PluggableUSBMSD.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "pico/bootrom.h"
#include "hardware/flash.h"
#include "USBPhy.h"

#define SERIALNUM "e661ac886366c024"

typedef struct {
  uint8_t keyCode;
  uint8_t modifier;
  char* key;
} keyMapStruct;

#define MAPSIZE 147

keyMapStruct keyMap[MAPSIZE] = {
  {0x2c, 0, " "},
  {0x04, 0, "a"},
  {0x05, 0, "b"},
  {0x06, 0, "c"},
  {0x07, 0, "d"},
  {0x08, 0, "e"},
  {0x09, 0, "f"},
  {0x0a, 0, "g"},
  {0x0b, 0, "h"},
  {0x0c, 0, "i"},
  {0x0d, 0, "j"},
  {0x0e, 0, "k"},
  {0x0f, 0, "l"},
  {0x10, 0, "m"},
  {0x11, 0, "n"},
  {0x12, 0, "o"},
  {0x13, 0, "p"},
  {0x14, 0, "q"},
  {0x15, 0, "r"},
  {0x16, 0, "s"},
  {0x17, 0, "t"},
  {0x18, 0, "u"},
  {0x19, 0, "v"},
  {0x1a, 0, "w"},
  {0x1b, 0, "x"},
  {0x1c, 0, "y"},
  {0x1d, 0, "z"},
  {0x2f, 0, "å"},
  {0x34, 0, "ä"},
  {0x33, 0, "ö"},
  {0x04, KEY_SHIFT, "A"},
  {0x05, KEY_SHIFT, "B"},
  {0x06, KEY_SHIFT, "C"},
  {0x07, KEY_SHIFT, "D"},
  {0x08, KEY_SHIFT, "E"},
  {0x09, KEY_SHIFT, "F"},
  {0x0a, KEY_SHIFT, "G"},
  {0x0b, KEY_SHIFT, "H"},
  {0x0c, KEY_SHIFT, "I"},
  {0x0d, KEY_SHIFT, "J"},
  {0x0e, KEY_SHIFT, "K"},
  {0x0f, KEY_SHIFT, "L"},
  {0x10, KEY_SHIFT, "M"},
  {0x11, KEY_SHIFT, "N"},
  {0x12, KEY_SHIFT, "O"},
  {0x13, KEY_SHIFT, "P"},
  {0x14, KEY_SHIFT, "Q"},
  {0x15, KEY_SHIFT, "R"},
  {0x16, KEY_SHIFT, "S"},
  {0x17, KEY_SHIFT, "T"},
  {0x18, KEY_SHIFT, "U"},
  {0x19, KEY_SHIFT, "V"},
  {0x1a, KEY_SHIFT, "W"},
  {0x1b, KEY_SHIFT, "X"},
  {0x1c, KEY_SHIFT, "Y"},
  {0x1d, KEY_SHIFT, "Z"},
  {0x2f, KEY_SHIFT, "Å"},
  {0x34, KEY_SHIFT, "Ä"},
  {0x33, KEY_SHIFT, "Ö"},
  {0x1e, 0, "1"},
  {0x1f, 0, "2"},
  {0x20, 0, "3"},
  {0x21, 0, "4"},
  {0x22, 0, "5"},
  {0x23, 0, "6"},
  {0x24, 0, "7"},
  {0x25, 0, "8"},
  {0x26, 0, "9"},
  {0x27, 0, "0"},
  {0x38, 0, "-"},
  {0x30, 0, "¨"},
  {0x2e, 0, "´"},
  {0x35, 0, "§"},
  {0x2d, 0, "+"},
  {0x31, 0, "'"},
  {0x36, 0, ","},
  {0x37, 0, "."},
  {100, 0, "<"},
  {0x38, KEY_SHIFT, "_"},
  {0x30, KEY_SHIFT, "^"},
  {0x2e, KEY_SHIFT, "`"},
  {0x35, KEY_SHIFT, "½"},
  {0x2d, KEY_SHIFT, "?"},
  {0x31, KEY_SHIFT, "*"},
  {0x36, KEY_SHIFT, ";"},
  {0x37, KEY_SHIFT, ":"},
  {100, KEY_SHIFT, ">"},
  {0x1e, KEY_SHIFT, "!"},
  {0x1f, KEY_SHIFT, "\""},
  {0x20, KEY_SHIFT, "#"},
  {0x21, KEY_SHIFT, "¤"},
  {0x22, KEY_SHIFT, "%"},
  {0x23, KEY_SHIFT, "&"},
  {0x24, KEY_SHIFT, "/"},
  {0x25, KEY_SHIFT, "("},
  {0x26, KEY_SHIFT, ")"},
  {0x27, KEY_SHIFT, "="},
  {0x08, KEY_RALT, "€"},
  {0x1f, KEY_RALT, "@"},
  {0x20, KEY_RALT, "£"},
  {0x21, KEY_RALT, "$"},
  {0x24, KEY_RALT, "{"},
  {0x25, KEY_RALT, "["},
  {0x26, KEY_RALT, "]"},
  {0x27, KEY_RALT, "}"},
  {0x2d, KEY_RALT, "\\"},
  {0x30, KEY_RALT, "~"},
  {100, KEY_RALT, "|"},
  {0x3a, 0, "F1"},
  {0x3b, 0, "F2"},
  {0x3c, 0, "F3"},
  {0x3d, 0, "F4"},
  {0x3e, 0, "F5"},
  {0x3f, 0, "F6"},
  {0x40, 0, "F7"},
  {0x41, 0, "F8"},
  {0x42, 0, "F9"},
  {0x43, 0, "F10"},
  {0x44, 0, "F11"},
  {0x45, 0, "F12"},
  {0x68, 0, "F13"},
  {0x69, 0, "F14"},
  {0x6a, 0, "F15"},
  {0x6b, 0, "F16"},
  {0x6c, 0, "F17"},
  {0x6d, 0, "F18"},
  {0x6e, 0, "F19"},
  {0x6f, 0, "F20"},
  {0x70, 0, "F21"},
  {0x71, 0, "F22"},
  {0x72, 0, "F23"},
  {0x73, 0, "F24"},
  {0x28, 0, "ENTER"},
  {0x29, 0, "ESC"},
  {0x2a, 0, "BACKSPACE"},
  {0x2b, 0, "TAB"},
  {0x49, 0, "INSERT"},
  {0x4a, 0, "HOME"},
  {0x4b, 0, "PGUP"},
  {0x4c, 0, "DELETE"},
  {0x4d, 0, "END"},
  {0x4e, 0, "PGDN"},
  {0x4f, 0, "RIGHT"},
  {0x50, 0, "LEFT"},
  {0x51, 0, "DOWN"},
  {0x52, 0, "UP"},
  {0x0, 0, NULL},
};

#define LED_PIN 25

const char *settingsFileName = "/root/settings.txt";
int minTimePerChar = 10;
int maxTimePerChar = 50;

USBKeyboard Keyboard;
static FlashIAPBlockDevice bd(XIP_BASE + 0x030000, 0x170000);
static mbed::FATFileSystem fs("root");

char *selectedPayload = "/root/payload1.txt";

void serialLock();
int getCaps();
void readSettings();
void payloadPicker();
void runPayload();
uint8_t charToCode(char *c);
uint8_t charToMod(char *c);
void keyParser(char *line);
void modParser(char *line);
int starts_with(const char *line, const char *prefix);
int random_number(int min, int max);

void USBMSD::begin()
{
  int err = getFileSystem().mount(&bd);
  if (err) {
    err = getFileSystem().reformat(&bd);
  }
  if (err) {
    while(1);
  }
}

mbed::FATFileSystem &USBMSD::getFileSystem()
{
  static mbed::FATFileSystem fs("root");
  return fs;
}

USBPhy* phy;
USBMSD msd(phy, &bd, 0x0703, 0x0104, 0x0001);

void setup() {
  Serial.begin(9600);
  while (!Serial) {;;}
  serialLock(); // Lock device to serial number
  fs.mount(&bd);
  msd.connect();
  delay(500);
  readSettings();
  delay(500);
  if (!getCaps()) {
    msd.disconnect();
    runPayload();
  } else {
    msd.begin();
  }
}

void loop() {
  _gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  while (1) {
    gpio_put(LED_PIN, 1);
    delay(500);
    gpio_put(LED_PIN, 0);
    delay(500);
  }
}

void serialLock() { 
  char buffer[100];
  char serialNum[16] = {0};
  serialNum[17] = '\0';
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

int getCaps() {
  uint8_t statuses = Keyboard.lock_status();
  int status = (statuses >> 1) & 1;
  return status;
}

void readSettings() {
  uint8_t lineLen = 80;
  char line[lineLen];
  FILE* file;
  file = fopen(settingsFileName, "r");
  if (file != NULL) {
    while (fgets(line, sizeof(line), file)) {
      if (line[strlen(line) - 1] == '\n') {
        line[strlen(line) - 1] = '\0';
      }
      if (starts_with(line, "MIN ")) {
        uint8_t len = 4;
        char *time = line+len;
        for (int i = 0; i < strlen(line); i++) {
          if (i > len) {
            time[i-len] = line[i];
          }
        }
        time[lineLen-len] = '\0';
        int intTime = atoi(time);
        minTimePerChar = intTime;
      }
      else if (starts_with(line, "MAX ")) {
        uint8_t len = 4;
        char *time = line+len;
        for (int i = 0; i < strlen(line); i++) {
          if (i > len) {
            time[i-len] = line[i];
          }
        }
        time[lineLen-len] = '\0';
        int intTime = atoi(time);
        maxTimePerChar = intTime;
      }
      else if (starts_with(line, "PAYLOAD ")) {
        uint8_t len = 8;
        char *filename = line+len;
        for (int i = len; i < strlen(line); i++) {
          filename[i-len] = line[i];
        }
        FILE* fileCheck;
        char filenameRoot[100] = "/root/";
        strcat(filenameRoot, filename);
        selectedPayload = (char*)malloc(strlen(filenameRoot) + 1);
        strcpy(selectedPayload, filenameRoot);
        fclose(fileCheck);
      }
    }
    fclose(file);
  }
}

void runPayload() {
  FILE *f;
  f = fopen(selectedPayload, "r");
  if (f == nullptr || f == NULL) {
    return;
  }
  char line[8192];
  while (fgets(line, sizeof(line), f)) {
    // Replace newline character with null character
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }
    // Check if line is a comment
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
          delay(2);
          delay(randDelay);
          while (getCaps()) {
            delay(100);
          }
          Keyboard.key_code_raw(code, mod);
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
        Keyboard.key_code_raw(0x28, 0);
      }
      else if (strcmp(line, "GUI") == 0) {
        Keyboard.key_code(0, KEY_LOGO);
      }
      else if (starts_with(line, "GUI ") || starts_with(line, "CTRL ") || starts_with(line, "SHIFT ") || starts_with(line, "ALT ")) {
        modParser(line);
        delay(1);
      }
      else if (starts_with(line, "KEY ")) {
        keyParser(line);
        delay(1);
      }
    }
  }
  fclose(f);
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
    modifiers |= KEY_LOGO;
    modifierLength += 4;
  }
  if (strstr(line, "ALT ") != 0) {
    modifiers |= KEY_ALT;
    modifierLength += 4;
  }
  if (strstr(line, "CTRL ") != 0) {
    modifiers |= KEY_CTRL;
    modifierLength += 5;
  }
  if (strstr(line, "SHIFT ") != 0) {
    modifiers |= KEY_SHIFT;
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
  delay(6);
  Keyboard.key_code_raw(keycode, modifiers);
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
  delay(6);
  Keyboard.key_code_raw(keycode, charToMod(key));
}

int starts_with(const char *line, const char *prefix) {
  if(strncmp(line, prefix, strlen(prefix)) == 0) return 1;
    return 0;
}

int random_number(int min, int max) {
    srand(time(NULL));
    return (rand() % (max - min + 1)) + min;
}