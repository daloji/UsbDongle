package com.daloji.dongle;

import com.daloji.dongle.InvalidConfigurationException;

/**
 *   this class manager all command  for USB Dongle
 * 
 * @author Johan Boris IANTILA
 * @since USBDongle 1.0
 * @see UsbDongle
 *
 */

public class UsbDongle {

	private Core core;

	public UsbDongle(){
		core= new Core();
	}


	/**
	 * method that allow you to send SMS 
	 * @param number  destination phone number
	 * @param messages text message 
	 * 
	 */
	public void sendSmS(String number,String messages){
		if(core !=null){
			try {
				core.sendSMS(number, messages);
			} catch (InvalidConfigurationException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	/**
	 * method that allow you to send SMS 
	 * @param command   ussd  (check your operator phone command)
	 * @return  balance value
	 * 
	 */
	public float getBalanceMobileBanking(String command){
		float balance = 0;
		if(core !=null){
			try {
				balance = core.getBalanceMobileBanking(command);
			}catch (InvalidConfigurationException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return balance;
	}

	/**
	 * transfet money to another mobile money account
	 * @param phone number Destination phone number
	 * @param amount value to send
	 * @param command   ussd  (check your operator phone command
	 */
	public void transfertMobileBanking(String phonenumber,float amount,String command){
		if(core !=null){
			try {
				core.transfertMobileBanking(phonenumber,amount,command);
			}catch (InvalidConfigurationException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}
	}
}
