# Fancy Wi-Fi Power Meter
<p align=left>
<a target="_blank" href="https://www.gnu.org/licenses/gpl-3.0.en.html" title="License: GPL v3">
<img src="https://img.shields.io/badge/License:-GPL%20v3-darkred.svg">
</a>
<br>
<img src="https://i.imgur.com/HSFUo6M.png"> 
</p>
Fancy Wi-Fi Power Meter is a fancy cli tool to measure the received signal power (RSSI) of a wireless  (Wi-Fi)  network you are connected to.

## Dependencies
It should work out of the box in most GNU/Linux distributions, but here are the dependencies :
* cat
* cut
* grep
* iwgetid
* ping

## Build
``` bash
make
```
## Usage
Inside the directory of the binary, simply :
``` bash
./wpower [WIRELESS INTERFACE]
```
## License
The software provided above is licensed under the [GNU GPL v3](https://www.gnu.org/licenses/gpl-3.0.en.html).
