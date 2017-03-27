//-------------------------- GLOBAL PARAMETERS -----------------//
const char *strNVHost = "http://192.168.85.140:8182";

// To generate the Base64 encryption, go to: https://www.base64encode.org/
// type the NV credentials in the following format: 
// USER_NAME:PASSWORD
// and replace the encoded string below.
const char *strAuth = "Basic U2hhaGFyOkRhZ25haGFzaDEyMw==";
char strURI[160];
char retBuffer[5000];
//--------------------------------------------------------------//

// Concat null terminated strings
char *xstrcat( char *dest, char *src )
{ 
	while (*dest) dest++; 
	while (*dest++ = *src++); 
	return --dest; 
}


/***************************************************** 
StartNVCustom() will initiate NV test with the following parameters:

The 2 source IP addresses represent a range of IP addresses (From-To)
	srcIPFrom (string) - source IP address from
	srcIPTo (string) - source IP address to

The 2 destination IP addresses represent a range of IP addresses (From-To)
	destIPFrom (string) - destination IP address from
	destIPTo (string) - destination IP address to
	
 * Sending an empty string in the "to" parameters (srcIPTo and destIPTo) - will generate a single IP address (the 'From' IP) instead of a range
 		Sample: StartNVCustom("192.168.1.100", "", "192.168.1.200", "", 123, 0.5, 4015, 6145);
 		
 * Sending Empty strings in both destination IP addresses - will include in the destination server ALL IP addresses with the exclusion of the source IP address/range
 		Sample 1: StartNVCustom("192.168.1.100", "192.168.1.200", "", "", 123, 0.5, 4015, 6145);
 		Sample 2: StartNVCustom("192.168.1.6", "", "", "", 500, 1, 4015, 6145);

These are the network virtualization parameters:
	latency (int) - latency in milliseconds
	packetLoss (float) - packet loss in %
	bandwidthIn (float) - Inbound bandwidth in Kbps
	bandwidthOut (float) - Outbound bandwidth in Kbps
				
*****************************************************/
StartNVCustom(char *srcIPFrom, char *srcIPTo, char *destIPFrom, char *destIPTo, int latency, float packetLoss, float bandwidthIn, float bandwidthOut)
{
	char paramsBuffer[2500];
	int ret = 0;

	if (isNVRunning() > 0) 
		return 0; //exit if alraedy running

	strcpy(strURI, strNVHost);
	xstrcat(strURI, "/shunra/api/emulation/custom");
	
	if (srcIPTo == "") srcIPTo = srcIPFrom;

	if ((destIPFrom == "") && (destIPTo == ""))
	{
		ret = sprintf(paramsBuffer, "{\"flows\": [{"
				"\"flowId\": \"Flow_1\","
				"\"srcIpRange\": {"
					"\"include\": [{" 
						"\"from\": \"%s\"," 
						"\"to\": \"%s\"" 
					"}]" 
				"}," 
				"\"latency\": %d," 
				"\"packetloss\": %f," 
				"\"bandwidthIn\": %f," 
				"\"bandwidthOut\": %f," 
				"\"isCaptureClientPL\": \"false\"" 
				"}],"
				"\"testMetadata\": {" 
				"\"testName\":\"Prudential-User Defined Test_%d\"," 
				"\"description\":\"User defined NV test for testing backend servers\"," 
				"\"networkScenario\":\"User defined networks\"}}", srcIPFrom, srcIPTo, latency, packetLoss, bandwidthIn, bandwidthOut, time());
	}
	else
	{
		if (destIPTo == "") destIPTo = destIPFrom;
	
		ret = sprintf(paramsBuffer, "{\"flows\": [{"
				"\"flowId\": \"Flow_1\","
				"\"srcIpRange\": {"
					"\"include\": [{" 
						"\"from\": \"%s\"," 
						"\"to\": \"%s\"" 
					"}]" 
				"}," 
				"\"destIpRange\": {"
					"\"include\": [{" 
						"\"from\": \"%s\"," 
						"\"to\": \"%s\"" 
					"}]" 
				"}," 
				"\"latency\": %d," 
				"\"packetloss\": %f," 
				"\"bandwidthIn\": %f," 
				"\"bandwidthOut\": %f," 
				"\"isCaptureClientPL\": \"false\"" 
				"}],"
				"\"testMetadata\": {" 
				"\"testName\":\"Prudential-User Defined Test_%d\"," 
				"\"description\":\"User defined NV test for testing backend servers\"," 
				"\"networkScenario\":\"User defined networks\"}}", srcIPFrom, srcIPTo, destIPFrom, destIPTo, latency, packetLoss, bandwidthIn, bandwidthOut, time());
	}
	
	lr_output_message(">>%d<<", ret);
	SendRequest("StartFromProfile", "POST", strURI, paramsBuffer);

	// Debug:
	lr_output_message("---%s---\n---%d---", retBuffer, strlen(retBuffer));
	if (retBuffer) 
		return strlen(retBuffer);
	else
		return 0;
}


// Stop all NV tests
void StopNV()
{
	strcpy(strURI, strNVHost);
	xstrcat(strURI, "/shunra/api/emulation");
	SendRequest("StopNV", "DELETE", strURI, "");
	
	// Debug:
	lr_output_message("--- %s ---", retBuffer);
	
}

int isNVRunning()
{
	strcpy(strURI, strNVHost);
	xstrcat(strURI, "/shunra/api/emulation/tokens?all=false");
	SendRequest("TestTokens", "GET", strURI, "");

	// Debug:
	lr_output_message("---%s---\n---%d---", retBuffer, strlen(retBuffer));
	if (retBuffer) 
		return strlen(retBuffer);
	else
		return 0;
}

/*********************************
Build and sent REST request 
**********************************/
int SendRequest(char *requestName, char *requestType, char *uri, char *request_json_base)
{
	int returnFromRequest = 0;
	
	lr_save_string(uri, "REQ_URL");
	lr_save_string(request_json_base, "REQ_JSON_PARAM");
	lr_save_string(requestType,"REQ_METHOD");
	lr_save_string(requestName,"REQ_NAME");

	//Debug:
	lr_output_message("\n*************\nRequest Name: %s\nRequest Type: %s\nRequest URL: %s\nRequest Body: %s\n*************", 
	                  requestName,
	                  requestType,
	                  uri,
	                  request_json_base);
	
	web_reg_save_param("Response", "LB={", "Notfound=warning", "Search=Body", LAST);
		
	web_add_header("Authorization", strAuth);
	returnFromRequest = web_custom_request("{REQ_NAME}",
	         "URL={REQ_URL}",
	         "Method={REQ_METHOD}",
	         "Resource=1",
	         "RecContentType=application/json",
	         "EncType=application/json",
	         "Body={REQ_JSON_PARAM}",
	         LAST);
	
	if (returnFromRequest == 0)
		strcpy(retBuffer, lr_eval_string("{Response}"));
	
	return returnFromRequest;
	
}