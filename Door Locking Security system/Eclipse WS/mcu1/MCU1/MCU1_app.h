/*
 ================================================================================================
 Name        : MCU1_app.h
 Author      : Amr Hassan
 Description : HMI-ECU
 Date        : 14/10/2021
 ================================================================================================
 */

#ifndef MCU1_APP_H_
#define MCU1_APP_H_

#include "std_types.h"

/*******************************************************************************
 *                              Definitions                                    *
 *******************************************************************************/

#define PASSCODE_CH_LENGTH           (5)
#define DOOR_LOCK_UNLOCK_PERIOD      (15)
#define DOOR_STAND_BY_PERIOD         (3)
#define KEYPAD_INPUT_DELAY           (3000)
#define DISPLAY_MESSAGE_DELAY        (5000)


#define PASSCODE_MATCH             (1)
#define PASSCODE_NOT_MATCH         (0)
#define MC_READY_SEND             (0x15)
#define MC_READY_RECIVE           (0x16)
#define DOOR_UNLOOK               (0x25)
#define PASSCODE_WRONG            (0x30)
#define CHANGE_PASSCODE           (0x31)
#define CHANGE_PASSCODE_OPTION    (0x18)


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint16 g_sec = 0;
uint8  g_passcode[PASSCODE_CH_LENGTH];
uint8 g_matchStatus = 0;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void PASSCODE_Init (void);

void getPASSCODE(uint8 * arr);



void MainMenu  (void);

void sendPasscodeByUART (uint8 * pass_arr);


void timerCallBack(void);

void DoorOpen (void);








#endif /* MCU1_APP_H_ */
