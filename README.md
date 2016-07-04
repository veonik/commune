commune
=======

A silly app with two components: a music playing, LED-driving receiver,
and a transmitter.

Installation
------------

Clone the repository.

```bash
git clone --recursive https://github.com/veonik/commune
```


Building
--------

commune uses platformio for managing builds.


#### Build the transmitter

```bash
PLATFORMIO_BUILD_FLAGS="-DTRANSMITTER" pio run -e uno
```


#### Build the receiver

```bash
PLATFORMIO_BUILD_FLAGS="-DRECEIVER" pio run -e uno
```