#ofxMH871

This openFrameworks addon drives the MH871-MKII Pen Plotter and Vinyl Cutter.  The MH871-MKII supports the HPGL (Hewlett-Packard Graphics Language) and uses a limited subset of the HPGL standard: 

  1. Initialize - IN;
  2. Pen Up command - PU
  2. Pen Down command - PD
  3. Pen Absolute to an X,Y point - PAX,Y

## HOW TO SETUP 

### Connecting to the device

#### Connect to the MH871-MKII Pen Plotter via the COM port.  This will require a USB to Serial device.  

![COM PORT](/images/IMG_1608.JPG)
![USB To Serial](/images/IMG_1609.JPG)

#### Power on the Plotter

![POWER](/images/IMG_1610.JPG)

#### Press the Setup Button

![SELECT](/images/IMG_1624.JPG)

#### Change the Serial BaudRate to 19200

![BaudRate](/images/IMG_1625.JPG)

#### On your Mac or Linux Terminal

'''bash$ ls /dev/tty.*'''

####  Copy the name for your USB to Serial Device for when you setup the ofxMH871 object

'''setup(string serialPort)'''

