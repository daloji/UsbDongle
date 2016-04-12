# UsbDongle
UsBDongle is a JAVA  API  allow  you to send SMS and manage your mobile banking account and USSD command

Warning 
This  is in alpha stage.
I am not responsible if this apps  will eat your money on
your SIM card or do any unpredicted things


Supported features:
* Send SMS and receive SMS

next features:
* transfert money by mobile banking money 
* get mobile banking money balance 


#Requirement
Linux architecture is mandatory.For this moment, you can use only one dongle USB at the same time 
* Windows is not supported right now  .

This Api  work with the folowing UMTS cards:
* Huawei but only (E1752 and E1552) verified
* ZTE

Before using this  make sure to:
* Disable PIN code on your SIM card

Building
--------

UsBDongle Api contains both Java and C code,your variable JAVA_HOME must be set. this Api  is built with Makefile
To build Java and static library, just run the following
commands: 

    $ make all

after build you can find  usbDongle.jar inside  jar directory

Using UsbDongle API
--------
Here is an example:


     import com.daloji.dongle.SmsDongle;
     
    SmsDongle sms = new SmsDongle(); 
    sms.sendSmS("+3312345678","message depuis une cle 3G ");

- you can have this kind of problem
  * /dev/ttyUSB0: Permission non accordée (for french language)
  * /dev/ttyUSB0: Permission denied
  
This is because the device file of the serial port does not have permissions to allow to currently logged in user to "read" or "write" to the serial device
 add the user to the “dialout” supplementary group for fix it 
 ```
   $ sudo usermod -a -G dialout  yourUsername
   $sudo chmod a+rw /dev/ttyUSB0
  ```
