package com.daloji.dongle;

public class SmsDongle {

	private Core core;
   	
	public SmsDongle(){
		core= new Core();
	}

	

	public void sendSmS(String number,String messages){
		if(core !=null){
			core.sendSMS(number, messages);
		}
	}
}
