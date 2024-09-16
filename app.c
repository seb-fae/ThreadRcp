/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include <assert.h>
#include <openthread-core-config.h>
#include <openthread/config.h>

#include <openthread/ncp.h>
#include <openthread/diag.h>
#include <openthread/tasklet.h>

#include "openthread-system.h"
#include "app.h"

#include "reset_util.h"
ChannelAccessFailure

#ifdef BUILD_APP
/**
 * This function initializes the NCP app.
 *
 * @param[in]  aInstance  The OpenThread instance structure.
 *
 */
extern void otAppNcpInit(otInstance *aInstance);

otInstance *otGetInstance(void)
{
    return sInstance;
}


static otInstance* sInstance = NULL;

#endif

otInstance *sInstance;

void otSysProcessDrivers(otInstance *aInstance)
{
    sInstance = aInstance;

    // should sleep and wait for interrupts here
#if defined(SL_CATALOG_OT_RCP_GP_INTERFACE_PRESENT)
    efr32GpProcess();
#endif

#if OPENTHREAD_CONFIG_NCP_HDLC_ENABLE
    efr32UartProcess();
#elif OPENTHREAD_CONFIG_NCP_CPC_ENABLE
    efr32CpcProcess();
#elif OPENTHREAD_CONFIG_NCP_SPI_ENABLE
    efr32SpiProcess();
#endif
    efr32RadioProcess(aInstance);

    // See alarm.c: Wrapped in a critical section
    efr32AlarmProcess(aInstance);
}


otInstance* sl_ot_create_instance(void)
{
  otInstance* instance;
#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
    size_t   otInstanceBufferLength = 0;
    uint8_t *otInstanceBuffer       = NULL;

    // Call to query the buffer size
    (void)otInstanceInit(NULL, &otInstanceBufferLength);

    // Call to allocate the buffer
    otInstanceBuffer = (uint8_t *)malloc(otInstanceBufferLength);
    assert(otInstanceBuffer);

    // Initialize OpenThread with the buffer
    instance = otInstanceInit(otInstanceBuffer, &otInstanceBufferLength);
#else
    instance = otInstanceInitSingle();
#endif
    assert(instance);
    return instance;
}

#ifdef BUILD_APP

void sl_ot_ncp_init(void)
{
    otAppNcpInit(sInstance);
}

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/

void app_init(void)
{
    OT_SETUP_RESET_JUMP(argv);
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
void app_process_action(void)
{
    otTaskletsProcess(sInstance);
    otSysProcessDrivers(sInstance);
}

/**************************************************************************//**
 * Application Exit.
 *****************************************************************************/
void app_exit(void)
{
    otInstanceFinalize(sInstance);
#if OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
    free(otInstanceBuffer);
#endif
    // TO DO : pseudo reset?
}
#endif

