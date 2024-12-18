This software is licensed under the Personal Use License (PUL).

- You may use this software for personal, non-commercial purposes.
- Redistribution, modification, and commercial use are not permitted.

# Bluetooth badusb
Only supports the Swedish and the US keyboard layouts at the moment but feel free to add more

Thank you Earle Philhower for the arduino-pico core for the rp2040 boards, without you this wouldn't be possible

## Before you begin, make sure you have a...
1. Raspberry Pi Pico W and not a regular Raspberry Pi Pico.
2. Computer so that you can flash the firmware file.
3. Micro-usb cable that can transmit and receive data.

## How to install
### 1. Hold the BOOTSEL button on your Raspberry Pi Pico W and plug it in to your computer.
![Alt text](https://www.petecodes.co.uk/wp-content/uploads/2022/07/20220706_1206182-5.jpg " ")
### 2. Release the BOOTSEL button and make sure that a storage device with the name RPI-RP2 has been connected. If you don't see a storage device it may be due to faulty drivers or most likely because you're using a cable that doesn't transmit data.
![Alt text](https://startingelectronics.org/tutorials/raspberry-PI/easiest-way-to-start-using-a-raspberry-PI-pico/pico-folder-windows.jpg " ")
### 3. Upload the bluetooth-badusb.uf2 file to the RPI-RP2 drive by dragging and dropping the file.
### 4. You're done!
## Usage
### 1. Plug the Pico into your computer and wait for it to show up as a mass storage device.
### 2. Create following files: *settings.txt, payload1.txt*
### 3. Type the following text in *settings.txt*
```
MIN 0
MAX 1
PAYLOAD payload1.txt
NAME bluetooth-badusb
BLUETOOTH 1
LANGUAGE US 
```
### 4. MIN is the shortest amount of time a keypress can take. MAX is the longest amount of time a keypress can take. Between every keypress there is a random delay between MIN and MAX. PAYLOAD is the payload that will be executed. NAME is what the badusb will show up as in a bluetooth menu. BLUETOOTH is whether it should connect via bluetooth or USB. LANGUAGE sets the layout to US, SE for Swedish is also supported.
### 5. Type the following text in *payload1.txt*
```
STRING 123
DELAY 500
CTRL ALT DELETE
KEY DOWN
KEY DOWN
KEY DOWN
ENTER
DELAY 1000
GUI
DELAY 100
GUI r
```
### 6. You can either create your own scripts or you can find scripts on the internet. Beware that many scripts have different syntax than this version of duckyscript. You may need to modify some parts.
### 7. When you have connected to the badusb via bluetooth you only have to press capslock twice for it to start typing. When it is done executing your script you can press capslock twice again and it will retype your script.
### 8. You can also send commands through serial, execute files with FILE <filename> or send individual commands.

## Common problems
### My files aren't being saved.
You may need to eject the badusb storage device to save the files you created/modified. On most operating systems you right-click the storage device and press the eject button. 
## Compilation process
1. Download the Arduino IDE
2. Install the Earle Philhower arduino-pico core (https://github.com/earlephilhower/arduino-pico?tab=readme-ov-file#installation)
3. Navigate to the "Tools" tab
4. Select the Raspberry Pi Pico W board
5. Select the "IPv4 + IPv6 + Bluetooth" option under "IP/Bluetooth Stack"
6. Select the "2MB (Sketch: 1MB, FS: 1MB)" option under "Flash Size"
7. Either upload directly to your Pico with the upload button or press the "Export Compiled Binary" button under the "Sketch" tab

## Script syntax
### STRING \<text\> - Types text. Maximum of 8192 bytes per line, beware of multibyte chars.
### DELAY \<time\> - Waits <time> milliseconds before next command.
### ENTER - Presses the enter key
### GUI - Presses the windows key
### CTRL, ALT, GUI, SHIFT \<key\> - Modifier keys. Can be used in series like "CTRL ALT DELETE" (DELETE is not a modifier, it is a key.). Order does not matter. 
### KEY \<character\> - Presses key associated with character and releases the key afterwards. Any modifier associated with the specified character is ignored. Check the keymap in the source code if you are unsure of the name of a key.
### FILE \<filename\> - Executes specified file that is on the flash.
