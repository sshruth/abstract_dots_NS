/* ECODE.H
**
** Revision history:
**	5/31/01 created by jig
*/

#ifndef _ECODE_H_
#define _ECODE_H_

/* PUBLIC ROUTINE PROTOTYPES */
int		ec_send_code					(long);
int 	ec_send_code_hi				(long);
int 	ec_send_code_lo				(long);

void	ec_send_dio						(unsigned long);
void 	ec_send_dio_hi					(unsigned long);
void 	ec_send_dio_lo					(unsigned long);

void	ec_send_matlab_command		(int, int);
void 	ec_send_matlab_command_hi	(int, int);
void 	ec_send_matlab_command_lo	(int, int);

void 	ec_send_matlab_arg			(int, int);
void 	ec_send_matlab_arg_hi		(int, int);
void 	ec_send_matlab_arg_lo		(int, int);

void 	ec_send_hi						(unsigned short);
void 	ec_send_lo						(unsigned short);

/* Drop ecodes for task parameters */
#define TAG1 7500 /* for parameters between -500 and 500 */
#define EC_TAG1(I,p) {ec_send_code((I)); ec_send_code(TAG1+(p)); }
#define TAG2 7000 /* for parameters between 0 and 1000   */
#define EC_TAG2(I,p) {ec_send_code((I)); ec_send_code(TAG2+(p)); }

#endif
