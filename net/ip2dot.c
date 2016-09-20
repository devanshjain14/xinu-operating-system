/* ip2dot.c */

#include <xinu.h>

/*------------------------------------------------------------------------
 * ip2dot
 *------------------------------------------------------------------------
 */
uint32	ip2dot (
 
  char	 *dotted, /* Location to which IP address */
                 /*     string will be stored	*/
  uint32 ipaddress
	)
{

	sprintf(dotted, "%d.%d.%d.%d",
          (ipaddress>>24)&0xff, (ipaddress>>16)&0xff,
          (ipaddress>>8)&0xff,   ipaddress & 0xff);

	return OK;
}
