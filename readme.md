![OSP](extras/osp.jpg)
# Open System Protocol (OSP) on Arduino

Welcome to the landing page of the 
**aolibs** (short for Arduino OSP libraries from ams-OSRAM) to be used 
with the **Arduino OSP evaluation kit**.


## Introduction

OSP or Open System Protocol was developed by ams OSRAM for dynamic lighting,
for example in car interiors. OSP is open for everybody and free of license. 

An OSP based system consist of an MCU (the "RootMCU") connected to a series 
of (up to 1000) daisy chained OSP nodes. OSP nodes contain or drive RGB 
modules, or implement gateways to other protocols like I2C. The SAID or
Stand Alone Intelligent Driver, officially known as AS1163 is an example of
an OSP node withe external LEDs and integrated I2C bridge. The E3731i
or Intelligent RGB (RGBI) is an example of an OSP node with integrated red, 
green and blue LED.

![OSP chain](extras/ospchain.jpg)


ams OSRAM sells an evaluation kit demonstrating OSP. This kit is known as 
the _Arduino OSP evaluation kit_. With this evaluation kit comes 
software, which is partitioned over several libraries (the _aolibs_), 
each with its own GitHub repository. This document is part of one of those
libraries (_aotop_). However, _aotop_ is not really a library, but rather a 
container of top level demo applications (which use the real libraries),
and a container of top-level documentation (like this readme). The diagram
below shows an overview of all _aolibs_.

![aolibs in context](extras/aolibs.drawio.png)

All _aolibs_ (including _aotop_) are registered as Arduino libraries.
Library _aotop_ is registered as an Arduino library which is dependent 
on all others. 
Use the Library Manager of the Arduino IDE to search for _aotop_ 
("OSP ToplevelSketches aotop"), install it and all others will 
also be installed.


There is a dedicated [Getting started manual](gettingstarted.md).
It describes what hard and software is needed (the evaluation kit and 
the libraries), and how to turn on the first RGBs.


## Links

This section links to various resources.

- Open System Protocol [website](https://ams-osram.com/technology/open-system-protocol).

- OSP products

  - RGBi [E3731i](https://ams-osram.com/products/leds/multi-color-leds/osram-osire-e3731i-krtbi-d2lm31-31) product page (OSP node with built-in RGB LEDs).
  - SAID [AS1163](https://ams-osram.com/news/blog/as1163-said-led-driver-connects-any-led-to-osp-automotive-lighting-network) product page (OSP node with 9 LED drivers) (todo:replace URL).
  - (todo: add SAID2)

- [Getting started manual](gettingstarted.md) for the 
  _aolibs_ with the _Arduino OSP evaluation kit_.

- [Arduino OSP evaluation kit](todo) (todo:add URL). The kit contains:

  - 1× OPS32 ([schematics](extras/schematics/OSP32_complete_v9.pdf));
  - 1× SAIDbasic ([schematics](extras/schematics/SAIDbasic_complete_v7.1.pdf));
  - 1× SAIDlooker ([schematics](extras/schematics/SAIDLooker_complete_v3.1.pdf));
  - 1× RGBIstrip (todo:add URL);
  - 2× terminator ([schematics](extras/schematics/Terminator_complete_v1.2.pdf));
  - 2× CAN adapter ([schematics](extras/schematics/CANadapter_complete_v3.2.pdf));
  - 2× EEPROM stick ([schematics](extras/schematics/I2CEEPROMstick-schematics_v1.0.pdf));
  - 3× ERNI cable ([supplier](https://www.distrelec.nl/en/ribbon-cable-27mm-cores-200mm-black-erni-839017/p/14362654)).
  
- The [aolibs](https://github.com/orgs/ams-OSRAM/repositories?q=OSP_ao)
  (on the ams OSRAM GitHub [site](https://github.com/ams-OSRAM/)): 

  - [aotop](https://github.com/ams-OSRAM/OSP_aotop) (this repo) with _Top level sketches_;
  - [aoapps](https://github.com/ams-OSRAM/OSP_aoapps) library with _Reusable apps_;
  - [aoui32](https://github.com/ams-OSRAM/OSP_aoui32) library with _UI drivers for OSP32 board_;
  - [aomw](https://github.com/ams-OSRAM/OSP_aomw) library with _Middleware_;
  - [aocmd](https://github.com/ams-OSRAM/OSP_aocmd) library with _Command interpreter_;
  - [aoosp](https://github.com/ams-OSRAM/OSP_aoosp) library with _Telegrams_;
  - [aospi](https://github.com/ams-OSRAM/OSP_aospi) library with _2-wire SPI_;
  - [aoresult](https://github.com/ams-OSRAM/OSP_aoresult) library with _Result codes_.

  
(end of top-level documentation)

-----------------------------------------------------------------------------
-----------------------------------------------------------------------------

(begin of library specific documentation)


# OSP ToplevelSketches aotop

Library "OSP ToplevelSketches aotop", usually abbreviated to "aotop", 
is one of the **aolibs**; short for Arduino OSP libraries from ams-OSRAM.
This suite implements support for chips that use the Open System Protocol, 
like the AS1163 ("SAID") or the OSIRE E3731i ("RGBi").
The landing page for the _aolibs_ is on 
[GitHub](https://github.com/ams-OSRAM/OSP_aotop).


## Introduction

Library _aotop_ is at the top of the dependency graph.
It contains no (library) code; but it serves several other functions
- It contains top-level sketches for advanced demos 
  (using the Arduino IDE examples mechanism).
- It is made dependent on all Open System Protocol libraries (aolibs),
  so installing _aotop_ via the Arduino IDE will install all other 
  libraries as well.
- It is the "landing" place for the Open System Protocol software,
  providing global documentation with links to all related resources.

![aotop in context](extras/aolibs-aotop.drawio.png)


## Examples

The examples in _aotop_ are not examples in the Arduino IDE sense 
(explaining a feature), rather they are full-fledged (demo) applications
You can find them in the Arduino IDE via 
File > Examples > OSP ToplevelSketches aotop > ...


- **saidbasic** ([source](examples/saidbasic))  
  This application assumes a SAIDbasic board is connected to the OSP32 board.
  It shows several features of the SAID, which can be selected via the 
  (A,X and Y) button on the OSP32 board.

- **eepromflasher** ([source](examples/eepromflasher))  
  One of the features of the SAIDbasic application is to play (LED animation)
  scripts from an EEPROM. There are EEPROMs on the OSP32 board, the SAIDbasic
  board, and on stand-alone I2C EEPROM sticks. The eepromflasher application
  allows writing any script (stock or user developed) to any of these EEPROMs.

- **osplink** ([source](examples/osplink))  
  This application allows the PC (with a terminal like the Arduino Serial Monitor)
  to send and receive OSP telegrams, using serial-over-USB.
  
  There is an experimental Python app for PC, that sends commands to the 
  OSPlink application. Please find it in 
  [`aocmd/python`](https://github.com/ams-OSRAM/OSP_aocmd/tree/main/python).


## API

This library does not contain (reusable library) code, so there is no API.


## Version history _aotop_

- **2024 October 8, 0.1.0**  
  - Fixed name to `Open System Protocol (OSP) on Arduino` and `aolibs`.
  - Moved domain from `github.com/ams-OSRAM-Group` to `github.com/ams-OSRAM`.
  - Added user manual (ppt) to `saidbasic.ino`.
  - Changed `eepromflasher.ino` to _compare_ SAIDbasic eeprom to rainbow.
  - Adapted to `aoosp_exec_resetinit` not needing `last` and `loop`.
  - Added links in `readme.md` for all example sketches.
  - Updated `ospchain.jpg`.
  - Extended `gettingstarted.md`.
  - Added minimal documentation to top of `saidbasic.ino`.
  - Corrected link to all GitHub repos from aoxxx to OSP_aoxxx.
  - Remove "oalib" from `sentence=` in `library.properties`.
  - Updated `readme.md`.
  - Arduino name changed from `OSP Top level sketches - aotop` to `OSP ToplevelSketches aotop`.
  - `license.txt` line endings changed from LF to CR+LF.
  - Reorder libs in `aocmd_version_extra()` in `saidbasic.ino`.
  - Added `extra\schematics\xxx.pdf` of all boards in the evaluation kit.
  
- **2024 July 2, 0.0.1**  
  - Initial release candidate.


(end)
