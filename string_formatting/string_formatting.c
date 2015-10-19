/**
 * Examples of string formatting for different data types
 * Uses C99 dialect, so compile with -std=gnu99 or -std=c99
 * or via IDE-s Visual Studio 2013 (and higher) / DevC++5.7 (and higher)
 */
#include <stdlib.h>
#include <string.h> // strlen
#include <stdio.h>  // printf, sprintf
#include <wchar.h>  // wprintf, wide string functions, UTF8 -> UCS2 etc.



#if _MSC_VER && _MSC_VER < 1900
	#define snprintf(buffer, maxCount, fmt, ...) _snprintf_s(buffer, maxCount, _TRUNCATE, fmt, __VA_ARGS__)
#endif



// Formatting to a string buffer instead of STDOUT or FILE
// snprintf - the safest string formatting function. Always prefer this function.
// http://www.cplusplus.com/reference/cstdio/snprintf/
void format_to_string()
{
	char stringBuffer[128]; // destination string buffer

	// convenient string formatting - better, easier and more efficient than
	// string concatenation
	int formattedLength = snprintf(stringBuffer, sizeof stringBuffer,
		"Formatted %s with values x=%d y=%d",  "string", 10, 20);

	printf("-------------------------------\n");
	printf("Formatted string length = %d\n", formattedLength);
	printf("Result: \"%s\"\n", stringBuffer);


	char buffer[28]; // destination string buffer (obviously too small)
	int len = snprintf(buffer, sizeof buffer,
		"Formatted %s with values x=%d y=%d", "string", 10, 20);

	// if len >= sizeof buffer, then end of string is truncated
	int actualLength = (len >= sizeof buffer) ? strlen(buffer): len;
	printf("Formatted string length = %d\n", len);
	printf("Actual    string length = %d\n", actualLength);
	printf("Result: \"%s\"\n", buffer);
}



// Formatting of several integer types
// http://www.cplusplus.com/reference/cstdio/printf/
void format_integer_types()
{
	char  int8  = -120;
	short int16 = -12000;
	int   int32 = -128768;
	long long int64 = -1287680975631242ll;

	printf("-------------------------------\n");

	// print regular signed integers.
	printf("int8   = %d\n", int8);
	printf("int16  = %d\n", int16);
	printf("int32  = %d\n", int32);
	printf("int64  = %lld\n", int64);   // only 64-bit values require %lld size specifier on x86

	// now interpret these signed integers as unsigned and lets see what happens
	printf("uint8  = %u\n", int8);
	printf("uint16 = %u\n", int16);
	printf("uint32 = %u\n", int32);
	printf("uint64 = %llu\n", int64);   // again 64-bit value requires long long size specifier

	// perhaps better explained if we print these as HEX strings
	printf("hex8   = 0x%2x\n", int8);   // using WIDTH specifier on HEX string to always get 2 digits
	printf("hex16  = 0x%4x\n", int16);
	printf("hex32  = 0x%8x\n", int32);
	printf("hex64  = 0x%16llx\n", int64);

	printf("-------------------------------\n");
	printf("Left justified:   |%-12d|\n", int32);    // specify width and justification
	printf("Right justified:  |%12d|\n", int32);     // right justified is the default
	printf("Variable justify: |%*d|\n", -12, int32); // justify is specified via variable
	printf("Pad with zeroes:  |%012d|\n", int32);    // pad with zeroes instead of spaces

	printf("Sign specifier:   |%+d|\n", 10);	 // %+d  the sign is always printed
	printf("Align specifier:  |% d|\n", 20);	 // % d  a space is inserted if there's no sign
}



// Different formatting options for FLOAT and DOUBLE types
// http://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html
void format_float_types()
{
	float precise = 60.0f;
	float irrat_1 = 3.14159265358979323846f;
	float irrat_2 = 0.0000119999f;

	// simplest float format - no rectification done
	printf("-------------------------------\n");
	printf("regular precise   %f \n", precise); // default %f is %.6f
	printf("regular irrat_1   %f \n", irrat_1);
	printf("regular irrat_2   %f \n", irrat_2);

	// decimal precision and minimum width specifiers (decimal rounding)
	printf("-------------------------------\n");
	printf("rounded precise   %5.3f \n", precise);
	printf("rounded irrat_1   %5.3f \n", irrat_1);
	printf("rounded irrat_2   %5.3f \n", irrat_2);

	// scientific representation
	printf("-------------------------------\n");
	printf("scienti precise   %e \n", precise);
	printf("scienti irrat_1   %e \n", irrat_1);
	printf("scienti irrat_2   %e \n", irrat_2);

	// shortest possible presentation
	printf("-------------------------------\n");
	printf("short   precise   %g \n", precise);
	printf("short   irrat_1   %g \n", irrat_1);
	printf("short   irrat_2   %g \n", irrat_2);
	printf("-------------------------------\n");
	printf("short5  precise   %.5g \n", precise);
	printf("short5  irrat_1   %.5g \n", irrat_1);
	printf("short5  irrat_2   %.5g \n", irrat_2);
	printf("-------------------------------\n");

	void printfloat(const char*, float);
	printfloat("custom  precise   ", precise);
	printfloat("custom  irrat_1   ", irrat_1);
	printfloat("custom  irrat_2   ", irrat_2);
}



// probably the only 'good' way of displaying 'readable' values
// is to check for unsuitable results (prevent scientific notation)
void printfloat(const char* text, float value)
{
	if (value < 0.0001f) printf("%s%.6f\n", text, value);
	else		 printf("%s%g\n",   text, value);
}



// Formatting different string types and explicitly overriding string length
// http://www.joelonsoftware.com/articles/Unicode.html
// UTF8 is the best. UCS2 is evil. Period.
void format_string_types()
{
	char    utf_string[128]; // ASCII or UTF8 string, depending on our interpretation
	wchar_t wcs_string[128]; // wide UCS2 or UTF16 string on WIN32, UTF32

	
	// size of wchar_t varies on platforms. Windows 2 bytes, Linux 4 bytes.
	printf("-------------------------------\n");
	printf("sizeof wchar_t = %d\n", sizeof(wchar_t));
	
	// first we initialize the strings, could also be done with strcpy & wcscpy
	snprintf(utf_string, 128, "ASCII|UTF8_string");
	_snwprintf(wcs_string, 128, L"UTF16|UCS2_string");


	// print out using the designed printf / wprintf functions
	printf(  "printf  utf_string = %s\n", utf_string);
	wprintf(L"wprintf wcs_string = %ls\n", wcs_string); // l(wide) specifier required on some platforms :/


	// @warning using ls and hs specifiers is the safest and most portable way of printing mixed strings
	//	  MSVC uses %s and %S to differentiate char width
	//	  GCC determines char width by function name, so printf %s = char, wprintf %s = wchar_t
	// by using width specifiers l(wide) and h(narrow), we can print mixed types of strings
	printf(  "printf  utf_string = %ls\n", wcs_string);
	wprintf(L"wprintf wcs_string = %hs\n", utf_string);


	// output string length specifier: %.*s
	printf(  "printf  utf_string = %.*s\n", 6, utf_string);
	wprintf(L"wprintf wcs_string = %.*ls\n", 6, wcs_string);


	// convert UCS2(win32) or UTF32(linux) WIDE STRING to an UTF8 string (NOT NULL TERMINATED)
	char converted[160];
	int converted_len = wcstombs(converted, wcs_string, wcslen(wcs_string));
	printf( "printf  converted wcs_string = %.*s\n", converted_len, converted);


	// reverse conversion UTF8 -> WIDE STRING (NOT NULL TERMINATED)
	wchar_t reverted[128];
	int reverted_len = mbstowcs(reverted, converted, converted_len);
	wprintf(L"wprintf reverted  wcs_string = %.*ls\n", reverted_len, reverted);
}



int main(int argc, char** argv)
{
	format_to_string();
	format_integer_types();
	format_float_types();
	format_string_types();
	
	#if _MSC_VER // pause VisualC before exit
		system("pause");
	#endif
	return 0;
}

