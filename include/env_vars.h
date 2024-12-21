//
// Created by fernando on 07/03/2022.
//
#ifndef LOGHELPER_ENV_VARS_H
#define LOGHELPER_ENV_VARS_H
/**
 * Variables that will be set by CMake
 * DO NOT change them unless you know
 * what you are doing!!!
 */
// Config file info
// Keys to be extracted from config file

// directory that the logs will be saved locally
#define LOG_DIR_KEY "logdir"
// Path to the configuration files that log helper will save
#define VAR_DIR_KEY "vardir"
// key to load ecc verification data
#define ECC_INFO_KEY "eccinfofile"
// Config for UDP logging
#define SERVER_IP_KEY "serverip"
#define SERVER_PORT_KEY "serverport"
// Define the logging type at the config file
#define LOGGING_TYPE "loggingtype"
// // Deprecated: Command that log_helper lib will send to the watchdog
// #define SIGNAL_CMD_KEY "signalcmd"

#ifndef CONFIG_FILE_PATH
#error "CONFIG_FILE_PATH not set check you CMake configurations"
#endif


#endif //LOGHELPER_ENV_VARS_H
