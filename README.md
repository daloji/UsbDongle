# UsbDongle
UsBDongle is a JAVA  API  that allow  you to send sms and manage your mobile banking account and ussd command

Warning 
This  is in alpha stage.
I am not responsible if this apps  will eat your money on
your SIM card or do any unpredicted things

Please use Linux architecture recommended. For this moment, you can use only one dongle USB at the same time 
* Windows is not supported right now  .

This apps  work with the folowing UMTS cards:
* Huawei but only (E1752 and E1552) verified
* ZTE

Before using this  make sure to:
* Disable PIN code on your SIM card


Supported features:
* Send SMS and receive SMS

next features:
* mobile banking money (send and receive)

Here is an example:


import com.daloji.dongle.SmsDongle;

SmsDongle sms = new SmsDongle();
sms.sendSmS("+3312345678","message depuis une cle 3G ");
