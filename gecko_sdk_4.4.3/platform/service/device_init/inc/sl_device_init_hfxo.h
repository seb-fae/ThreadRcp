/***************************************************************************//**
 * @file
 * @brief Device initialization for HFXO.
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef SL_DEVICE_INIT_HFXO_H
#define SL_DEVICE_INIT_HFXO_H

#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup device_init
 * @{
 * @addtogroup device_init_hfxo HFXO Initialization
 * @brief Initialize the HFXO oscillator.
 * @details
 * Configure the HFXO for crystal or externally-generated waveform operation.
 *
 * When operating in crystal mode, the value of the variable tuning capacitor
 * C<sub>tune</sub> is sourced from a prioritized list of locations:
 *
 * 1. Device-specific tuning value in Device Information page (factory calibrated PCB and SiP modules with integrated HFXO)
 * 2. Device-specific tuning value in User Data page at offset 0x100 (manufacturing token `TOKEN_MFG_CTUNE`)
 * 3. Application-specific tuning value in configuration header `sl_device_init_hfxo_config.h`
 * 4. Default value in EMLIB initialization struct for HFXO
 *
 * If using Simplicity Studio, this configuration header is also configurable through the Project Configurator by selecting the
 * "Device Init: HFXO" configuration component.
 * @{
 */

/**
 * Initialize HFXO
 *
 * @details
 * Configure the HFXO for crystal or externally-generated waveform operation.
 *
 * When operating in crystal mode, the value of the variable tuning capacitor
 * C<sub>tune</sub> is sourced from a prioritized list of locations:
 *
 * 1. Device-specific tuning value in Device Information page (PCB and SiP modules with integrated HFXO)
 * 2. Device-specific tuning value in User Data page at offset 0x100 ("manufacturing token")
 * 3. Application-specific tuning value in configuration header `sl_device_init_hfxo_config.h`
 * 4. Default value in EMLIB initialization struct for HFXO
 *
 * @return Status code
 * @retval SL_STATUS_OK HFXO started successfully
 */
sl_status_t sl_device_init_hfxo(void);

/**
 * @} device_init_hfxo
 * @} device_init
 */

#ifdef __cplusplus
}
#endif

#endif // SL_DEVICE_INIT_HFXO_H
