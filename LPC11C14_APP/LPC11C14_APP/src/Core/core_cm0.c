/******************************************************************************
 * @file:    core_cm0.c
 * @purpose: CMSIS Cortex-M0 Core Peripheral Access Layer Source File
 * @version: V1.0
 * @date:    12. Nov. 2008
 *----------------------------------------------------------------------------
 *
 * Copyright (C) 2008 ARM Limited. All rights reserved.
 *
 * ARM Limited (ARM) is supplying this software for use with Cortex-M0 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#include <stdint.h>

/* define compiler specific symbols */
#if defined(__CC_ARM)
#define __ASM __asm       /*!< asm keyword for armcc          */
#define __INLINE __inline /*!< inline keyword for armcc        */

#elif defined(__ICCARM__)
#define __ASM __asm          /*!< asm keyword for iarcc           */
#define __INLINE inline      /*!< inline keyword for iarcc. Only avaiable in High optimization mode! */
#define __nop __no_operation /*!< no operation intrinsic in iarcc */

#elif defined(__GNUC__)
#define __ASM asm       /*!< asm keyword for gcc            */
#define __INLINE inline /*!< inline keyword for gcc         */
#endif

#if (defined(__ARMCC_VERSION)) && (__ARMCC_VERSION < 400000)

/**
 * @brief  Return the Process Stack Pointer
 *
 * @param  none
 * @return uint32 ProcessStackPointer
 *
 * Return the actual process stack pointer
 */
__ASM uint32 __get_PSP(void){
    mrs r0, psp
                bx lr}

/**
 * @brief  Set the Process Stack Pointer
 *
 * @param  uint32 Process Stack Pointer
 * @return none
 *
 * Assign the value ProcessStackPointer to the MSP 
 * (process stack pointer) Cortex processor register
 */
__ASM void __set_PSP(uint32 topOfProcStack){
    msr psp, r0
                 bx lr}

/**
 * @brief  Return the Main Stack Pointer
 *
 * @param  none
 * @return uint32 Main Stack Pointer
 *
 * Return the current value of the MSP (main stack pointer)
 * Cortex processor register
 */
__ASM uint32 __get_MSP(void){
    mrs r0, msp
                bx lr}

/**
 * @brief  Set the Main Stack Pointer
 *
 * @param  uint32 Main Stack Pointer
 * @return none
 *
 * Assign the value mainStackPointer to the MSP 
 * (main stack pointer) Cortex processor register
 */
__ASM void __set_MSP(uint32 mainStackPointer){
    msr msp, r0
                 bx lr}

/**
 * @brief  Return the Base Priority value
 *
 * @param  none
 * @return uint32 BasePriority
 *
 * Return the content of the base priority register
 */
__ASM uint32 __get_BASEPRI(void){
    mrs r0, basepri
                bx lr}

/**
 * @brief  Set the Base Priority value
 *
 * @param  uint32 BasePriority
 * @return none
 *
 * Set the base priority register
 */
__ASM void __set_BASEPRI(uint32 basePri){
    msr basepri, r0
                     bx lr}

/**
 * @brief  Return the Priority Mask value
 *
 * @param  none
 * @return uint32 PriMask
 *
 * Return the state of the priority mask bit from the priority mask
 * register
 */
__ASM uint32 __get_PRIMASK(void){
    mrs r0, primask
                bx lr}

/**
 * @brief  Set the Priority Mask value
 *
 * @param  uint32 PriMask
 * @return none
 *
 * Set the priority mask bit in the priority mask register
 */
__ASM void __set_PRIMASK(uint32 priMask){
    msr primask, r0
                     bx lr}

/**
 * @brief  Return the Fault Mask value
 *
 * @param  none
 * @return uint32 FaultMask
 *
 * Return the content of the fault mask register
 */
__ASM uint32 __get_FAULTMASK(void){
    mrs r0, faultmask
                bx lr}

/**
 * @brief  Set the Fault Mask value
 *
 * @param  uint32 faultMask value
 * @return none
 *
 * Set the fault mask register
 */
__ASM void __set_FAULTMASK(uint32 faultMask){
    msr faultmask, r0
                       bx lr}

/**
 * @brief  Return the Control Register value
* 
*  @param  none
*  @return uint32 Control value
 *
 * Return the content of the control register
 */
__ASM uint32 __get_CONTROL(void){
    mrs r0, control
                bx lr}

/**
 * @brief  Set the Control Register value
 *
 * @param  uint32 Control value
 * @return none
 *
 * Set the control register
 */
__ASM void __set_CONTROL(uint32 control){
    msr control, r0
                     bx lr}

/**
 * @brief  Reverse byte order in unsigned short value
 *
 * @param  uint16 value to reverse
 * @return uint32 reversed value
 *
 * Reverse byte order in unsigned short value
 */
__ASM uint32 __REV16(uint16 value){
    rev16 r0, r0
                  bx lr}

/**
 * @brief  Reverse byte order in signed short value with sign extension to integer
 *
 * @param  int16 value to reverse
 * @return int32 reversed value
 *
 * Reverse byte order in signed short value with sign extension to integer
 */
__ASM int32 __REVSH(int16 value)
{
  revsh r0, r0
                bx lr
}

#elif (defined(__ICCARM__))
#pragma diag_suppress = Pe940

/**
 * @brief  Return the Process Stack Pointer
 *
 * @param  none
 * @return uint32 ProcessStackPointer
 *
 * Return the actual process stack pointer
 */
uint32 __get_PSP(void)
{
  __ASM("mrs r0, psp");
  __ASM("bx lr");
}

/**
 * @brief  Set the Process Stack Pointer
 *
 * @param  uint32 Process Stack Pointer
 * @return none
 *
 * Assign the value ProcessStackPointer to the MSP 
 * (process stack pointer) Cortex processor register
 */
void __set_PSP(uint32 topOfProcStack)
{
  __ASM("msr psp, r0");
  __ASM("bx lr");
}

/**
 * @brief  Return the Main Stack Pointer
 *
 * @param  none
 * @return uint32 Main Stack Pointer
 *
 * Return the current value of the MSP (main stack pointer)
 * Cortex processor register
 */
uint32 __get_MSP(void)
{
  __ASM("mrs r0, msp");
  __ASM("bx lr");
}

/**
 * @brief  Set the Main Stack Pointer
 *
 * @param  uint32 Main Stack Pointer
 * @return none
 *
 * Assign the value mainStackPointer to the MSP 
 * (main stack pointer) Cortex processor register
 */
void __set_MSP(uint32 topOfMainStack)
{
  __ASM("msr msp, r0");
  __ASM("bx lr");
}

/**
 * @brief  Reverse byte order in unsigned short value
 *
 * @param  uint16 value to reverse
 * @return uint32 reversed value
 *
 * Reverse byte order in unsigned short value
 */
uint32 __REV16(uint16 value)
{
  __ASM("rev16 r0, r0");
  __ASM("bx lr");
}

/**
 * @brief  Reverse bit order of value
 *
 * @param  uint32 value to reverse
 * @return uint32 reversed value
 *
 * Reverse bit order of value
 */
uint32 __RBIT(uint32 value)
{
  __ASM("rbit r0, r0");
  __ASM("bx lr");
}

/**
 * @brief  LDR Exclusive
 *
 * @param  uint8* address
 * @return uint8 value of (*address)
 *
 * Exclusive LDR command
 */
uint8 __LDREXB(uint8 *addr)
{
  __ASM("ldrexb r0, [r0]");
  __ASM("bx lr");
}

/**
 * @brief  LDR Exclusive
 *
 * @param  uint16* address
 * @return uint16 value of (*address)
 *
 * Exclusive LDR command
 */
uint16 __LDREXH(uint16 *addr)
{
  __ASM("ldrexh r0, [r0]");
  __ASM("bx lr");
}

/**
 * @brief  LDR Exclusive
 *
 * @param  uint32* address
 * @return uint32 value of (*address)
 *
 * Exclusive LDR command
 */
uint32 __LDREXW(uint32 *addr)
{
  __ASM("ldrex r0, [r0]");
  __ASM("bx lr");
}

/**
 * @brief  STR Exclusive
 *
 * @param  uint8 *address
 * @param  uint8 value to store
 * @return uint32 successful / failed
 *
 * Exclusive STR command
 */
uint32 __STREXB(uint8 value, uint8 *addr)
{
  __ASM("strexb r0, r0, [r1]");
  __ASM("bx lr");
}

/**
 * @brief  STR Exclusive
 *
 * @param  uint16 *address
 * @param  uint16 value to store
 * @return uint32 successful / failed
 *
 * Exclusive STR command
 */
uint32 __STREXH(uint16 value, uint16 *addr)
{
  __ASM("strexh r0, r0, [r1]");
  __ASM("bx lr");
}

/**
 * @brief  STR Exclusive
 *
 * @param  uint32 *address
 * @param  uint32 value to store
 * @return uint32 successful / failed
 *
 * Exclusive STR command
 */
uint32 __STREXW(uint32 value, uint32 *addr)
{
  __ASM("strex r0, r0, [r1]");
  __ASM("bx lr");
}

#pragma diag_default = Pe940

#elif (defined(__GNUC__))

/**
 * @brief  Return the Control Register value
* 
*  @param  none
*  @return uint32 Control value
 *
 * Return the content of the control register
 */
uint32 __get_CONTROL(void)
{
  uint32 result = 0;
  __ASM volatile("MRS %0, control"
                 : "=r"(result));
  return (result);
}

/**
 * @brief  Set the Control Register value
 *
 * @param  uint32 Control value
 * @return none
 *
 * Set the control register
 */
void __set_CONTROL(uint32 control)
{
  __ASM volatile("MSR control, %0"
                 :
                 : "r"(control));
}

/**
 * @brief  Return the Process Stack Pointer
 *
 * @param  none
 * @return uint32 ProcessStackPointer
 *
 * Return the actual process stack pointer
 */
uint32 __get_PSP(void)
{
  uint32 result = 0;

  __ASM volatile("MRS %0, psp"
                 : "=r"(result));
  return (result);
}

/**
 * @brief  Set the Process Stack Pointer
 *
 * @param  uint32 Process Stack Pointer
 * @return none
 *
 * Assign the value ProcessStackPointer to the MSP 
 * (process stack pointer) Cortex processor register
 */
void __set_PSP(uint32 topOfProcStack)
{
  __ASM volatile("MSR psp, %0"
                 :
                 : "r"(topOfProcStack));
}

/**
 * @brief  Return the Main Stack Pointer
 *
 * @param  none
 * @return uint32 Main Stack Pointer
 *
 * Return the current value of the MSP (main stack pointer)
 * Cortex processor register
 */
uint32 __get_MSP(void)
{
  uint32 result = 0;

  __ASM volatile("MRS %0, msp"
                 : "=r"(result));
  return (result);
}

/**
 * @brief  Set the Main Stack Pointer
 *
 * @param  uint32 Main Stack Pointer
 * @return none
 *
 * Assign the value mainStackPointer to the MSP 
 * (main stack pointer) Cortex processor register
 */
void __set_MSP(uint32 topOfMainStack)
{
  __ASM volatile("MSR msp, %0"
                 :
                 : "r"(topOfMainStack));
}

/**
 * @brief  Return the Base Priority value
 *
 * @param  none
 * @return uint32 BasePriority
 *
 * Return the content of the base priority register
 */
uint32 __get_BASEPRI(void)
{
  uint32 result = 0;

  __ASM volatile("MRS %0, basepri_max"
                 : "=r"(result));
  return (result);
}

/**
 * @brief  Set the Base Priority value
 *
 * @param  uint32 BasePriority
 * @return none
 *
 * Set the base priority register
 */
void __set_BASEPRI(uint32 value)
{
  __ASM volatile("MSR basepri, %0"
                 :
                 : "r"(value));
}

/**
 * @brief  Return the Priority Mask value
 *
 * @param  none
 * @return uint32 PriMask
 *
 * Return the state of the priority mask bit from the priority mask
 * register
 */
uint32 __get_PRIMASK(void)
{
  uint32 result = 0;

  __ASM volatile("MRS %0, primask"
                 : "=r"(result));
  return (result);
}

/**
 * @brief  Set the Priority Mask value
 *
 * @param  uint32 PriMask
 * @return none
 *
 * Set the priority mask bit in the priority mask register
 */
void __set_PRIMASK(uint32 priMask)
{
  __ASM volatile("MSR primask, %0"
                 :
                 : "r"(priMask));
}

/**
 * @brief  Return the Fault Mask value
 *
 * @param  none
 * @return uint32 FaultMask
 *
 * Return the content of the fault mask register
 */
uint32 __get_FAULTMASK(void)
{
  uint32 result = 0;

  __ASM volatile("MRS %0, faultmask"
                 : "=r"(result));
  return (result);
}

/**
 * @brief  Set the Fault Mask value
 *
 * @param  uint32 faultMask value
 * @return none
 *
 * Set the fault mask register
 */
void __set_FAULTMASK(uint32 faultMask)
{
  __ASM volatile("MSR faultmask, %0"
                 :
                 : "r"(faultMask));
}

/**
 * @brief  Reverse byte order in integer value
 *
 * @param  uint32 value to reverse
 * @return uint32 reversed value
 *
 * Reverse byte order in integer value
 */
uint32 __REV(uint32 value)
{
  uint32 result = 0;

  __ASM volatile("rev %0, %1"
                 : "=r"(result)
                 : "r"(value));
  return (result);
}

/**
 * @brief  Reverse byte order in unsigned short value
 *
 * @param  uint16 value to reverse
 * @return uint32 reversed value
 *
 * Reverse byte order in unsigned short value
 */
uint32 __REV16(uint16 value)
{
  uint32 result = 0;

  __ASM volatile("rev16 %0, %1"
                 : "=r"(result)
                 : "r"(value));
  return (result);
}

/**
 * @brief  Reverse byte order in signed short value with sign extension to integer
 *
 * @param  int32 value to reverse
 * @return int32 reversed value
 *
 * Reverse byte order in signed short value with sign extension to integer
 */
int32 __REVSH(int16 value)
{
  uint32 result = 0;

  __ASM volatile("revsh %0, %1"
                 : "=r"(result)
                 : "r"(value));
  return (result);
}

/**
 * @brief  Reverse bit order of value
 *
 * @param  uint32 value to reverse
 * @return uint32 reversed value
 *
 * Reverse bit order of value
 */
uint32 __RBIT(uint32 value)
{
  uint32 result = 0;

  __ASM volatile("rbit %0, %1"
                 : "=r"(result)
                 : "r"(value));
  return (result);
}

/**
 * @brief  LDR Exclusive
 *
 * @param  uint8* address
 * @return uint8 value of (*address)
 *
 * Exclusive LDR command
 */
uint8 __LDREXB(uint8 *addr)
{
  uint8 result = 0;

  __ASM volatile("ldrexb %0, [%1]"
                 : "=r"(result)
                 : "r"(addr));
  return (result);
}

/**
 * @brief  LDR Exclusive
 *
 * @param  uint16* address
 * @return uint16 value of (*address)
 *
 * Exclusive LDR command
 */
uint16 __LDREXH(uint16 *addr)
{
  uint16 result = 0;

  __ASM volatile("ldrexh %0, [%1]"
                 : "=r"(result)
                 : "r"(addr));
  return (result);
}

/**
 * @brief  LDR Exclusive
 *
 * @param  uint32* address
 * @return uint32 value of (*address)
 *
 * Exclusive LDR command
 */
uint32 __LDREXW(uint32 *addr)
{
  uint32 result = 0;

  __ASM volatile("ldrex %0, [%1]"
                 : "=r"(result)
                 : "r"(addr));
  return (result);
}

/**
 * @brief  STR Exclusive
 *
 * @param  uint8 *address
 * @param  uint8 value to store
 * @return uint32 successful / failed
 *
 * Exclusive STR command
 */
uint32 __STREXB(uint8 value, uint8 *addr)
{
  uint32 result = 0;

  __ASM volatile("strexb %0, %2, [%1]"
                 : "=r"(result)
                 : "r"(addr), "r"(value));
  return (result);
}

/**
 * @brief  STR Exclusive
 *
 * @param  uint16 *address
 * @param  uint16 value to store
 * @return uint32 successful / failed
 *
 * Exclusive STR command
 */
uint32 __STREXH(uint16 value, uint16 *addr)
{
  uint32 result = 0;

  __ASM volatile("strexh %0, %2, [%1]"
                 : "=r"(result)
                 : "r"(addr), "r"(value));
  return (result);
}

/**
 * @brief  STR Exclusive
 *
 * @param  uint32 *address
 * @param  uint32 value to store
 * @return uint32 successful / failed
 *
 * Exclusive STR command
 */
uint32 __STREXW(uint32 value, uint32 *addr)
{
  uint32 result = 0;

  __ASM volatile("strex %0, %2, [%1]"
                 : "=r"(result)
                 : "r"(addr), "r"(value));
  return (result);
}

#endif
