# Bluetooth badusb
Only supports the swedish keyboard layout at the moment but feel free to add more

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
```
### 4. MIN is the shortest amount of time a keypress can take. MAX is the longest amount of time a keypress can take. Between every keypress there is a random delay between MIN and MAX. PAYLOAD is the payload that will be executed. NAME is what the badusb will show up as in a bluetooth menu.
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
### 6. You can either create your own scripts or you can look up scripts on the internet. Beware that many scripts have different syntax than this version of duckyscript. You may need to modify some parts.
### 7. When you have connected to the badusb via bluetooth you only have to press capslock twice for it to start typing. When it is done executing your script you can press capslock twice again and it will retype your script.
