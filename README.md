# ESP8266-remote-display
ESP8266 remote tft display using telnet protocol.

I had 2.8 tft display connected to wemos d1 in my living room displaying temperature and time. It's working ok but I wanted to modify some things and solution is to modify code upload to wemos and.... I got idea that instead of polling some data from my rpi I could draw everything on Rpi and push that to ESP8266 and display it. ESP8266 listenes on port 23 and receives comma separated commands. Rpi sends commands using python and telnetlib.
