#include <string.h>

char *rexlinetok(char *string)
{
	static char returnString[128];
	static char *currentPosition;
	char *start;

	/* if a string is passed in, initialize the currenPosition and start pointers */
	if(string) {
		currentPosition = string;
		start = currentPosition;
	}

	/* if NULL is passed in set start pointer to 1 after currentPosition */
	else start = currentPosition + 1;

	/* scan the string until a newline or NULL is found */
	while(*++currentPosition != '\n') {
		if(!*currentPosition) return((char *)NULL);
	}

	/* change the newline or NULL to NULL */
	*currentPosition = (char)NULL;

	/* if the string from start to currenposition is not NULL, return the string */
	if(strlen(start)) {
		strcpy(returnString, start);
		*currentPosition = '\n';	/* reset the newline */
		return(returnString);
	}

	/* otherwise return a newline */
	else {
		strcpy(returnString, "\n");
		*currentPosition = '\n';
		return(returnString);
	}
}

