# ARC team 5 vending machine

## Table of Content
1. [Introduction] (#introduction)
2. [Hardware Connection] (#hardware)
3. [Software] (#software)
4. [User Manual] (#user_manual)

## Introduction <div id='introduction'/>
We use ARC as our user terminal and connect to the server, which is implemented with cloud data management and smart selling system. Peripheral devices and sensors are added to the ARC exp. WiFi, temperature sensor, OLED and many more.

Our vending machine system provides maintainance issue related to vending machine, such as auto-checking for food expired date, temperature overheat detecting. Other than that, vending machine is connected to database which users are allow to use their id for payment without physical bills or coins, and the machine will recommend what to buy depending on the purchase records of that user. The reason why our machine is different from others, the system administrator can get the information by signing in far away from the vending machine; the costomer can pay cashlessly. With e-payment, according to the purchase record of customer or other information, our machine can easily give some recommand after some operation, which can effectively decrease the time for customer making decision and interact with interest.

## hardware connection
### EMSK Board firmware used in this application
* emsk ver2.2

### The peripheral devices used in this application

| peripheral       | module  |
| :--------------: | ------: |
| Wi-Fi            | EMW3162 |
| OLED             | SSD1306 |
| temperture       | adt7420 |
| DC motor	       | 9V      |
| Infrared ray led | 940nm   |

### The hardware setup

| port | interface | peripheral   |
| :--: | :-------: | :----------: |
| J1   | GPIO      | DC motor     |
| J2   | I2C       | temperature  |
| J3   | GPIO      | Number pad   |
| J4   | GPIO      | Infrared ray |
| J5   | SPI       | OLED         |
| J6   | SPI       | Wi-Fi        |

## Software
sofware requirement
* Visual Studio code IDE
* ARC GNU Toolchain
* serial port terminal eg. putty

### Makefile settings
Target options about EMSK and toolchain:

BOARD ?= emsk
BD_VER ?= 22
CUR_CORE ?= arcem7d
TOOLCHAIN ?= gnu

## User Manual
### Before Running This Application
Firstly, download source code of vending machine from GitHub.
* Source code download link: https://github.com/cstandy/ARC_team_5_vending_machine.git

Then you need to register a new user account from our website 
* register link: https://vendingsmachine.caslab.ee.ncku.edu.tw/sign_up.php

### Moving Directory
Move the directories ```ARC_VSCODE``` and ```embarc_osp-master``` under ```D:/```

### Launch VSCode IDE

```D:/ARC_VSCODE/StartVSCode.bat```
Launch StartVSCode.bat to run this vending machine application. After launching VScode, right click on the workspace and choose add Folder in the path ```D:/ARC_VSCODE/vending-machine``` to workspace. 

### Wi-Fi hotspot connnection settings
Modify the ssid and password of Wi-Fi AP in ``` D:/embarc_osp-master/board/emsk/emsk.h``` to connect Wi-Fi module to the hotspot

```clike=137
#define WF_HOTSPOT_NAME             "EMBARC"
#define WF_HOTSPOT_PASSWD           "12345678"
```

### Entering Main
After Wi-Fi connected, FREERTOS rises all tasks including communication, oled, temperature, blinky, number pad and DC motor.

## Source code directory

* main function directory ```src\arc_osp-master\middleware\vending-machine```
	* Include
	* Source
	* Readme.md

* ```src\arc_osp-master\middleware\vending-machine\Source```

| FILE                 | description            |
| -------------------: | :--------------------- |
| vm_task.c            | main queue             |
| communication_task.c | mbedtls ssl client     |
| oled_task.c          | oled display           |
| adt7420.c            | temperature sensor     |
| dcmotor_task.c       | dc_motor controller    |
| numpad_task.c        | user input             |
| LED_task.c           | make the board blinky  |
| FreeRTOSConfig.c     | Freertos configuration |




