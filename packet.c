#include "config.h"

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_PCAP_H
#include <pcap.h>
#endif

pcap_t *pcap_handle;
char   errbuf[PCAP_ERRBUF_SIZE];

/* 
   function passed to pcap_loop to read processed packets
   that match the filter.
   sends matched packets to database for later interactive
   processing.
 */
void read_packet(u_char *args, const struct pcap_pkthdr *hdr,
		 const u_char *packet)
{
  return;
}

/* 
   function to read packets if we were given a device
   to read from.  Also the default if no pcap file
   was given
*/

int init_packet_capture_live(char *dev) 
{  
  if (dev) {
    pcap_handle = pcap_open_live(dev, 1500, 1, 1000, errbuf);
  }

  if (!pcap_handle) {
    syslog(LOG_ALERT, "Unable to open device %s: %s", dev, errbuf);
    return(-1);
  }

  return(0);
}

/* 
   function called to read packets if we were given a previous pcap file 
   to read from
*/
int init_packet_capture_from_file(char *file)
{
  if (file) {
    pcap_handle = pcap_open_offline(file, errbuf);
  }

  if (!pcap_handle) {
    syslog(LOG_ALERT, "Unable to open device %s: %s", file, errbuf);
    return(-1);
  }

  return(0);
}

/* 
   set up filter and start capture
   if reading from device,
   function will not return
*/
void run_packet_capture() 
{
  struct bpf_program filter;
  char               filter_ex[] = "tcp[13] = 18 or udp";

  if (pcap_handle) {
    /* compile filter */
    if (pcap_compile(pcap_handle, &filter, filter_ex, 0, 0) < 0) {
      syslog(LOG_ALERT, "Couldn't parse filter %s: %s\n", filter_ex,
	     pcap_geterr(pcap_handle));
    } else {
      if (pcap_setfilter(pcap_handle, &filter) < 0) {
	syslog(LOG_ALERT, "Couldn't parse filter %s: %s\n", filter_ex,
	       pcap_geterr(pcap_handle));
      }
    }
  }

  /* device ready, filter compiled.  Here we go... */
  if (pcap_handle) {
    pcap_loop(pcap_handle, -1, read_packet, NULL);
  }
}
