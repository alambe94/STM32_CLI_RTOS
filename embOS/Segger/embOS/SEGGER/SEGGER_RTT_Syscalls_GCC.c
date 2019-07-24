/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2019 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system for microcontrollers      *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: 5.06                                             *
*                                                                    *
**********************************************************************

---------------------------END-OF-HEADER------------------------------
File    : SEGGER_RTT_Syscalls_GCC.c
Purpose : Low-level functions for using printf() via RTT in GCC.
          To use RTT for printf output, include this file in your
          application.
Revision: $Rev: 9599 $
----------------------------------------------------------------------
*/

#if (defined __GNUC__) && !(defined __SES_ARM) && !(defined __CROSSWORKS_ARM)

#include "SEGGER_RTT.h"

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/
int _write(int file, char *ptr, int len);

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _write()
*
*  Function description
*    Low-level write function writing data via RTT.
*
*    libc subroutines will use this system routine for output to all
*    files, including stdout.
*
*  Note
*    This function will be called by its reentrant version '_write_r'
*    which is contained in libc.
*/
int _write(int file, char *ptr, int len) {
  (void) file;  /* Not used, avoid warning */
  SEGGER_RTT_Write(0, ptr, len);
  return len;
}

#endif

/*************************** End of file ****************************/
