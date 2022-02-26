#include <AndroidProp.h>

const char *getAndroidMac(){
    char *prop_value = (char *) malloc(PROP_VALUE_MAX);
    __system_property_get("ro.serialno", prop_value);
    return prop_value;
}

int getAndroidApiLevel(){
    char prop_value[PROP_VALUE_MAX];
    __system_property_get("ro.build.version.sdk", prop_value);
    return atoi(prop_value);
}