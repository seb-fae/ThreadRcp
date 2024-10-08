/***************************************************************************//**
 * @file
 * @brief Core interrupt handling API
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "em_core.h"
#include "sl_assert.h"

/* *INDENT-OFF* */
// *****************************************************************************
///  @addtogroup core CORE - Core Interrupt
///
///@n @section core_intro Introduction
///
///  CORE interrupt API provides a simple and safe means
///  to disable and enable interrupts to protect sections of code.
///
///  This is often referred to as "critical sections". This module provides
///  support for three types of critical sections, each with different interrupt
///  blocking capabilities.
///
///  @li <b>CRITICAL</b> section: Inside a critical section, all interrupts are
///      disabled (except for fault handlers). The PRIMASK register is always used for
///      interrupt disable/enable.
///  @li <b>ATOMIC</b> section: This type of section is configurable and the default
///      method is to use PRIMASK. With BASEPRI configuration, interrupts with priority
///      equal to or lower than a given configurable level are disabled. The interrupt
///      disable priority level is defined at compile time. The BASEPRI register is not
///      available for all architectures.
///  @li <b>NVIC mask</b> section: Disable NVIC (external interrupts) on an
///      individual manner.
///
///  em_core also has an API for manipulating RAM-based interrupt vector tables.
///
///@n @section core_conf Compile-time Configuration
///
///  The following #defines are used to configure em_core:
///  @code{.c}
///  // The interrupt priority level used inside ATOMIC sections.
///  #define CORE_ATOMIC_BASE_PRIORITY_LEVEL    3
///
///  // A method used for interrupt disable/enable within ATOMIC sections.
///  #define CORE_ATOMIC_METHOD                 CORE_ATOMIC_METHOD_PRIMASK
///  @endcode
///
///  If the default values do not support your needs, they can be overridden
///  by supplying -D compiler flags on the compiler command line or by collecting
///  all macro redefinitions in a file named @em emlib_config.h and then supplying
///  -DEMLIB_USER_CONFIG on a compiler command line.
///
///  @note The default emlib configuration for ATOMIC section interrupt disable
///        method is using PRIMASK, i.e., ATOMIC sections are implemented as
///        CRITICAL sections.
///
///  @note Due to architectural limitations Cortex-M0+ devices do not support
///        ATOMIC type critical sections using the BASEPRI register. On M0+
///        devices ATOMIC section helper macros are available but they are
///        implemented as CRITICAL sections using PRIMASK register.
///
///@n @section core_macro_api Macro API
///
///  The primary em_core API is the macro API. Macro API will map to correct
///  CORE functions according to the selected @ref CORE_ATOMIC_METHOD and similar
///  configurations (the full CORE API is of course also available).
///  The most useful macros are as follows:
///
///  @ref CORE_DECLARE_IRQ_STATE @n @ref CORE_ENTER_ATOMIC() @n
///  @ref CORE_EXIT_ATOMIC()@n
///  Used together to implement an ATOMIC section.
///  @code{.c}
///  {
///    CORE_DECLARE_IRQ_STATE;           // Storage for saving IRQ state prior to
///                                      // atomic section entry.
///
///    CORE_ENTER_ATOMIC();              // Enter atomic section.
///
///    ...
///    ... your code goes here ...
///    ...
///
///    CORE_EXIT_ATOMIC();               // Exit atomic section, IRQ state is restored.
///  }
///  @endcode
///
///  @n @ref CORE_ATOMIC_SECTION(yourcode)@n
///  A concatenation of all three macros above.
///  @code{.c}
///  {
///    CORE_ATOMIC_SECTION(
///      ...
///      ... your code goes here ...
///      ...
///    )
///  }
///  @endcode
///
///  @n @ref CORE_DECLARE_IRQ_STATE @n @ref CORE_ENTER_CRITICAL() @n
///  @ref CORE_EXIT_CRITICAL() @n @ref CORE_CRITICAL_SECTION(yourcode)@n
///  These macros implement CRITICAL sections in a similar fashion as described
///  above for ATOMIC sections.
///
///  @n @ref CORE_DECLARE_NVIC_STATE @n @ref CORE_ENTER_NVIC() @n
///  @ref CORE_EXIT_NVIC() @n @ref CORE_NVIC_SECTION(yourcode)@n
///  These macros implement NVIC mask sections in a similar fashion as described
///  above for ATOMIC sections. See @ref core_examples for an example.
///
///  Refer to @em Macros or <em>Macro Definition Documentation</em> below for a
///  full list of macros.
///
///@n @section core_reimplementation API reimplementation
///
///  Most of the functions in the API are implemented as weak functions. This means
///  that it is easy to reimplement when special needs arise. Shown below is a
///  reimplementation of CRITICAL sections suitable if FreeRTOS OS is used:
///  @code{.c}
///  CORE_irqState_t CORE_EnterCritical(void)
///  {
///    vPortEnterCritical();
///    return 0;
///  }
///
///  void CORE_ExitCritical(CORE_irqState_t irqState)
///  {
///    (void)irqState;
///    vPortExitCritical();
///  }
///  @endcode
///  Also note that CORE_Enter/ExitCritical() are not implemented as inline
///  functions. As a result, reimplementations will be possible even when original
///  implementations are inside a linked library.
///
///  Some RTOSes must be notified on interrupt handler entry and exit. Macros
///  @ref CORE_INTERRUPT_ENTRY() and @ref CORE_INTERRUPT_EXIT() are suitable
///  placeholders for inserting such code. Insert these macros in all your
///  interrupt handlers and then override the default macro implementations.
///  This is an example if uC/OS is used:
///  @code{.c}
///  // In emlib_config.h:
///
///  #define CORE_INTERRUPT_ENTRY()   OSIntEnter()
///  #define CORE_INTERRUPT_EXIT()    OSIntExit()
///  @endcode
///
///@n @section core_vector_tables Interrupt vector tables
///
///  When using RAM based interrupt vector tables it is the user's responsibility
///  to allocate the table space correctly. The tables must be aligned as specified
///  in the CPU reference manual.
///
///  @ref CORE_InitNvicVectorTable()@n
///  Initialize a RAM based vector table by copying table entries from a source
///  vector table to a target table. VTOR is set to the address of the target
///  vector table.
///
///  @n @ref CORE_GetNvicRamTableHandler() @n @ref CORE_SetNvicRamTableHandler()@n
///  Use these functions to get or set the interrupt handler for a specific IRQn.
///  They both use the interrupt vector table defined by the current
///  VTOR register value.
///
///@n @section core_max_timing Maximum Interrupt Disabled Time
///
///  The maximum time spent (in cycles) in critical and atomic sections can be
///  measured for performance and interrupt latency analysis.
///  To enable the timings, use the SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING
///  configuration option. When enabled, the functions
///  @n @ref CORE_get_max_time_critical_section()
///  @n @ref CORE_get_max_time_atomic_section() @n
///  can be used to get the max timings since startup.
///
///@n @section core_examples Examples
///
///  Implement an NVIC critical section:
///  @code{.c}
///  {
///    CORE_DECLARE_NVIC_ZEROMASK(mask); // A zero initialized NVIC disable mask
///
///    // Set mask bits for IRQs to block in the NVIC critical section.
///    // In many cases, you can create the disable mask once upon application
///    // startup and use the mask globally throughout the application lifetime.
///    CORE_NvicMaskSetIRQ(LEUART0_IRQn, &mask);
///    CORE_NvicMaskSetIRQ(VCMP_IRQn,    &mask);
///
///    // Enter NVIC critical section with the disable mask
///    CORE_NVIC_SECTION(&mask,
///      ...
///      ... your code goes here ...
///      ...
///    )
///  }
///  @endcode
///
///@n @section core_porting Porting from em_int
///
///  Existing code using INT_Enable() and INT_Disable() must be ported to the
///  em_core API. While em_int used, a global counter to store the interrupt state,
///  em_core uses a local variable. Any usage of INT_Disable(), therefore, needs to
///  be replaced with a declaration of the interrupt state variable before entering
///  the critical section.
///
///  Since the state variable is in local scope, the critical section exit
///  needs to occur within the scope of the variable. If multiple nested critical
///  sections are used, each needs to have its own state variable in its own scope.
///
///  In many cases, completely disabling all interrupts using CRITICAL sections
///  might be more heavy-handed than needed. When porting, consider whether other
///  types of sections, such as ATOMIC or NVIC mask, can be used to only disable
///  a subset of the interrupts.
///
///  Replacing em_int calls with em_core function calls:
///  @code{.c}
///  void func(void)
///  {
///    // INT_Disable();
///    CORE_DECLARE_IRQ_STATE;
///    CORE_ENTER_ATOMIC();
///      .
///      .
///      .
///    // INT_Enable();
///    CORE_EXIT_ATOMIC();
///  }
///  @endcode
/// @{
// *****************************************************************************
/* *INDENT-ON* */

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#if !defined(CORE_INTERRUPT_ENTRY)
// Some RTOSes must be notified on interrupt entry (and exit).
// Use this macro at the start of all your interrupt handlers.
// Reimplement the macro in emlib_config.h to suit the needs of your RTOS.
/** Placeholder for optional interrupt handler entry code. This might be needed
 *  when working with an RTOS. */
#define CORE_INTERRUPT_ENTRY()
#endif

#if !defined(CORE_INTERRUPT_EXIT)
/** Placeholder for optional interrupt handler exit code. This might be needed
 *  when working with an RTOS. */
#define CORE_INTERRUPT_EXIT()
#endif

// Compile time sanity check.
#if (CORE_ATOMIC_METHOD != CORE_ATOMIC_METHOD_PRIMASK) \
  && (CORE_ATOMIC_METHOD != CORE_ATOMIC_METHOD_BASEPRI)
#error "em_core: Undefined ATOMIC IRQ handling strategy."
#endif

/*******************************************************************************
 **************************   STRUCTS   ****************************************
 ******************************************************************************/
/** A Cycle Counter Instance. */
typedef struct {
  uint32_t start;    /*!< Cycle counter at start of recording. */
  uint32_t cycles;   /*!< Cycles elapsed in last recording. */
  uint32_t max;      /*!< Max recorded cycles since last reset or init. */
} dwt_cycle_counter_handle_t;

/*******************************************************************************
 ***************************   LOCAL VARIABLES   *******************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#if (SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1)
// cycle counter to record atomic sections
dwt_cycle_counter_handle_t atomic_cycle_counter   = { 0 };
// cycle counter to record critical sections
dwt_cycle_counter_handle_t critical_cycle_counter = { 0 };
#endif

/** @endcond */

/*******************************************************************************
 ***************************   LOCAL FUNCTIONS   *******************************
 ******************************************************************************/

#if (SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1)
static void cycle_counter_start(dwt_cycle_counter_handle_t *handle);
static void cycle_counter_stop(dwt_cycle_counter_handle_t *handle);
#endif

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Disable interrupts.
 *
 *   Disable all interrupts by setting PRIMASK.
 *   (Fault exception handlers will still be enabled).
 ******************************************************************************/
SL_WEAK void CORE_CriticalDisableIrq(void)
{
  __disable_irq();
}

/***************************************************************************//**
 * @brief
 *   Enable interrupts.
 *
 *   Enable interrupts by clearing PRIMASK.
 ******************************************************************************/
SL_WEAK void CORE_CriticalEnableIrq(void)
{
  __enable_irq();
}

/***************************************************************************//**
 * @brief
 *   Enter a CRITICAL section.
 *
 *   When a CRITICAL section is entered, all interrupts (except fault handlers)
 *   are disabled.
 *
 * @return
 *   The value of PRIMASK register prior to the CRITICAL section entry.
 ******************************************************************************/
SL_WEAK CORE_irqState_t CORE_EnterCritical(void)
{
  CORE_irqState_t irqState = __get_PRIMASK();
  __disable_irq();
  if (irqState == 0U) {
#if (SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1)
    cycle_counter_start(&critical_cycle_counter);
#endif
  }
  return irqState;
}

/***************************************************************************//**
 * @brief
 *   Exit a CRITICAL section.
 *
 * @param[in] irqState
 *   The interrupt priority blocking level to restore to PRIMASK when exiting
 *   the CRITICAL section. This value is usually the one returned by a prior
 *   call to @ref CORE_EnterCritical().
 ******************************************************************************/
SL_WEAK void CORE_ExitCritical(CORE_irqState_t irqState)
{
  if (irqState == 0U) {
#if (SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1)
    cycle_counter_stop(&critical_cycle_counter);
#endif
    __enable_irq();
  }
}

/***************************************************************************//**
 * @brief
 *   Brief interrupt enable/disable sequence to allow handling of
 *   pending interrupts.
 *
 * @note
 *   Usually used within a CRITICAL section.
 ******************************************************************************/
SL_WEAK void CORE_YieldCritical(void)
{
  if ((__get_PRIMASK() & 1U) != 0U) {
    __enable_irq();
    __ISB();
    __disable_irq();
  }
}

/***************************************************************************//**
 * @brief
 *   Disable interrupts.
 *
 *   Disable interrupts with a priority lower or equal to
 *   @ref CORE_ATOMIC_BASE_PRIORITY_LEVEL. Sets core BASEPRI register
 *   to CORE_ATOMIC_BASE_PRIORITY_LEVEL.
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is @ref CORE_ATOMIC_METHOD_PRIMASK, this
 *   function is identical to @ref CORE_CriticalDisableIrq().
 ******************************************************************************/
SL_WEAK void CORE_AtomicDisableIrq(void)
{
#if (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
  __set_BASEPRI(CORE_ATOMIC_BASE_PRIORITY_LEVEL << (8UL - __NVIC_PRIO_BITS));
#else
  __disable_irq();
#endif // (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
}

/***************************************************************************//**
 * @brief
 *   Enable interrupts.
 *
 *   Enable interrupts by setting core BASEPRI register to 0.
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is @ref CORE_ATOMIC_METHOD_BASEPRI and PRIMASK
 *   is set (CPU is inside a CRITICAL section), interrupts will still be
 *   disabled after calling this function.
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is @ref CORE_ATOMIC_METHOD_PRIMASK, this
 *   function is identical to @ref CORE_CriticalEnableIrq().
 ******************************************************************************/
SL_WEAK void CORE_AtomicEnableIrq(void)
{
#if (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
  __set_BASEPRI(0);
#else
  __enable_irq();
#endif // (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
}

/***************************************************************************//**
 * @brief
 *   Enter an ATOMIC section.
 *
 *   When an ATOMIC section is entered, interrupts with priority lower or equal
 *   to @ref CORE_ATOMIC_BASE_PRIORITY_LEVEL are disabled.
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is @ref CORE_ATOMIC_METHOD_PRIMASK, this
 *   function is identical to @ref CORE_EnterCritical().
 *
 * @return
 *   The value of BASEPRI register prior to ATOMIC section entry.
 ******************************************************************************/
SL_WEAK CORE_irqState_t CORE_EnterAtomic(void)
{
#if (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
  CORE_irqState_t irqState = __get_BASEPRI();
  __set_BASEPRI(CORE_ATOMIC_BASE_PRIORITY_LEVEL << (8U - __NVIC_PRIO_BITS));
  if ((irqState & (CORE_ATOMIC_BASE_PRIORITY_LEVEL << (8U - __NVIC_PRIO_BITS)))
      != (CORE_ATOMIC_BASE_PRIORITY_LEVEL << (8U - __NVIC_PRIO_BITS))) {
#if (SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1)
    cycle_counter_start(&atomic_cycle_counter);
#endif
  }
  return irqState;
#else
  CORE_irqState_t irqState = __get_PRIMASK();
  __disable_irq();
  if (irqState == 0U) {
#if (SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1)
    cycle_counter_start(&atomic_cycle_counter);
#endif
  }
  return irqState;
#endif // (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
}

/***************************************************************************//**
 * @brief
 *   Exit an ATOMIC section.
 *
 * @param[in] irqState
 *   The interrupt priority blocking level to restore to BASEPRI when exiting
 *   the ATOMIC section. This value is usually the one returned by a prior
 *   call to @ref CORE_EnterAtomic().
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is set to @ref CORE_ATOMIC_METHOD_PRIMASK, this
 *   function is identical to @ref CORE_ExitCritical().
 ******************************************************************************/
SL_WEAK void CORE_ExitAtomic(CORE_irqState_t irqState)
{
#if (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
  if ((irqState & (CORE_ATOMIC_BASE_PRIORITY_LEVEL << (8U - __NVIC_PRIO_BITS)))
      != (CORE_ATOMIC_BASE_PRIORITY_LEVEL << (8U - __NVIC_PRIO_BITS))) {
#if (SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1)
    cycle_counter_stop(&atomic_cycle_counter);
#endif
  }
  __set_BASEPRI(irqState);
#else
  if (irqState == 0U) {
#if (SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1)
    cycle_counter_stop(&atomic_cycle_counter);
#endif
    __enable_irq();
  }
#endif // (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
}

/***************************************************************************//**
 * @brief
 *   Brief interrupt enable/disable sequence to allow handling of
 *   pending interrupts.
 *
 * @note
 *   Usully used within an ATOMIC section.
 *
 * @note
 *   If @ref CORE_ATOMIC_METHOD is @ref CORE_ATOMIC_METHOD_PRIMASK, this
 *   function is identical to @ref CORE_YieldCritical().
 ******************************************************************************/
SL_WEAK void CORE_YieldAtomic(void)
{
#if (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
  CORE_irqState_t basepri = __get_BASEPRI();
  if (basepri >= (CORE_ATOMIC_BASE_PRIORITY_LEVEL << (8U - __NVIC_PRIO_BITS))) {
    __set_BASEPRI(0);
    __ISB();
    __set_BASEPRI(basepri);
  }
#else
  if ((__get_PRIMASK() & 1U) != 0U) {
    __enable_irq();
    __ISB();
    __disable_irq();
  }
#endif // (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
}

/***************************************************************************//**
 * @brief
 *   Enter a NVIC mask section.
 *
 *   When a NVIC mask section is entered, specified NVIC interrupts
 *   are disabled.
 *
 * @param[out] nvicState
 *   Return NVIC interrupts enable mask prior to section entry.
 *
 * @param[in] disable
 *   A mask specifying which NVIC interrupts to disable within the section.
 ******************************************************************************/
void CORE_EnterNvicMask(CORE_nvicMask_t *nvicState,
                        const CORE_nvicMask_t *disable)
{
  CORE_CRITICAL_SECTION(
    *nvicState = *(CORE_nvicMask_t*)((uint32_t)&NVIC->ICER[0]);
    *(CORE_nvicMask_t*)((uint32_t)&NVIC->ICER[0]) = *disable;
    )
}

/***************************************************************************//**
 * @brief
 *   Disable NVIC interrupts.
 *
 * @param[in] disable
 *   A mask specifying which NVIC interrupts to disable.
 ******************************************************************************/
void CORE_NvicDisableMask(const CORE_nvicMask_t *disable)
{
  CORE_CRITICAL_SECTION(
    *(CORE_nvicMask_t*)((uint32_t)&NVIC->ICER[0]) = *disable;
    )
}

/***************************************************************************//**
 * @brief
 *   Set current NVIC interrupt enable mask.
 *
 * @param[out] enable
 *   A mask specifying which NVIC interrupts are currently enabled.
 ******************************************************************************/
void CORE_NvicEnableMask(const CORE_nvicMask_t *enable)
{
  CORE_CRITICAL_SECTION(
    *(CORE_nvicMask_t*)((uint32_t)&NVIC->ISER[0]) = *enable;
    )
}

/***************************************************************************//**
 * @brief
 *   Brief NVIC interrupt enable/disable sequence to allow handling of
 *   pending interrupts.
 *
 * @param[in] enable
 *   A mask specifying which NVIC interrupts to briefly enable.
 *
 * @note
 *   Usually used within an NVIC mask section.
 ******************************************************************************/
void CORE_YieldNvicMask(const CORE_nvicMask_t *enable)
{
  CORE_nvicMask_t nvicMask;

  // Get current NVIC enable mask.
  CORE_CRITICAL_SECTION(
    nvicMask = *(CORE_nvicMask_t*)((uint32_t)&NVIC->ISER[0]);
    )

  // Make a mask with bits set for those interrupts that are currently
  // disabled but are set in the enable mask.
#if (CORE_NVIC_REG_WORDS == 1)
  nvicMask.a[0] &= enable->a[0];
  nvicMask.a[0] = ~nvicMask.a[0] & enable->a[0];

  if (nvicMask.a[0] != 0) {
#elif (CORE_NVIC_REG_WORDS == 2)
  nvicMask.a[0] &= enable->a[0];
  nvicMask.a[1] &= enable->a[1];
  nvicMask.a[0] = ~nvicMask.a[0] & enable->a[0];
  nvicMask.a[1] = ~nvicMask.a[1] & enable->a[1];

  if ((nvicMask.a[0] != 0U) || (nvicMask.a[1] != 0U)) {
#elif (CORE_NVIC_REG_WORDS == 3)
  nvicMask.a[0] &= enable->a[0];
  nvicMask.a[1] &= enable->a[1];
  nvicMask.a[2] &= enable->a[2];
  nvicMask.a[0] = ~nvicMask.a[0] & enable->a[0];
  nvicMask.a[1] = ~nvicMask.a[1] & enable->a[1];
  nvicMask.a[2] = ~nvicMask.a[2] & enable->a[2];

  if ((nvicMask.a[0] != 0U) || (nvicMask.a[1] != 0U) || (nvicMask.a[2] != 0U)) {
#endif

    // Enable previously disabled interrupts.
    *(CORE_nvicMask_t*)((uint32_t)&NVIC->ISER[0]) = nvicMask;

    // Disable those interrupts again.
    *(CORE_nvicMask_t*)((uint32_t)&NVIC->ICER[0]) = nvicMask;
  }
}

/***************************************************************************//**
 * @brief
 *   Utility function to set an IRQn bit in a NVIC enable/disable mask.
 *
 * @param[in] irqN
 *   The IRQn_Type enumerator for the interrupt.
 *
 * @param[in,out] mask
 *   The mask to set the interrupt bit in.
 ******************************************************************************/
void CORE_NvicMaskSetIRQ(IRQn_Type irqN, CORE_nvicMask_t *mask)
{
  EFM_ASSERT(((int)irqN >= 0) && ((int)irqN < EXT_IRQ_COUNT));
  mask->a[(unsigned)irqN >> 5] |= 1UL << ((unsigned)irqN & 0x1FUL);
}

/***************************************************************************//**
 * @brief
 *   Utility function to clear an IRQn bit in a NVIC enable/disable mask.
 *
 * @param[in] irqN
 *   The IRQn_Type enumerator for the interrupt.
 *
 * @param[in,out] mask
 *   The mask to clear the interrupt bit in.
 ******************************************************************************/
void CORE_NvicMaskClearIRQ(IRQn_Type irqN, CORE_nvicMask_t *mask)
{
  EFM_ASSERT(((int)irqN >= 0) && ((int)irqN < EXT_IRQ_COUNT));
  mask->a[(unsigned)irqN >> 5] &= ~(1UL << ((unsigned)irqN & 0x1FUL));
}

/***************************************************************************//**
 * @brief
 *   Check whether the current CPU operation mode is handler mode.
 *
 * @return
 *   True if the CPU is in handler mode (currently executing an interrupt handler).
 *   @n False if the CPU is in thread mode.
 ******************************************************************************/
SL_WEAK bool CORE_InIrqContext(void)
{
  return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0U;
}

/***************************************************************************//**
 * @brief
 *   Check if a specific interrupt is disabled or blocked.
 *
 * @param[in] irqN
 *   The IRQn_Type enumerator for the interrupt to check.
 *
 * @return
 *   True if the interrupt is disabled or blocked.
 ******************************************************************************/
SL_WEAK bool CORE_IrqIsBlocked(IRQn_Type irqN)
{
  uint32_t irqPri, activeIrq;

#if (__CORTEX_M >= 3)
  uint32_t basepri;

  EFM_ASSERT((irqN >= MemoryManagement_IRQn)
             && (irqN < (IRQn_Type)EXT_IRQ_COUNT));
#else
  EFM_ASSERT((irqN >= SVCall_IRQn) && ((IRQn_Type)irqN < EXT_IRQ_COUNT));
#endif

  if ((__get_PRIMASK() & 1U) != 0U) {
    return true;                            // All IRQs are disabled.
  }

  if (CORE_NvicIRQDisabled(irqN)) {
    return true;                            // The IRQ in question is disabled.
  }

  irqPri  = NVIC_GetPriority(irqN);
#if (__CORTEX_M >= 3)
  basepri = __get_BASEPRI();
  if ((basepri != 0U)
      && (irqPri >= (basepri >> (8U - __NVIC_PRIO_BITS)))) {
    return true;                            // The IRQ in question has too low
  }                                         // priority vs. BASEPRI.
#endif

  // Check if already in an interrupt handler. If so, an interrupt with a
  // higher priority (lower priority value) can preempt.
  activeIrq = (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) >> SCB_ICSR_VECTACTIVE_Pos;
  if (activeIrq != 0U) {
    if (irqPri >= NVIC_GetPriority((IRQn_Type)(activeIrq - 16U))) {
      return true;                          // The IRQ in question has too low
    }                                       // priority vs. current active IRQ
  }

  return false;
}

/***************************************************************************//**
 * @brief
 *   Check if interrupts are disabled.
 *
 * @return
 *   True if interrupts are disabled.
 ******************************************************************************/
SL_WEAK bool CORE_IrqIsDisabled(void)
{
#if (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_PRIMASK)
  return (__get_PRIMASK() & 1U) == 1U;

#elif (CORE_ATOMIC_METHOD == CORE_ATOMIC_METHOD_BASEPRI)
  return ((__get_PRIMASK() & 1U) == 1U)
         || (__get_BASEPRI() >= (CORE_ATOMIC_BASE_PRIORITY_LEVEL
                                 << (8U - __NVIC_PRIO_BITS)));
#endif
}

/***************************************************************************//**
 * @brief
 *   Get the current NVIC enable mask state.
 *
 * @param[out] mask
 *   The current NVIC enable mask.
 ******************************************************************************/
void CORE_GetNvicEnabledMask(CORE_nvicMask_t *mask)
{
  CORE_CRITICAL_SECTION(
    *mask = *(CORE_nvicMask_t*)((uint32_t)&NVIC->ISER[0]);
    )
}

/***************************************************************************//**
 * @brief
 *   Get NVIC disable state for a given mask.
 *
 * @param[in] mask
 *   An NVIC mask to check.
 *
 * @return
 *   True if all NVIC interrupt mask bits are clear.
 ******************************************************************************/
bool CORE_GetNvicMaskDisableState(const CORE_nvicMask_t *mask)
{
  CORE_nvicMask_t nvicMask;

  CORE_CRITICAL_SECTION(
    nvicMask = *(CORE_nvicMask_t*)((uint32_t)&NVIC->ISER[0]);
    )

#if (CORE_NVIC_REG_WORDS == 1)
  return (mask->a[0] & nvicMask.a[0]) == 0U;

#elif (CORE_NVIC_REG_WORDS == 2)
  return ((mask->a[0] & nvicMask.a[0]) == 0U)
         && ((mask->a[1] & nvicMask.a[1]) == 0U);

#elif (CORE_NVIC_REG_WORDS == 3)
  return ((mask->a[0] & nvicMask.a[0]) == 0U)
         && ((mask->a[1] & nvicMask.a[1]) == 0U)
         && ((mask->a[2] & nvicMask.a[2]) == 0U);
#endif
}

/***************************************************************************//**
 * @brief
 *   Check if an NVIC interrupt is disabled.
 *
 * @param[in] irqN
 *   The IRQn_Type enumerator for the interrupt to check.
 *
 * @return
 *   True if the interrupt is disabled.
 ******************************************************************************/
bool CORE_NvicIRQDisabled(IRQn_Type irqN)
{
  CORE_nvicMask_t *mask;

  EFM_ASSERT(((int)irqN >= 0) && ((int)irqN < EXT_IRQ_COUNT));
  mask = (CORE_nvicMask_t*)((uint32_t)&NVIC->ISER[0]);
  return (mask->a[(unsigned)irqN >> 5U] & (1UL << ((unsigned)irqN & 0x1FUL)))
         == 0UL;
}

/***************************************************************************//**
 * @brief
 *   Utility function to get the handler for a specific interrupt.
 *
 * @param[in] irqN
 *   The IRQn_Type enumerator for the interrupt.
 *
 * @return
 *   The handler address.
 *
 * @note
 *   Uses the interrupt vector table defined by the current VTOR register value.
 ******************************************************************************/
void *CORE_GetNvicRamTableHandler(IRQn_Type irqN)
{
  EFM_ASSERT(((int)irqN >= -16) && ((int)irqN < EXT_IRQ_COUNT));
  return (void*)((uint32_t*)(((uint32_t*)SCB->VTOR)[(int)irqN + 16]));
}

/***************************************************************************//**
 * @brief
 *   Utility function to set the handler for a specific interrupt.
 *
 * @param[in] irqN
 *   The IRQn_Type enumerator for the interrupt.
 *
 * @param[in] handler
 *   The handler address.
 *
 * @note
 *   Uses the interrupt vector table defined by the current VTOR register value.
 ******************************************************************************/
void CORE_SetNvicRamTableHandler(IRQn_Type irqN, void *handler)
{
  EFM_ASSERT(((int)irqN >= -16) && ((int)irqN < EXT_IRQ_COUNT));
  ((uint32_t*)SCB->VTOR)[(int)irqN + 16] = (uint32_t)((uint32_t*)handler);
}

/***************************************************************************//**
 * @brief
 *   Initialize an interrupt vector table by copying table entries from a
 *   source to a target table.
 *
 * @note This function will set a new VTOR register value.
 *
 * @param[in] sourceTable
 *   The address of the source vector table.
 *
 * @param[in] sourceSize
 *   A number of entries in the source vector table.
 *
 * @param[in] targetTable
 *   The address of the target (new) vector table.
 *
 * @param[in] targetSize
 *   A number of entries in the target vector table.
 *
 * @param[in] defaultHandler
 *   An address of the interrupt handler used for target entries for which where there
 *   is no corresponding source entry (i.e., the target table is larger than the source
 *   table).
 *
 * @param[in] overwriteActive
 *   When true, a target table entry is always overwritten with the
 *   corresponding source entry. If false, a target table entry is only
 *   overwritten if it is zero. This makes it possible for an application
 *   to partly initialize a target table before passing it to this function.
 *
 ******************************************************************************/
void CORE_InitNvicVectorTable(uint32_t *sourceTable,
                              uint32_t sourceSize,
                              uint32_t *targetTable,
                              uint32_t targetSize,
                              void *defaultHandler,
                              bool overwriteActive)
{
  uint32_t i;

  // ASSERT on non SRAM-based target table.
  EFM_ASSERT(((uint32_t)targetTable >= SRAM_BASE)
             && ((uint32_t)targetTable < (SRAM_BASE + SRAM_SIZE)));

  // ASSERT if misaligned with respect to the VTOR register implementation.
#if defined(SCB_VTOR_TBLBASE_Msk)
  EFM_ASSERT(((uint32_t)targetTable & ~(SCB_VTOR_TBLOFF_Msk
                                        | SCB_VTOR_TBLBASE_Msk)) == 0U);
#else
  EFM_ASSERT(((uint32_t)targetTable & ~SCB_VTOR_TBLOFF_Msk) == 0U);
#endif

  // ASSERT if misaligned with respect to the vector table size.
  // The vector table address must be aligned at its size rounded up to nearest 2^n.
  EFM_ASSERT(((uint32_t)targetTable
              & ((1UL << (32UL - __CLZ((targetSize * 4UL) - 1UL))) - 1UL))
             == 0UL);

  for (i = 0; i < targetSize; i++) {
    if (overwriteActive) {                      // Overwrite target entries.
      if (i < sourceSize) {                     //   targetSize <= sourceSize
        targetTable[i] = sourceTable[i];
      } else {                                  //   targetSize > sourceSize
        targetTable[i] = (uint32_t)((uint32_t*)defaultHandler);
      }
    } else {                            // Overwrite target entries which are 0.
      if (i < sourceSize) {                     // targetSize <= sourceSize
        if (targetTable[i] == 0U) {
          targetTable[i] = sourceTable[i];
        }
      } else {                                  // targetSize > sourceSize
        if (targetTable[i] == 0U) {
          targetTable[i] = (uint32_t)((uint32_t*)defaultHandler);
        }
      }
    }
  }
  SCB->VTOR = (uint32_t)targetTable;
}

#if (SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1) || defined(DOXYGEN)
/***************************************************************************//**
 * @brief
 *   Start a recording.
 *
 * @param[in] handle
 *   Pointer to initialized counter handle.
 *
 * @note SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING must be enabled.
 ******************************************************************************/
static void cycle_counter_start(dwt_cycle_counter_handle_t *handle)
{
  handle->start = DWT->CYCCNT;
}

/***************************************************************************//**
 * @brief
 *   Stop a recording.
 *
 * @param[in] handle
 *   Pointer to initialized counter handle.
 *
 * @note SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING must be enabled.
 ******************************************************************************/
static void cycle_counter_stop(dwt_cycle_counter_handle_t *handle)
{
  handle->cycles = DWT->CYCCNT - handle->start;

  if (handle->cycles > handle->max) {
    handle->max = handle->cycles;
  }
}

/***************************************************************************//**
 * @brief
 *   Returns the max time spent in critical section.
 *
 * @return
 *   The max time spent in critical section.
 *
 * @note SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING must be enabled.
 ******************************************************************************/
uint32_t CORE_get_max_time_critical_section(void)
{
  return critical_cycle_counter.max;
}

/***************************************************************************//**
 * @brief
 *   Returns the max time spent in atomic section.
 *
 * @return
 *   The max time spent in atomic section.
 *
 * @note SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING must be enabled.
 ******************************************************************************/
uint32_t CORE_get_max_time_atomic_section(void)
{
  return atomic_cycle_counter.max;
}

/***************************************************************************//**
 * @brief
 *   Clears the max time spent in atomic section.
 *
 * @note SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING must be enabled.
 ******************************************************************************/
void CORE_clear_max_time_critical_section(void)
{
  critical_cycle_counter.max = 0;
}

/***************************************************************************//**
 * @brief
 *   Clears the max time spent in atomic section.
 *
 * @note SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING must be enabled.
 ******************************************************************************/
void CORE_clear_max_time_atomic_section(void)
{
  atomic_cycle_counter.max = 0;
}
#endif //(SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1)

/** @} (end addtogroup core) */
