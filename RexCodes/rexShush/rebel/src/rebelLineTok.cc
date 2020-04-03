/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.01  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "rebel.h"
#include "abimport.h"
#include "proto.h"

rebelLineTok::rebelLineTok()
{
}

rebelLineTok::~rebelLineTok()
{
}

char *rebelLineTok::tok(char *string)
{
	char *start;
	char terminator;

	/* if a string is passed in, initialize the currenPosition and start pointers */
	if(string) {
		_currentPosition = string;
		start = _currentPosition;
		_endPosition = _currentPosition + strlen(string);
	}

	/* if NULL is passed in set start pointer to 1 after _currentPosition */
	else start = _currentPosition + 1;

	/* scan the string until a newline or NULL is found */
	while((*_currentPosition != '\n') && (*_currentPosition != '\r')) {
		++_currentPosition;

		// if the _currentPosition pointer gets past the end of the string
		// of if the string ends with out a carriage return or newline
		if((_currentPosition >= _endPosition) || (*_currentPosition == (char)NULL)) {
			if(string) return(string);
			else if(strlen(start)) {
				strncpy(_returnString, start, 1022);
				strcat(_returnString, "\n");
				return(_returnString);
			}
			else {
				return((char *)NULL);
			}
		}
	}

	// save the terminator
	terminator = *_currentPosition;

	/* change the newline to NULL to terminate the string */
	*_currentPosition = (char)NULL;

	// Microsoft editors terminate lines with two characters (\n and \r)
	// while unix editors terminate lines with one character (\n);
	// the previous while loop will exit after finding one of the terminators
	// these test find the second if the file was generated on a Microsoft editor
	if(terminator == '\r') {
		if(*(_currentPosition + 1) == '\n') {
			++_currentPosition;				// increment the pointer to the newline
			*_currentPosition = (char)NULL;	// and replace it with a NULL
		}
	}
	else if(terminator == '\n') {
		if(*(_currentPosition + 1) == '\r') {
			++_currentPosition;				// increment the pointer to the newline
			*_currentPosition = (char)NULL;	// and replace it with a NULL
		}
	}

	/* if the string from start to currentposition is not NULL, return the string */
	if(strlen(start)) {
		strcpy(_returnString, start);
		strcat(_returnString, "\n");
		return(_returnString);
	}

	/* otherwise return a newline */
	else {
		strcpy(_returnString, "\n");
		return(_returnString);
	}
}

void rebelLineTok::extractString(string& line, string& tmp, string fd, string nd)
{
	size_t f;
	size_t n;

	tmp.erase(tmp.begin(), tmp.end());

	if((f = line.find(fd)) != string::npos) {
		line.erase(0, (f + 1));

		tmp.insert(tmp.end(), line.begin(), line.end());
		if((n = tmp.find(nd)) != string::npos) tmp.erase(n);
		else if((n = tmp.find('\n')) != string::npos) tmp.erase(n);
	}

	return;
}
