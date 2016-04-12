/**
 * \file command.c
 * \brief bluid all command ussd and sms.
 * \author Johan Boris Iantila
 * \version 0.1
 * \date 21/01/2016
 *
 *
 */

#include <string.h>			
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "command.h"
#include <termios.h>  

static char *USB_TTY = "/dev/ttyUSB0";

/**
 * \fn createSmsCommand 
 * \brief  create sms structure with destination phone number and text message
 * \param phoneNumber destination phone number.
 * \param message text message
 * \return smscommand structure 
 **/
t_smscommand *createSmsCommand(const char* phoneNumber, const char* message){
	t_smscommand *smscommand = NULL;
	if(phoneNumber == NULL || message == NULL){
		return smscommand;
	}
	smscommand = (t_smscommand *)calloc(1,sizeof(t_smscommand));
	if (smscommand == NULL){
      fprintf(stderr,"No Memory while allocating \%d at %s \n",__LINE__,__FILE__);
      return NULL;
    }
    //buffer 2048 octet
    smscommand->pduBuffer = (char *)calloc(BUFFER_SIZE,sizeof(char)); 
    //copy phone number
    smscommand->phoneNumber = (char *)calloc(strlen(phoneNumber)+1,sizeof(char));
    memcpy(smscommand->phoneNumber,phoneNumber,strlen(phoneNumber)+1);
    //copy message
    smscommand->messages = (char *)calloc(strlen(message)+1,sizeof(char));
    memcpy(smscommand->messages,message,strlen(message)+1);
    
    return smscommand;
}

/**
 * \fn enqueing_sms_pdu 
 * \brief  forge pdu and send command AT to serial port
 * \param smscommand 
 * \return 
 **/
int enqueing_sms_pdu(t_smscommand *smscommand ){
	
	char * ptr = (char *) smscommand->pduBuffer;
	char buf[8+25+1];
	
	at_queue_cmd_t at_cmd[] = {
		{ CMD_AT_CMGS,    RES_SMS_PROMPT, ATQ_CMD_FLAG_DEFAULT, { ATQ_CMD_TIMEOUT_2S, 0}  , NULL, 0 },
		{ CMD_AT_SMSTEXT, RES_OK,         ATQ_CMD_FLAG_DEFAULT, { ATQ_CMD_TIMEOUT_40S, 0} , NULL, 0 }
		};
	
	
	size_t length = strlen(smscommand->pduBuffer);
	size_t pdulen = length;
	
	int scalen = pdu_parse_sca(&ptr, &pdulen);

	if(scalen < 2 || length % 2 != 0)
	{
		return -EINVAL;
	}

	at_cmd[1].data = (char *) calloc(length + 2,sizeof(char));
	if(!at_cmd[1].data)
	{
		return -ENOMEM;
	}

	at_cmd[1].length = length + 1;

	memcpy(at_cmd[1].data, smscommand->pduBuffer, length);
	at_cmd[1].data[length] = 0x1A;
	at_cmd[1].data[length+1] = 0x0;

	at_cmd[0].length = snprintf(buf, sizeof(buf), "AT+CMGS=%d\r", (int)(pdulen / 2));
	
	
	at_cmd[0].data =(char *) calloc(strlen(buf) + 1,sizeof(char)); 
	memcpy(at_cmd[0].data,buf,strlen(buf)+1);
	
	if(!at_cmd[0].data)
	{
		free(at_cmd[1].data);
		return -ENOMEM;
	}
   
   int  fds=openttyUSB(USB_TTY);
   
   printf("command AT  %s",at_cmd[0].data);
   sendcommand(fds, at_cmd[0].data);
   sleep(4);
   printf("command pdu  %s",at_cmd[1].data);
   sendcommand(fds, at_cmd[1].data);
   
   //release all memory
   free(at_cmd[0].data);
   free(at_cmd[1].data);
}


/**
 * \fn sendcommand 
 * \brief send command AT to serial port
 * \param fd pointer to serial port
 * \param command to send
 * \return 
 **/
void sendcommand(int fd, const char *buf){

   ssize_t out_count;
	size_t total = 0;
	unsigned errs = 10;
    size_t count =strlen(buf);

	while (count > 0)
	{
		out_count = write (fd, buf, count);
		if (out_count <= 0)
		{
			if(errno == EINTR || errno == EAGAIN)
			{
				errs--;
				if(errs != 0)
					continue;
			}
			break;
		}
		errs = 10;
		count -= out_count;
		buf += out_count;
		total += out_count;
	}
}


/**
 * \fn free_sms_cmd_memory 
 * \brief release all memory allocate
 * \param smscommand to send
 **/
void free_sms_cmd_memory(t_smscommand *smscommand){
	if(smscommand == NULL) 
	    return;
   if(smscommand->messages)
		free(smscommand->messages);
  
  if(smscommand->phoneNumber)
		free(smscommand->phoneNumber);
		
   if(smscommand->pduBuffer)
		free(smscommand->pduBuffer);
	//free(smscommand);
}



/**
 * \fn openttyUSB 
 * \brief  open serial port /dev/ttyUSB*
 * \param dev name of device port.
 * \return  device pointer
 **/
int openttyUSB (const char* dev){
	int		flags;
	int		pid;
	int		fd;
	struct termios	term_attr;
	char		buf[40];

	fd = open (dev, O_RDWR | O_NOCTTY);
	if (fd < 0)
	{
		flags = errno;
		snprintf(buf, sizeof(buf), "Open Failed\r\nErrorCode: %d", flags);
		printf("unable to open %s: %s\n", dev, strerror(flags));
		return -1;
	}

	flags = fcntl(fd, F_GETFD);
	if(flags == -1 || fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
	{
		flags = errno;
		printf("fcntl(F_GETFD/F_SETFD) failed for %s: %s\n", dev, strerror(flags));
		return -1;
	}

	if (tcgetattr (fd, &term_attr) != 0)
	{
		flags = errno;
		printf( "tcgetattr() failed for %s: %s\n", dev, strerror(flags));
		return -1;
	}

	term_attr.c_cflag = B115200 | CS8 | CREAD | CRTSCTS;
	term_attr.c_iflag = 0;
	term_attr.c_oflag = 0;
	term_attr.c_lflag = 0;
	term_attr.c_cc[VMIN] = 1;
	term_attr.c_cc[VTIME] = 0;

	if (tcsetattr (fd, TCSAFLUSH, &term_attr) != 0)
	{
		printf("tcsetattr(TCSAFLUSH) failed for %s: %s\n", dev, strerror(errno));
	}

	return fd;
}

/**
 * \fn listDevice 
 * \brief  list of device available
 * \return t_device list device 
 **/
t_device* listDevice(){
 	



}

