################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gecko_sdk_4.4.3/platform/driver/debug/src/sl_debug_swo.c 

OBJS += \
./gecko_sdk_4.4.3/platform/driver/debug/src/sl_debug_swo.o 

C_DEPS += \
./gecko_sdk_4.4.3/platform/driver/debug/src/sl_debug_swo.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_4.4.3/platform/driver/debug/src/sl_debug_swo.o: ../gecko_sdk_4.4.3/platform/driver/debug/src/sl_debug_swo.c gecko_sdk_4.4.3/platform/driver/debug/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DEFR32MG24A010F1536GM48=1' '-DSL_APP_PROPERTIES=1' '-DSL_BOARD_NAME="BRD4186A"' '-DSL_BOARD_REV="A04"' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DHFXO_FREQ=39000000' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' '-DOPENTHREAD_ENABLE_VENDOR_EXTENSION=1' '-DCORTEXM3=1' '-DCORTEXM3_EFM32_MICRO=1' '-DCORTEXM3_EFR32=1' '-DPHY_RAIL=1' '-DPLATFORM_HEADER="platform-header.h"' '-DOPENTHREAD_CONFIG_ENABLE_BUILTIN_MBEDTLS=0' '-DSPINEL_PLATFORM_HEADER="spinel_platform.h"' '-D_GNU_SOURCE=1' '-DINCLUDE_xTimerPendFunctionCall=1' '-DRADIO_CONFIG_DMP_SUPPORT=1' '-DOPENTHREAD_CORE_CONFIG_PLATFORM_CHECK_FILE="openthread-core-efr32-config-check.h"' '-DOPENTHREAD_PROJECT_CORE_CONFIG_FILE="openthread-core-efr32-config.h"' '-DOPENTHREAD_CONFIG_FILE="sl_openthread_generic_config.h"' '-DOPENTHREAD_CONFIG_PLATFORM_KEY_REFERENCES_ENABLE=0' '-DOPENTHREAD_RADIO=1' '-DOPENTHREAD_SPINEL_CONFIG_OPENTHREAD_MESSAGE_ENABLE=0' '-DSL_OPENTHREAD_STACK_FEATURES_CONFIG_FILE="sl_openthread_features_config.h"' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=1' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DSLI_RADIOAES_REQUIRES_MASKING=1' -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\config" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\autogen" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\Device\SiliconLabs\EFR32MG24\Include" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\common\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\hardware\board\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\bootloader" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\bootloader\api" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\CMSIS\Core\Include" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\CMSIS\RTOS2\Include" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\hardware\driver\configuration_over_swo\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\driver\debug\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\service\device_init\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\emdrv\dmadrv\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\emdrv\common\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\emlib\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\freertos\cmsis\Include" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\freertos\kernel\include" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\freertos\kernel\portable\GCC\ARM_CM33_NTZ\non_secure" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\security\sl_component\sl_mbedtls_support\config" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\security\sl_component\sl_mbedtls_support\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\mbedtls\include" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\mbedtls\library" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\service\mpu\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\hardware\driver\mx25_flash_shutdown\inc\sl_mx25_flash_shutdown_usart" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\emdrv\nvm3\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\protocol\openthread\src\legacy_hal\include" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\service\legacy_hal\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\include" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\include\openthread" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\src\core" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\src\lib" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\third_party\tcplp" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\src" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\src\ncp" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\src\lib\spinel" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\src\lib\hdlc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\protocol\openthread\platform-abstraction\rtos" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\examples\platforms" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\third_party\openthread\examples\platforms\utils" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\protocol\openthread\platform-abstraction\efr32" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\protocol\openthread\platform-abstraction\include" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\peripheral\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\security\sl_component\sl_psa_driver\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\common" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\protocol\ble" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\protocol\ieee802154" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\protocol\wmbus" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\protocol\zwave" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\chip\efr32\efr32xg2x" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\protocol\sidewalk" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\plugin\rail_util_built_in_phys\efr32xg24" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\plugin\pa-conversions" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\plugin\pa-conversions\efr32xg24" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\plugin\rail_util_pti" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\radio\rail_lib\plugin\rail_util_rssi" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\security\sl_component\se_manager\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\security\sl_component\se_manager\src" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\plugin\security_manager" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\util\silicon_labs\silabs_core\memory_manager" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\common\toolchain\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\service\system\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\service\sleeptimer\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\security\sl_component\sl_protocol_crypto\src" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\emdrv\uartdrv\inc" -I"C:\Users\secerdan\SimplicityStudio\v5_workspace_11\ot-rcp-base\gecko_sdk_4.4.3\platform\service\udelay\inc" -Og -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_openthread_rtos_config.h -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse -Werror=unused-variable -Werror=unused-function --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.4.3/platform/driver/debug/src/sl_debug_swo.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


