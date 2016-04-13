package com.daloji.dongle;

import com.daloji.dongle.InvalidConfigurationException;

/**
 *  Helper for loading shared library and native methodse
 * 
 * @author Johan Boris IANTILA
 * @since USBDongle 1.0
 * @see SystemLoadCustom
 *
 */
public class Core {
	
	public native void sendSMS(String phonenumber,String messages) throws InvalidConfigurationException;

	public native float getBalanceMobileBanking(String command);
	
	public native void transfertMobileBanking(String phonenumber,float amount,String command);
	
	private void jnicallback() throws InvalidConfigurationException{
		throw new InvalidConfigurationException();
	}
	static {
		
			try {
				SystemLoadCustom.loadLibrary("lib/libUsbDongle");
			} catch (InvalidConfigurationException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		
	}
}
