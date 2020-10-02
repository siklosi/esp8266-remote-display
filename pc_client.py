#! /usr/bin/python3
import telnetlib
import time
import datetime
deviceip="192.168.0.181"
tn = telnetlib.Telnet(deviceip)

def setOrientation(orientation): #/0=0deg,1=90deg,2=180deg,3=270deg tft screen rotation
  tn.write(b"1,%d\r" % (orientation))  
def dText(sText,xPos,yPos,font_type,red,green,blue):
    tn.write(b"2,%s,%d,%d,%d,%d,%d,%d\r" % (bytes(sText,'ascii'),xPos,yPos,font_type,red,green,blue))
def drawRect(xPos,yPos,width,height,cornerRadius,red,green,blue):
    tn.write(b"3,%d,%d,%d,%d,%d,%d,%d,%d\r" % (xPos,yPos,width,height,cornerRadius,red,green,blue))
def fillRect(xPos,yPos,width,height,cornerRadius,red,green,blue):
    tn.write(b"4,%d,%d,%d,%d,%d,%d,%d,%d\r" % (xPos,yPos,width,height,cornerRadius,red,green,blue))
def drawLine(xPos,yPos,width,height,red,green,blue):
    tn.write(b"5,%d,%d,%d,%d,%d,%d,%d\r" % (xPos,yPos,width,height,red,green,blue))
def drawCircle(xPos,yPos,ra,red,green,blue):
    tn.write(b"6,%d,%d,%d,%d,%d,%d,%d,%d\r" % (xPos,yPos,ra,red,green,blue))
def fillCircle(xPos,yPos,radius,red,green,blue):
    tn.write(b"7,%d,%d,%d,%d,%d,%d,%d,%d\r" % (xPos,yPos,radius,red,green,blue))
def backgroundColor(red,green,blue):
    tn.write(b"8,%d,%d,%d\r" % (red,green,blue))
    #time.sleep(.1) #tft needs some time to fill display with color
def setBrightnest(brightnestValue):
     tn.write(b"9,%d\r" % (brightnestValue))


#setOrientation(1)
#backgroundColor(0,0,0)

fillRect(30,10,250,75,0,0,0,0)
dText(datetime.datetime.now().strftime('%H:%M'),30,10,8,100,100,100)