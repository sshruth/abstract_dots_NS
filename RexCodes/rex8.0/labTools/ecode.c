/* ECODE.C
**
**
** a collection of utilities for storing ecodes -- either
** dropping them into a REX ecode file or sending them to
** Harvey's magic box
**
** PREFIX: ec
**
**	15-bit messages (plus a strobe bit) are sent to plexon
** via the dio board... in the shadlen lab rig 4, we have
** this configured to use ports 16 & 17 (low-order bytes on
**	module 4, the last module). Port 16 has the lsb. The msb
** on port 17 is the strobe bit.
**
** The ms 2 bits of the message indicate what kind of
**	information is being sent. The rest of the message
** is then parsed accordingly
**
**	 MSB														LSB
**
**	 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
**	 -----------------------------------------------
**	|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
**	 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
**
** Standard code (SC):
**
**	 S  0  0 |<---------------Message-------------->|
**                           13 bits
**
**
**	DIO command (DC):
**
**	 S  0  1 |<----Port---->|<--------Data--------->|
**                5 bits            8 bits
**
**
**	Matlab command (MC):
**
**	 S  1  0 |<-----------Command--------->|<# args>|
**                       10 bits            3 bits
**                       
**
**	Matlab argument (MA):
**
**	 S  1  1 |<arg #>-|<-----------Value----------->|
**            3 bits   10 bits (two's compliment)
**
**
**	Note that "hi" and "lo" refer to the priority -- "hi"
** priority codes are always dealt with before "lo" prioity
** codes
**	
** Revision history:
**	5/31/01 created by jig
*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>	/*needed for ../hdr/sys.h, which is included in rexhdrs*/
#include "rexhdrs.h"
#include "toys.h"
#include "ecode.h"

/* PRIVATE CONSTANTS */
#define EC_CMD_CODE   (0x0000)
#define EC_CMD_DIO  	 (0x2000)
#define EC_CMD_MATCMD (0x4000)
#define EC_CMD_MATARG (0x6000)

#define EC_SC_CMD		(0x0000)
#define EC_SC_FLD1	13			/* number of bits for "Message"	*/
#define EC_DC_CMD		(0x2000)
#define EC_DC_FLD1	5			/* number of bits for "Port"		*/
#define EC_DC_FLD2	8			/* number of bits for "Data"		*/
#define EC_MC_CMD		(0x4000)
#define EC_MC_FLD1	10			/* number of bits for "Command"	*/
#define EC_MC_FLD2	3			/* number of bits for "# args"	*/
#define EC_MA_CMD		(0x6000)
#define EC_MA_FLD1	3			/* number of bits for "arg #"		*/
#define EC_MA_FLD2	10			/* number of bits for "Value"		*/

#define EC_FLD(v,b,s) (((unsigned short)( (1<<(b))-1) & (v) ) << (s))

/* PUBLIC ROUTINES */

/* PUBLIC ROUTINES: ec_send_code*
**
**	message that is a "standard" REX code
** (i.e., value that we normally drop in the ecode file)
**
**	We have 13 bits -- 8192 codes -- to work with. 
**	So we could think about sending REX codes defined
**	in lcode.h as is....
**
** default priority: high
**
**	NOTE: In Plexon, these will appear as codes with prefix
**		SE00 or SE01
*/
#define EC_MAKE_SC(m) (EC_SC_CMD | EC_FLD((m),EC_SC_FLD1,0))

int ec_send_code(long code)
 {
   	
//   	printf("sending code %ld\n", (long) code);
   	
   	
   	ec_send_hi(EC_MAKE_SC(code));
   	return(0);
 }
 
int ec_send_code_hi(long code)
 {
   ec_send_hi(EC_MAKE_SC(code));
   return(0);
 }
 
int ec_send_code_lo(long code)
 {
   ec_send_lo(EC_MAKE_SC(code));
   return(0);
 }

/* PUBLIC ROUTINE: ec_send_dio*
**
**	message that is a dio command
**	(i.e., something created by "Dio_id(PCDIO_DIO, <port>, <data>)")
**	-- according to REX, port can be 8 bits -- however, since
**			we only use up to port #19, we can get away with taking
**			only the ls 5 bits
**	-- according to REX, data can be 16 bits -- which frankly, I don't
**			understand because each port is only 8 bits... so here
**			we only use the ls 8 bits
**
**	argument:
**		dio_id_pattern	... the DIO_ID created by Dio_id
**
**	returns:
**		nada
**
** default priority: high
**
**	NOTE: In Plexon, these will appear as codes with prefix
**		SE02 or SE03
*/
#define EC_MAKE_DC(p,d) (EC_DC_CMD | 									\
								 EC_FLD((p),EC_DC_FLD1,EC_DC_FLD2) |  	\
			 					 EC_FLD((d),EC_DC_FLD2,0))
/* see hdr/device.h for details of the DIO_ID */
#define EC_DIO_PORT_MASK	(0x1f0000)	/* only taking ms 5 bits */
#define EC_DIO_PORT_SHIFT	16				/* shift to high byte	 */
#define EC_DIO_DATA_MASK	(0xff)		/* only taking ls 8 bits */
#define EC_DIO_DATA_SHIFT	0
#define EC_GET_DIO_PORT(id) ((unsigned short)(((id) & EC_DIO_PORT_MASK) \
											>> EC_DIO_PORT_SHIFT))
#define EC_GET_DIO_DATA(id) ((unsigned short)(((id) & EC_DIO_DATA_MASK) \
											>> EC_DIO_DATA_SHIFT))

void ec_send_dio(DIO_ID dio_id_pattern)
 {
   /*
   **	printf("sending port %ld, data %ld\n",
   **			(long) EC_GET_DIO_PORT(dio_id_pattern), 
   **			(long) EC_GET_DIO_DATA(dio_id_pattern));
   */
   
   ec_send_hi(EC_MAKE_DC(EC_GET_DIO_PORT(dio_id_pattern),
                                           EC_GET_DIO_DATA(dio_id_pattern)));
 }
 
void ec_send_dio_hi(DIO_ID dio_id_pattern)
 {
   ec_send_hi(EC_MAKE_DC(EC_GET_DIO_PORT(dio_id_pattern),
                                           EC_GET_DIO_DATA(dio_id_pattern)));
 }
 
void ec_send_dio_lo(DIO_ID dio_id_pattern)
 {
   ec_send_lo(EC_MAKE_DC(EC_GET_DIO_PORT(dio_id_pattern),
                                           EC_GET_DIO_DATA(dio_id_pattern)));
 }

/* PUBLIC ROUTINE: ec_send_matlab_command*
**
**	message that is a matlab command
**
**	Arguments:
** 	command  ... code of the command (defined in **)
**		num_args ... number of arguments
**
**	Returns:
**		none
**
**	Keep in mind that Matlab commands can take arrays/matrices
**	as arguments. Num_args is simply the number of separate
**	variables listed in the function declaration. e.g.,
**	showTarget([0 1], 0, [], [-80 80]) has 4 arguments.
**
** default priority: high
**
**	NOTE: In Plexon, these will appear as codes with prefix
**		SE04 or SE05
*/
#define EC_MAKE_MC(c,n) (EC_MC_CMD | 									\
								 EC_FLD((c),EC_MC_FLD1,EC_MC_FLD2) |  	\
			 					 EC_FLD((n),EC_MC_FLD2,0))

void ec_send_matlab_command(int command, int num_args)
 {
   /*
   **	printf("sending command %ld, num args %ld\n", command, num_args);
   */
   
/*   ec_send_hi(EC_MAKE_MC(command, num_args)); */
 }
 
void ec_send_matlab_command_hi(int command, int num_args)
 {
   ec_send_hi(EC_MAKE_MC(command, num_args));
 }
 
void ec_send_matlab_command_lo(int command, int num_args)
 {
   ec_send_lo(EC_MAKE_MC(command, num_args));
 }

/* PUBLIC ROUTINE: ec_send_matlab_arg*
**
**	message that is an argument to a matlab command. By convention,
**	we'll send Arg #0 as the last argument, marking the end of
**	a given cmd call
**
**	Arguments:
** 	arg #  ... which argument
**		value  ... duuh
**
**	Returns:
**		none
**
** default priority: low
**
**	NOTE: In Plexon, these will appear as codes with prefix
**		SE06 or SE07
**
*/

#define EC_MAKE_MA(n,v) (EC_MA_CMD | 									\
								 EC_FLD((n),EC_MA_FLD1,EC_MA_FLD2) |  	\
			 					 EC_FLD((v),EC_MA_FLD2,0))

void ec_send_matlab_arg(int arg_num, int value)
 {
   /*
   **	printf("sending arg num %ld, value %ld\n", arg_num, value);
   */
   
/*   ec_send_lo(EC_MAKE_MA(arg_num, value)); */
 }
 
void ec_send_matlab_arg_hi(int arg_num, int value)
 {
   ec_send_hi(EC_MAKE_MA(arg_num, value));
 }
 
void ec_send_matlab_arg_lo(int arg_num, int value)
 {
   ec_send_lo(EC_MAKE_MA(arg_num, value));
 }

/* PUBLIC ROUTINE: ec_send*
**
** Calls ldevent or ldevent_plexon_low based on the requested priority 
*/
void ec_send_hi(unsigned short msg)
 {
   EVENT event;
   /*
   ** printf("HIGH PRIORITY MESSAGE: %d\n", (int) msg);
   */
   
   event.e_key = i_b->i_time;
   event.e_code = msg;
   ldevent(&event);
 }

void ec_send_lo(unsigned short msg)
 {
   EVENT event;
   /*
   ** printf("LOW PRIORITY MESSAGE:  %d\n", (int) msg);
   */
   
   event.e_key = i_b->i_time;
   event.e_code = msg;
   
   #ifdef PLEX_ECODES
   ldevent_plexon_low(&event,0);
   #else
   ldevent(&event);
   #endif
 }





