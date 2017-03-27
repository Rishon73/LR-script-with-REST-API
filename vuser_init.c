vuser_init()
{
	/*
	StartNVCustom() parameters are defined in NVAPIWrapper.h file.
	--------------------------------------------------------------
	
	 * The function will accept an empty string in the 2nd and 4th parameters (the range’s ‘From’):
		e.g.: StartNVCustom("192.168.1.6", "", "10.10.10.43", "", 500, 1, 0, 0); 
		This will create a single source IP (192.168.1.6) and single destination IP (10.10.10.43) addresses – only packets going from the source IP to destination IP, and back, will experience the NV emulation
	
	* The function will accept an empty strings in both destination IP addresses:
		e.g.: StartNVCustom("192.168.1.6", "", "", "", 500, 1, 0, 0); 
		Or
		e.g.: StartNVCustom("192.168.1.6", "192.168.1.60", "", "", 500, 1, 0, 0); 
		Both will create single or a range for the source IP address/es and will include all the other IP addresses (any IP) with the exclusion of the source IP address/es.
		It means that any inbound/outbound packet from/to the source IP address/es will experience the NV emulation
	
	* Packet loss parameter is percentage between 0-90,  0, 0.1, 0.05 and 1 are valid values
	
	* The 2 bandwidth parameters (2 last ones) are Kbps and can be 0 (zero) for ‘Unrestricted’ bandwidth
 */
	 StartNVCustom("192.168.1.6", "192.168.1.60", "", "", 500, 1, 0, 0);
	

	return 0;
}
