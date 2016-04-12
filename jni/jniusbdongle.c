/**
 * \file usbdongle.c
 * \brief this a interface between Java land and C land.
 * \author Johan Boris Iantila
 * \version 0.1
 * \date 21/01/2016
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "command.h"
#include "sms_pdu.h"
#include "com_daloji_dongle_Core.h"



void JNU_ThrowException(JNIEnv *env,jobject jobj,const char* messages){

   jthrowable exc;
   jclass cls = (*env)->GetObjectClass(env, jobj);
   jmethodID mid =(*env)->GetMethodID(env, cls, "jnicallback", "()V");
   
   if (mid == NULL){
	return;
   }
   (*env)->CallVoidMethod(env, jobj, mid);
   exc = (*env)->ExceptionOccurred(env);
   if(exc){
	 jclass newExcCls;
	 (*env)->ExceptionDescribe(env);
	 (*env)->ExceptionClear(env);
     newExcCls = (*env)->FindClass(env,"com/daloji/dongle/exception/InvalidConfigurationException");
    
     if (newExcCls == NULL) {
		return;
	 }
	 
	(*env)->ThrowNew(env, newExcCls, messages);
  }

}

/**
 * \fn Java_com_daloji_dongle_Core_sendSMS
 * \brief  send SMS  using USB Dongle
 *
 * \param env JNI env (default).
 * \param jobj JNI obj (default).
 * \param jnumber destination phone number .
 * \param jmessage  message to send.
 */
JNIEXPORT void JNICALL Java_com_daloji_dongle_Core_sendSMS(JNIEnv *env, jobject jobj, jstring jnumber, jstring jmessage){
   char *phoneNumber = NULL;
   char *messages = NULL;
   jthrowable exception;
   jclass cls = (*env)->GetObjectClass(env, jobj);
   int lengthPhoneNumber = (*env)->GetStringLength(env, jnumber);
   phoneNumber = (char*) calloc(lengthPhoneNumber+1,sizeof(char));
   if(phoneNumber == NULL){
     JNU_ThrowException(env,jobj,"allocation memory failed ");
   }
   (*env)->GetStringUTFRegion(env, jnumber, 0, lengthPhoneNumber, phoneNumber);
   printf("destination phone number : %s \n",phoneNumber);
   int lengthMessage = (*env)->GetStringLength(env, jmessage);
   messages = (char*) calloc(lengthMessage+1,sizeof(char));
   if(messages == NULL){
    JNU_ThrowException(env,jobj,"allocation memory failed ");
   }
   (*env)->GetStringUTFRegion(env, jmessage, 0, lengthMessage, messages);
   printf("Text messages:  %s \n",messages);
   
   t_smscommand *smscommand = createSmsCommand(phoneNumber,messages);
   int rep = create_pdu(smscommand);
   enqueing_sms_pdu(smscommand );
   
   //free_sms_cmd_memory(smscommand);
   free(phoneNumber);
   free(messages);
} 



/**
 * \fn Java_com_daloji_dongle_Core_getBalanceMobileBanking
 * \brief  get balance of mobile banking (SIM card inside USB dongle)
 *
 * \param env JNI env (default).
 * \param jobj JNI obj (default).
 * \param jcommand  command ussd for getting balance this command is implemented by your mobile operator.
 *   you must now the command ussd
 * \return balance of your  mobile account 
 */
JNIEXPORT jfloat JNICALL Java_com_daloji_dongle_Core_getBalanceMobileBanking
  (JNIEnv *env, jobject jobj, jstring jcommand){
	  
	  //not yet implemented
 }


/**
 * \fn JNICALL Java_com_daloji_dongle_Core_transfertMobileBanking
 * \brief  transfert money to another mobile account
 * \param env JNI env (default).
 * \param jobj JNI obj (default).
 * \param jphonenumber destination phone number .
 * \param jamount amount .
 * \param jcommand  command ussd for getting balance this command is implemented by your mobile operator.
 *   you must now the command ussd
 * \return balance of your  mobile account 
 */
JNIEXPORT void JNICALL Java_com_daloji_dongle_Core_transfertMobileBanking
  (JNIEnv *env, jobject jobj, jstring jphonenumber, jfloat jamount, jstring jcommand){
	  
	  	  //not yet implemented
	  
 }
  
  


