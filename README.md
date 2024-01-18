# LCD_Project
IT is a ESP-IDF project that interfaces the ILI9486 display(380x480). The display had 8-bit parallel interface with additionand 5 helper pins. It uses LVGL library for creating extraordinary graphics.
The TFT library used in this project is modified version of github repository available [here](https://github.com/nopnop2002/esp-idf-parallel-tft) for ILI9486 and many other LCD displays with different interface scemes.



## Features
The features of this LCD project includes:
1. ILI9486 8-bit parallel display(320x480) interface
2. [LVGLv8.3](https://github.com/lvgl/lvgl/tree/release/v8.3) for graphics ([docs](https://docs.lvgl.io/8.3/))
3. Supports ESP32 and ESP32S3 as micontroller

#### NOTE
**ESP32S3 is untested for now**

## Pins assigned
#### Default pins for ESP32
| LCD Pins          | Description   
| :---              |    :----:     
| D0                |12           
| D1                |13           
| D2                |26           
| D3                |25           
| D4                |18           
| D5                |19           
| D6                |27           
| D7                |14            
| RD                |2            
| WR                |4            
| RS                |15           
| CS                |33           
| RESET             |32               


#### Default pins for ESP32S3
TODO

### NOTE
**It is required to look at the pins and their functionalities in datasheet for specific microcontrollers used as not all the pins can be used; for example, some are used as input only, some are used for SPI flash. Assiging those pins for LCD interface cause problems.**

