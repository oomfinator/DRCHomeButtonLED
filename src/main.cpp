#include "main.hpp"
#include <nn/acp.h>

#include <wups.h>
#include <wups/config/WUPSConfigCategory.h>
#include <wups/config/WUPSConfigItemMultipleValues.h>
#include <wups/config_api.h>

WUPS_PLUGIN_NAME("DRCHomeButtonLED");
WUPS_PLUGIN_DESCRIPTION("Turn the home button LED on");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("pai");
WUPS_PLUGIN_LICENSE("GPLv3");
WUPS_USE_STORAGE("Home Button LED");

#define PATTERN_CONFIG_ID "pattern"

static uint32_t pid;
DrcLedPattern sPattern = DRC_LED_ON;

void patternChanged(ConfigItemMultipleValues* item, uint32_t newValue) {
    sPattern = static_cast<DrcLedPattern>(newValue);
    WUPSStorageAPI::Store(item->identifier, sPattern);
    nn::acp::TurnOnDrcLed(pid, sPattern);
}

WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle) {
    WUPSConfigCategory root(rootHandle);
    constexpr WUPSConfigItemMultipleValues::ValuePair patternValues[] = {
        {DRC_LED_ON, "On"},
        {DRC_LED_ONE_TICK, "1 tick"},
        {DRC_LED_TWO_TICKS, "2 ticks"},
        {DRC_LED_THREE_TICKS, "3 ticks"},
        {DRC_LED_OFF, "Off"},
    };

    root.add(WUPSConfigItemMultipleValues::CreateFromValue(
        PATTERN_CONFIG_ID, "Pulse pattern", DRC_LED_ON, sPattern,
        patternValues, patternChanged));

    return WUPSCONFIG_API_CALLBACK_RESULT_SUCCESS;
}

void ConfigMenuClosedCallback() {
    WUPSStorageAPI::SaveStorage();
}

INITIALIZE_PLUGIN() {
    pid = GetPersistentId();
    
    WUPSStorageAPI::GetOrStoreDefault(PATTERN_CONFIG_ID, sPattern, DRC_LED_ON);
    WUPSStorageAPI::SaveStorage();

    WUPSConfigAPIOptionsV1 configOptions = { .name = "Home Button LED" };
    WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback);
    
    nn::acp::TurnOnDrcLed(pid, sPattern);
}

DEINITIALIZE_PLUGIN() {
    nn::acp::TurnOffDrcLed();
}

ON_APPLICATION_START() {
    nn::acp::TurnOnDrcLed(pid, sPattern);
}
