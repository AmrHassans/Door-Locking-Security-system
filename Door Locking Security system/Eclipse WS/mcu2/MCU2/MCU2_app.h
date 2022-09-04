/*
 ================================================================================================
 Name        : MCU2_app.h
 Author      : Amr Hassan
 Description : control-ECU
 Date        : 14/10/2021
 ================================================================================================
 */
#ifndef MCU2_APP_H_
#define MCU2_APP_H_

#include "std_types.h"


/*******************************************************************************
 *                              Definitions                                    *
 *******************************************************************************/

#define PASSCODE_CH_LENGTH                    (5)
#define DOOR_LOCK_UNLOCK_PERIOD               (15)
#define DOOR_STAND_BY_PERIOD                  (3)
#define NUMBER_OF_ALLOWED_WRONG_ATTEMPTS      (3)
#define ALARM_PERIOD                          (60)

#define PASSCODE_MATCH                        (1)
#define PASSCODE_NOT_MATCH                    (0)
#define MC_READY_SEND                        (0x15)
#define MC_READY_RECIVE                      (0x16)
#define DOOR_UNLOOK                          (0x25)
#define PASSCODE_WRONG                       (0x30)
#define CHANGE_PASSCODE                      (0x31)
#define CHANGE_PASSCODE_OPTION               (0x18)

#define TWI_CONTROL_ECU_ADDRESS              (0x1)
#define EEPROM_STORE_ADDREESS		 		 (0x00)


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint16 g_sec = 0 ;
uint8 g_wrongAttemptCtr = 0 ;
uint8 g_recivedPasscode[PASSCODE_CH_LENGTH];
uint8 g_storedPasscode[PASSCODE_CH_LENGTH];

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void PASSCODEInit(void);

void DoorOpen (void);

void recivePasscodeByUART (uint8 * pass_arr);

void timerCallBack (void);

void storePasscode(void);

void updateStoredPasscode(void);

uint8 comparePasscode(uint8 a_passcode1[PASSCODE_CH_LENGTH],uint8 a_passcode2[PASSCODE_CH_LENGTH]);







#endif /* MCU2_APP_H_ */
