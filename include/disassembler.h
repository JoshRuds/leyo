/*
 * Copyright (c) 2026 Leyo Contributors
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdint.h>
#include <stdio.h>

/// @brief Gets the string name of an operand.
/// @param op The current operand. 
/// @return The name of the @p op as a string.
const char* opcode_name(uint8_t op);

/// @brief A helper to get the amount of bytes are used by an operand.
/// @param op The current operand. 
/// @return How many bytes are used by @p op operand.
int opcode_has_operand(uint8_t op);

/// @brief A helper that adds front padding to a number.
/// @param highest The highest number.
/// @param number The number to format.
/// @return The number as a string that is always the width of @p highest .
char *formatNumber(int highest, int number);

/// @brief Prints code as a human readable text.
/// @param code The bytecode.
/// @param size The size of the byte array.
void disassemble(const uint8_t* code, size_t size);

/// @brief Prints a array of bytes as their hex values.
/// @param code The bytes to print.
/// @param size The size of the byte array.
void disassembleHex(const uint8_t* code, size_t size);

/// @brief A handler for all disassembely-based actions.
/// @param filename The file to disassemble.
/// @param flag_justHex A bool to decide which disassemble function to use.
/// @param flag_head A bool to decide whether to include the header in the disassembled print-out.
/// @retval 0 Success.
/// @retval 1 Failure.
int dis(char *filename, bool flag_justHex, bool flag_head);

#endif
