/**
 * \file command.h
 * \author Johan Boris Iantila
 * \version 0.1
 * \date 21/01/2016
 *
 *
 */

#ifndef COMMAND_H
#define COMMAND_H
#define BUFFER_SIZE 2048

#include <sys/time.h>
#include "enum.h"	

typedef struct smscommand t_smscommand;
typedef struct device t_device;

struct smscommand{
  char *pduBuffer;
  char *messages;
  char *phoneNumber;
};

struct device{
 char *devicename;
 t_device *next;
};

typedef struct at_smscommand{
	at_cmd_t		cmd;			/*!< command code */
	at_res_t		res;			/*!< expected responce code, can be RES_OK, RES_CMGR, RES_SMS_PROMPT */

	unsigned		flags;			/*!< flags */
#define ATQ_CMD_FLAG_DEFAULT	0x00				/*!< empty flags */
#define ATQ_CMD_FLAG_STATIC	0x01				/*!< data is static no try deallocate */
#define ATQ_CMD_FLAG_IGNORE	0x02				/*!< ignore response non match condition */

	struct timeval		timeout;		/*!< timeout value, started at time when command actually written on device */
#define ATQ_CMD_TIMEOUT_1S	1				/*!< timeout value  1 sec */
#define ATQ_CMD_TIMEOUT_2S	2				/*!< timeout value  2 sec */
#define ATQ_CMD_TIMEOUT_5S	5				/*!< timeout value  5 sec */
#define ATQ_CMD_TIMEOUT_10S	10				/*!< timeout value 10 sec */
#define ATQ_CMD_TIMEOUT_15S	15				/*!< timeout value 15 ses */
#define ATQ_CMD_TIMEOUT_40S	40				/*!< timeout value 40 ses */

	char*			data;			/*!< command and data to send in device */
	unsigned		length;			/*!< data length */
} at_queue_cmd_t;


t_smscommand *createSmsCommand(const char* phoneNumber, const char* message);

void sendcommand(int fd, const char *str);

int openttyUSB (const char* dev);
#endif 



