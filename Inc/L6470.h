/**
  ******************************************************************************
  * @file       L6470.h
  * @date       01/10/2014 12:00:00
  * @brief      This file contains definitions, exported variables and function
  *             prototypes related to the L6470.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2014 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __L6470_H
#define __L6470_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "microstepping_motor.h"
#include "stm32f4xx_hal.h"
#include "main.h"


#define L6470DAISYCHAINSIZE      4                  //!< Max number of identifiers of L6470 in daisy chain configuration


extern SPI_HandleTypeDef hspi1;


#define STATUS_HiZ           (1<<0)
#define STATUS_BUSY          (1<<1)
#define STATUS_SW_F          (1<<2)
#define STATUS_SW_EVN        (1<<3)
#define STATUS_DIR           (1<<4)
#define STATUS_MOT_STATUS_0  (1<<5)
#define STATUS_MOT_STATUS_1  (1<<6)
#define STATUS_NOTPERF_CMD   (1<<7)
#define STATUS_WRONG_CMD     (1<<8)
#define STATUS_UVLO          (1<<9)
#define STATUS_TH_WRN        (1<<10)
#define STATUS_TH_SD         (1<<11)
#define STATUS_OCD           (1<<12)
#define STATUS_STEP_LOSS_A   (1<<13)
#define STATUS_STEP_LOSS_B   (1<<14)
#define STATUS_SCK_MOD       (1<<15)



/**
  * @addtogroup BSP
  * @{
  */

/**
  * @addtogroup Components
  * @{
  */

/**
  * @defgroup L6470
  * @brief    Tools to manage the L6470 Stepper Motor Driver.
  * @{
  */

/**
  * @defgroup   L6470_Exported_Types
  * @brief      L6470 Exported Types.
  * @{
  */

/**
  * @brief  The structure to store some features of the L6470 Registers.
  */
typedef struct {
  uint8_t Address;                  //!< Register Address
  uint8_t Name[12];                 //!< Register Name
  uint8_t LengthBit;                //!< Register Length in bits
  uint8_t LengthByte;               //!< Register Length in bytes
  uint32_t ResetValue;              //!< Register Reset Value
} sL6470_Register_t;

/**
  * @brief  The structure to store some features of the L6470 Application Commands.
  */
typedef struct {
  uint8_t Mnemonic[12];             //!< AppCmd Mnemonic
  uint8_t BinaryCode;               //!< AppCmd Binary Code
  uint8_t NrOfParameters;           //!< AppCmd number of needed parameters
} sL6470_ApplicationCommand_t;

/**
  * @brief  The structure to store some features about the L6470 Motor Direction.
  */
typedef struct {
  uint8_t Mnemonic[8];              //!< L6470 Direction Mnemonic
  uint8_t BinaryCode;               //!< L6470 Direction Binary Code
} sL6470_Direction_t;

/**
  * @brief  The structure to store some features about the action taken with the L6470 ABS_POS register.
  */
typedef struct {
  uint8_t Mnemonic[4];              //!< ACT Mnemonic
  uint8_t BinaryCode;               //!< ACT Binary Code
} sL6470_ACT_t;

/**
  * @brief  The structure used to store the identifier of the L6470 application
  *         command and its the needed parameters.
  * @note   The data stored into this structure will be used to fill the matrix
  *         used by SPI to send the command to the L6470.
  */
typedef struct {
  eL6470_AppCmdId_t L6470_AppCmdId;   //!< The identifier of the actual L6470 Application Command
  uint32_t p1;                      //!< The 1st parameter if needed
  uint32_t p2;                      //!< The 2nd parameter if needed
  uint32_t p3;                      //!< The 3rd parameter if needed
} sL6470_AppCmdPkg_t;

/**
  * @}
  */ /* End of L6470_Exported_Types */

/**
  * @defgroup   L6470_Exported_Macros
  * @brief      L6470 Exported Macros.
  * @{
  */

#define L6470_DISABLE()         HAL_GPIO_WritePin(L6470_RST_STBY_GPIO_Port, L6470_RST_STBY_Pin, GPIO_PIN_RESET)  //!< Connect to GND the L6470 nSTBY/nRST pin
#define L6470_ENABLE()          HAL_GPIO_WritePin(L6470_RST_STBY_GPIO_Port, L6470_RST_STBY_Pin, GPIO_PIN_SET)    //!< Connect to VDD the L6470 nSTBY/nRST pin
#define L6470_nCS_LOW()         HAL_GPIO_WritePin(L6470_CS_GPIO_Port, L6470_CS_Pin, GPIO_PIN_RESET)                //!< Connect to GND the selected L6470 nCS pin
#define L6470_nCS_HIGH()        HAL_GPIO_WritePin(L6470_CS_GPIO_Port, L6470_CS_Pin, GPIO_PIN_SET)                  //!< Connect to VDD the selected L6470 nCS pin

/**
  * @}
  */ /* End of L6470_Exported_Macros */

/**
  * @defgroup   L6470_Exported_Constants
  * @brief      L6470 Exported Constants.
  * @{
  */

/**
  * @defgroup   L6470_Register_Max_Values
  * @brief      Maximum values for L6470 registers.
  * @{
  */

#define L6470_MAX_POSITION          (0x1FFFFF)          //!< Max position
#define L6470_MIN_POSITION          (-(0x200000))       //!< Min position
#define L6470_POSITION_RANGE        ((uint32_t)(L6470_MAX_POSITION - L6470_MIN_POSITION))   //!< Position range
#define L6470_MAX_SPEED             (0xFFFFF)           //!< max value of SPEED
#define L6470_MAX_ACC               (0xFFF)             //!< max value of ACC
#define L6470_MAX_DEC               (0xFFF)             //!< max value of DEC
#define L6470_MAX_MAX_SPEED         (0x3FF)             //!< max value of MAX_SPEED
#define L6470_MAX_MIN_SPEED         (0xFFF)             //!< max value of MIN_SPEED
#define L6470_MAX_FS_SPD            (0x3FF)             //!< max value of FS_SPD
#define L6470_MAX_INT_SPEED         (0x3FFF)            //!< max value of INT_SPEED
#define L6470_MAX_ST_SLP            (0xFF)              //!< max value of ST_SLP
#define L6470_MAX_FN_SLP_ACC        (0xFF)              //!< max value of FN_SLP_ACC
#define L6470_MAX_FN_SLP_DEC        (0xFF)              //!< max value of FN_SLP_DEC
#define L6470_MAX_OCD_TH            (0xF)               //!< max value of OCD_TH
#define L6470_MAX_STALL_TH          (0x7F)              //!< max value of STALL_TH

/**
  * @}
  */ /* End of L6470_Register_Max_Values */

#define L6470REGIDSIZE          25                  //!< Max number of identifiers of L6470 Registers
#define L6470APPCMDIDSIZE       19                  //!< Max number of identifiers of L6470 Application Commands
#define L6470DIRIDSIZE           2                  //!< Max number of identifiers of L6470 directions
#define L6470ACTIDSIZE           2                  //!< Max number of identifiers of actions to perform about ABS_POS register
#define L6470MAXSPICMDBYTESIZE   4                  //!< Max number of byte to send via SPI to perform an application command

#define L6470_MAX_SPEED_VALUE   ((float)15610)      //!< max value for the speed in step/s
#define L6470_MAX_ACC_VALUE     ((float)59590)      //!< max value for the acceleration in step/s^2
#define L6470_MAX_DEC_VALUE     ((float)59590)      //!< max value for the acceleration in step/s^2
#define L6470_MAX_DEC_VALUE     ((float)59590)      //!< max value for the acceleration in step/s^2

#define OCD_TH_STEP             ((float)375)      //!< Minimum step for OCD_TH register in mAmpere
#define STALL_TH_STEP           ((float)31.25)    //!< Minimum step for STALL_TH register in mAmpere

#define L6470_ACC_CONV          ((float)0.068719)   //!< Conversion factor for acceleration value from step/s^2 to the right value
#define L6470_DEC_CONV          ((float)0.068719)   //!< Conversion factor for deceleration value from step/s^2 to the right value
#define L6470_MAXSPEED_CONV     ((float)0.065536)   //!< Conversion factor for max speed value from step/s to the right value
#define L6470_MINSPEED_CONV     ((float)4.194304)   //!< Conversion factor for min speed value from step/s to the right value
#define L6470_SPEED_CONV        ((float)67.108864)  //!< Conversion factor for speed value from step/s to the right value

extern const sL6470_Register_t L6470_Register[];
extern const sL6470_ApplicationCommand_t L6470_ApplicationCommand[];
extern const sL6470_Direction_t L6470_Direction[];
extern const sL6470_ACT_t L6470_ACT[];

/**
  * @}
  */ /* End of L6470_Exported_Constants */

/**
  * @defgroup   L6470_Exported_Variables
  * @brief      L6470 Exported Variables.
  * @{
  */

extern StepperMotorCommand_t L6470Command;
extern uint8_t L6470_DaisyChainSpiTxStruct[L6470MAXSPICMDBYTESIZE][L6470DAISYCHAINSIZE];
extern uint8_t L6470_DaisyChainSpiRxStruct[L6470MAXSPICMDBYTESIZE][L6470DAISYCHAINSIZE];

/**
  * @}
  */ /* End of L6470_Exported_Variables */

/**
  * @defgroup   L6470_Exported_Functions
  * @brief      L6470 Exported Functions.
  * @{
  */

/**
  * @defgroup   L6470_Conversion_Functions
  * @brief      The following functions act just on one driver inside the L6470
  *             daisy chain. The command is immediately sent.
  * @{
  */
int32_t AbsPos_2_Position(uint32_t AbsPos);
uint32_t Position_2_AbsPos(int32_t Position);
float Speed_2_Step_s(uint32_t Speed);
uint32_t Step_s_2_Speed(float Step_s);
float Acc_2_Step_s2(uint16_t Acc);
uint16_t Step_s2_2_Acc(float Step_s2);
float Dec_2_Step_s2(uint16_t Dec);
uint16_t Step_s2_2_Dec(float Step_s2);
float MaxSpeed_2_Step_s(uint16_t MaxSpeed);
uint16_t Step_s_2_MaxSpeed(float Step_s);
float MinSpeed_2_Step_s(uint16_t MinSpeed);
uint16_t Step_s_2_MinSpeed(float Step_s);
float FsSpd_2_Step_s(uint16_t FsSpd);
uint16_t Step_s_2_FsSpd(float Step_s);
float IntSpeed_2_Step_s(uint16_t IntSpeed);
uint16_t Step_s_2_IntSpeed(float Step_s);
float StSlp_2_s_Step(uint8_t StSlp);
uint8_t s_Step_2_StSlp(float s_Step);
float FnSlpAcc_2_s_Step(uint8_t FnSlpAcc);
uint8_t s_Step_2_FnSlpAcc(float s_Step);
float FnSlpDec_2_s_Step(uint8_t FnSlpDec);
uint8_t s_Step_2_FnSlpDec(float s_Step);

float OcdTh_2_mA(uint8_t OcdTh);
uint8_t mA_2_OcdTh(float mA);
float StallTh_2_mA(uint8_t StallTh);
uint8_t mA_2_StallTh(float mA);


/**
  * @}
  */ /* End of L6470_Conversion_Functions */

/**
  * @defgroup   L6470_AppCMDs
  * @brief      The following functions act just on one driver inside the L6470
  *             daisy chain. The command is immediately sent.
  * @{
  */
void L6470_SetParam(uint8_t L6470_Id, eL6470_RegId_t L6470_RegId, uint32_t Value);
uint32_t L6470_GetParam(uint8_t L6470_Id, eL6470_RegId_t L6470_RegId);
void L6470_Run(uint8_t L6470_Id, eL6470_DirId_t L6470_DirId, uint32_t Speed);
void L6470_StepClock(uint8_t L6470_Id, eL6470_DirId_t L6470_DirId);
void L6470_Move(uint8_t L6470_Id, eL6470_DirId_t L6470_DirId, uint32_t N_Step);
void L6470_GoTo(uint8_t L6470_Id, uint32_t AbsPos);
void L6470_GoToDir(uint8_t L6470_Id, eL6470_DirId_t L6470_DirId, uint32_t AbsPos);
void L6470_GoUntil(uint8_t L6470_Id, eL6470_ActId_t L6470_ActId, eL6470_DirId_t L6470_DirId, uint32_t Speed);
void L6470_ReleaseSW(uint8_t L6470_Id, eL6470_ActId_t L6470_ActId, eL6470_DirId_t L6470_DirId);
void L6470_GoHome(uint8_t L6470_Id);
void L6470_GoMark(uint8_t L6470_Id);
void L6470_ResetPos(uint8_t L6470_Id);
void L6470_ResetDevice(uint8_t L6470_Id);
void L6470_SoftStop(uint8_t L6470_Id);
void L6470_HardStop(uint8_t L6470_Id);
void L6470_SoftHiZ(uint8_t L6470_Id);
void L6470_HardHiZ(uint8_t L6470_Id);
uint16_t L6470_GetStatus(uint8_t L6470_Id);
/**
  * @}
  */ /* End of L6470_AppCMDs */

/**
  * @defgroup L6470_AppCMDs_ToBePrepared
  * @brief  The following functions act just on one driver inside the L6470 daisy
  *         chain. The command is prepared and not sent. To send the application
  *         commands already prepared for the addressed L6470 drivers you have to
  *         use @ref L6470_PerformPreparedApplicationCommand.
  * @{
  */
void L6470_PrepareSetParam(uint8_t L6470_Id, eL6470_RegId_t L6470_RegId, uint32_t Value);
void L6470_PrepareGetParam(uint8_t L6470_Id, eL6470_RegId_t L6470_RegId);
void L6470_PrepareRun(uint8_t L6470_Id, eL6470_DirId_t L6470_DirId, uint32_t Speed);
void L6470_PrepareStepClock(uint8_t L6470_Id, eL6470_DirId_t L6470_DirId);
void L6470_PrepareMove(uint8_t L6470_Id, eL6470_DirId_t L6470_DirId, uint32_t N_Step);
void L6470_PrepareGoTo(uint8_t L6470_Id, uint32_t AbsPos);
void L6470_PrepareGoToDir(uint8_t L6470_Id, eL6470_DirId_t L6470_DirId, uint32_t AbsPos);
void L6470_PrepareGoUntil(uint8_t L6470_Id, eL6470_ActId_t L6470_ActId, eL6470_DirId_t L6470_DirId, uint32_t Speed);
void L6470_PrepareReleaseSW(uint8_t L6470_Id, eL6470_ActId_t L6470_ActId, eL6470_DirId_t L6470_DirId);
void L6470_PrepareGoHome(uint8_t L6470_Id);
void L6470_PrepareGoMark(uint8_t L6470_Id);
void L6470_PrepareResetPos(uint8_t L6470_Id);
void L6470_PrepareResetDevice(uint8_t L6470_Id);
void L6470_PrepareSoftStop(uint8_t L6470_Id);
void L6470_PrepareHardStop(uint8_t L6470_Id);
void L6470_PrepareSoftHiZ(uint8_t L6470_Id);
void L6470_PrepareHardHiZ(uint8_t L6470_Id);
void L6470_PrepareGetStatus(uint8_t L6470_Id);
/**
  * @}
  */ /* End of L6470_AppCMDs_ToBePrepared */

uint8_t* L6470_PerformPreparedApplicationCommand(void);
uint8_t L6470_ID(uint8_t position);
void L6470_DaisyChainCommand(uint8_t* pL6470_DaisyChainSpiTxStruct, uint8_t* pL6470_DaisyChainSpiRxStruct);
uint32_t L6470_ExtractReturnedData(uint8_t L6470_Id, uint8_t* pL6470_DaisyChainSpiRxStruct, uint8_t LengthByte);
uint8_t L6470_CheckStatusRegisterFlag(uint8_t L6470_Id, uint8_t L6470_StatusRegisterFlagId);
uint8_t *L6470_GetRegisterName(uint8_t id);
void L6470_Config(StepperMotorDriverHandle_t *StepperMotorDriverdHandle, MotorParameterData_t *MotorParameterData);

/**
  * @}
  */ /* End of L6470_Exported_Functions */

/**
  * @}
  */ /* End of L6470 */

/**
  * @}
  */ /* End of Components */

/**
  * @}
  */ /* End of BSP */

#ifdef __cplusplus
}
#endif

#endif /* __L6470_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
