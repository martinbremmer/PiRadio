# Setup

Setup Pi by following the steps of:  
https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up  
Differences:  
Get Raspberry Pi Imager by: sudo snap install rpi-imager  
I didn't wait until I had WiFi connection. So I had to upgrade manually, following [this guide](https://www.raspberrypi.org/documentation/raspbian/updating.md).  

I used 'Menu->Preferences->Add / Remove Software' to add software. But 'apt install' would probably have been easier (especially for a guide).  
The following software was installed.
* qtcreator (possibly only qmake like stuff necessary, but this is what I'm somewhat used to).
* gedit (my preferred 'simple' editor).
* vim (my preferred command line editor).
* Qt 5 declarative development files
* qt5-default
* qt5-qmake (needed to build (at least from qtcreator)).
* libvlc development files
* libmusicbrainz5 development files
* libcoverart development files


