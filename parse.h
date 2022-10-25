#ifndef _PARSE_H_
#define _PARSE_H_


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <json-c/json.h>

#define MAX_APS 64

typedef struct {
	char ssid[16];
	int snr;
	int channel;

} ap_info_t;

typedef struct {
	unsigned int length;
	ap_info_t ap[MAX_APS];
} ap_info_list_t;

/**
 *@brief get access point object from json file
 *@param  [IN]  path : file path
 *@param  [OUT] access_points : access point object
 *@return On success, return 0 , on error return 1
 **/
int get_access_points(const char *path, char * buffer);

/**
 *@brief get ssid accesspoint object 
 *@param  [IN]  object_ap : access point json object
 *@param  [OUT] ssid : ssid output
 *@return On success, return 0 , on error return 1
 **/
int get_ssid(json_object* object_ap, char * ssid);

/**
 *@brief get channel accesspoint object 
 *@param  [IN]  object_ap : access point json object
 *@param  [OUT] channel : channel output
 *@return On success, return 0 , on error return 1
 **/
int get_channel(json_object* object_ap, int *channel);

/**
 *@brief get snr accesspoint object 
 *@param  [IN]  object_ap : access point json object
 *@param  [OUT] snr : snr output
 *@return On success, return 0 , on error return 1
 **/
int get_snr(json_object * object_ap, int *snr);


/**
 *@brief int the access point list entry
 *@param  [IN]  path : file path
 *@param  [OUT] ap_list : access point list
 *@return On success, return 0 , on error return 1
 **/
int format_ap_list(const char * path, ap_info_list_t* ap_list);

/**
 *@brief int the access point list entry
 *@param  [IN]  path : file path
 *@param  [OUT] ap_list : access point list
 *@param  [OUT] response : formatted response
 *@return On success, return 0 , on error return 1
 **/
int format_response(const char * path, ap_info_list_t *ap_list, char * response);

#endif