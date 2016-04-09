package com.daloji.dongle;

public class Core {
	
	public native void sendSMS(String number,String messages);

	static {
	      System.loadLibrary("libUsbDongle");
	}
}
