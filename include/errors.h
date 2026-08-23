/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

/// @file errors.h
/// @brief Header for the error handler.

#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include "codes.h"

enum { LOG_PATH_MAX = 512 };

/// @brief The log config struct. Describes how to log.
typedef struct {
    bool enabled;
    bool rotate;
    bool build;
    bool runtime;
    bool controller;
    bool errors;
    int retentionDays;
    int retentionAction;
    char path[LOG_PATH_MAX];
    char archivePath[LOG_PATH_MAX];
} LogConfig;

/// @brief Outputs an error to the log file.
/// @param msg The error message.
/// @param line Line of the error.
/// @param collumn Collumn of the error.
void _logError(const char *msg, int line, int collumn);

/// @brief Throw an error and save to memory. Ensures fatal errors are handled.
/// @param wf How the error will be printed. Specific to the caller.
/// @param code The error code to throw.
/// @param line The line of the error.
/// @param col The collumn of the error.
/// @param filename The file currently open.
/// @note @p line and @p col may be zero for controller based errors (eg. file open error).
/// @note @p line may be used as the vm's ip at the throw-time.
/// @note For errors not related to parsing/running, please keep @p filename as NULL.
void lraise(WhereFrom wf, ErrorCode code, int line, int col, char filename[512]);

/// @brief A helper to call all current errors.
/// @deprecated Retained for backwards compatability.
/// lraise now handles this automatically.
/// @note This function is intended for use only within errors.c.
/// @warning May cause unintended side effects.
void callAllErr(void);

/// @brief A boolean to determine whether errors occured during running.
extern bool isErr;

/// @brief Sets the current log config as the passed config.
/// @param config The log config to set as the current.
void setLogConfig(LogConfig config);

/// @brief Returns the current log config.
/// @return The current log config.
const LogConfig *getLogConfig(void);

/// @brief Flushes and closes the logfile.
void closeLog(void);

/// @brief Initialises the logg.
/// @param filename 
void initLog(const char *filename);

/// @brief A function to print a message to the logfile.
/// @param msg The content to print.
/// @note Prepends "[BUILD][LEXER]" to the @p msg.
void logBuildLexer(const char *msg);

/// @brief A function to print a message to the logfile.
/// @param msg The content to print.
/// @note Prepends "[BUILD][PARSER]" to the @p msg.
void logBuildParser(const char *msg);

/// @brief A function to print a message to the logfile.
/// @param msg The content to print.
/// @note Prepends "[RUNTIME]" to the @p msg.
void logRuntime(const char *msg);

/// @brief A function to print a message to the logfile.
/// @param msg The content to print.
/// @note Prepends "[CONTROLLER]" to the @p msg.
void logController(const char *msg);

/// @brief Causes all logging functions to fall through.
/// @note Use to ensure maximum speed or during testing.
void inertLogs(void);

#endif
