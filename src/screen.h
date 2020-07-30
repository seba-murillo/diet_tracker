/*
 * Screen.h
 *
 *  Created on: Jul 5, 2020
 *      Author: seba
 */

#ifndef SCREEN_H_
#define SCREEN_H_
// codes
#define RESET		"\u001B[0m" // reset
#define FULL_RESET	"\u001B[0m\u001B[K" // reset
#define BOLD		"\u001B[1m" // bold
#define UNDERLINE	"\u001B[4m" // underline
#define ENDL		RESET "\n" // new line
#undef TAB
#define TAB			"    "
// formats
//#define OK		"\u001B[1m[\u001B[32mOK\u001B[0m\u001B[1m]"
#define COLOR_OK		"\u001B[32m" // green
#define COLOR_WARNING	"\u001B[33m" // yellow
#define COLOR_FAIL		"\u001B[31m" // red
#define OK			BOLD "[" COLOR_OK "OK" RESET BOLD "]" ENDL
#define FAIL		BOLD "[" COLOR_FAIL "FAIL" RESET BOLD "]" ENDL
// colors
#define COLOR_AMOUNT		"\u001B[35m" // purple
#define COLOR_FOOD			"\u001B[34m" // blue
#define COLOR_DAY			"\u001B[36m" // cyan
#define COLOR_TABLE_TOTAL	"\u001B[33m" // yellow
#define COLOR_TABLE_TARGET	"\u001B[34m" // blue
#define COLOR_HELP			"\u001B[38;5;209m" // orange
#define COLOR_PROFILE		"\u001B[33m" // yellow
#define COLOR_SYNTAX		"\u001B[38;5;209m" // orange
#define COLOR_TABLE_BG		"\u001B[48;5;234m" // grey BG
#define COLOR_DATE			"\u001B[36m" // cyan
#define COLOR_INFO_TAG		"\u001B[33m" // yellow
#define COLOR_INFO_DATA		"\u001B[36m" // cyan
#define COLOR_WEIGHT		"\u001B[33m" // yellow
// spacing
#define SPACING_DEFAULT		30
// other
#define CLEAR_LINES	100
#define FORMAT_TAG TAB COLOR_TABLE_BG BOLD COLOR_INFO_TAG
#define FORMAT_DATA RESET COLOR_TABLE_BG COLOR_INFO_DATA
#define ERROR BOLD COLOR_FAIL "ERROR: "
#define FORMAT_ERROR RESET COLOR_FAIL
#define SYNTAX BOLD COLOR_SYNTAX TAB "- incorrect syntax -> " RESET COLOR_SYNTAX

using namespace std;

#endif /* SCREEN_H_ */
