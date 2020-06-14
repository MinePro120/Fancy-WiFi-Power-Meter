# Fancy Wi-Fi Power Meter
<p align=center>
<a target="_blank" href="https://www.gnu.org/licenses/gpl-3.0.en.html" title="License: GPL v3">
<img src="https://img.shields.io/badge/License:-GPL%20v3-darkred.svg">
</a>

<img src="https://i.imgur.com/7KC3SzI.png"> 
</p>
Fancy Wi-Fi Power Meter is a fancy cli tool to measure the received signal power (or RSSI) of a wireless  (Wi-Fi)  network you are connected to.

## Dependencies
It should work out of the box in most GNU/Linux distributions, but here are the dependencies :
* cat
* cut
* iwgetid
* grep

## Compile
``` bash
make
```
## Usage
Inside the directory of the binary, simply :
``` bash
wsignal [WIRELESS INTERFACE]
```
## License
The software provided above is licensed under the [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.en.html).
