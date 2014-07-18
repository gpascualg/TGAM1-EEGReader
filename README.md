TGAM1-EEGReader
===============

Two arduino boards, wirelessly communicating via bluetooth, are able to get EEG waves and data from a TGAM1 powered device


##Components
A list of components for both parts is exposed below

###Transmitter (EEG Reader)
* Mindflex headset or some other device using a TGAM1 microchip.
* HC-05 Bluetooth module
* Arduino board (any)

###Receiver
* HC-05 Blueetooth module
* Arduino board 
 
_Note that receiver is optional and a PC/laptop with bluetooth would suffy_


##Necessary arduino core code modifications
Arduino lacks of propper object polymorphism along HardwareSerial and SoftwareSerial. Both inherit from Stream, but Stream does *not* support _begin_ (among others), which renders this library completely useless.

In order to solve this, a simple new base class has been added. Using code on [8] and [9] as base, a new class called **_AbstractSerial_** is parent of both Hardware and Software serials.

###Definitions
* ``ARDUINO_HOME`` = The folder where arduino ide/code was installed to
* ``CORE_FOLDER`` = *ARDUINO_HOME/hardware/arduino/cores/arduino*
* ``LIBRARY_FOLDER`` = *ARDUINO_HOME/libraries*

If you don't know what git, diffs and patchs are, go to *Modifications - Manual*. Be warned though that this may break arduino core code if used with other version than **1.0.5-r2**. 

Otherwise, head up to *Modifications - Patching*

###Modifications - Manual
* Copy ``Modifications/AbstractSerial.h`` to arduino ``CORE_FOLDER``
* Copy ``Modifications/HardwareSerial.h`` to arduino ``CORE_FOLDER``
* Copy ``Modifications/SoftwareSerial.h`` to arduino ``LIBRARY_FOLDER/SoftwareSerial``


###Modifications - Patching
Excecute on ``ARDUINO_HOME`` via git console: 
```
git apply --ignore-whitespace core_modifications.patch
```
If no errors pop, you are done.


##Acknowledgements
A complete list of sources and articles used to develop the device

* [1] http://frontiernerds.com/brain-hack - Eric Mika's work on Mindflex hacking
* [2] https://github.com/kitschpatrol/Brain - Brain library for arduino to parse TGAM1 packets
* [3] http://developer.neurosky.com/docs/doku.php?id=thinkgear_communications_protocol - Neurosky (TGAM1) documentation
* [4] http://wearcam.org/ece1766/neurosky_eeg_brainwave_chip_and_board_tgam1.pdf - TGAM1 Datasheet
* [5] http://morloklabs.blogspot.com.es/2011/12/mindflex-duel-hacking-conversion-50hz-y.html - RAW/Normal mode hard switch for TGAM1
* [6] https://github.com/pcantin/bt2bt_HCa - Phillipe Cantin's code on configuring HC-05
* [7] http://wannabe-programmer.blogspot.com.es/2013/05/entrar-en-at-mode-de-un-hc-05-hc-06-con.html - HC-05 configuring
* [8] https://code.google.com/p/arduino/issues/detail?id=570 - Initial AbstractSerial class
* [9] https://code.google.com/p/arduino/issues/detail?id=60 - Outdated Serial base class
