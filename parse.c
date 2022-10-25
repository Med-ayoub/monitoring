#include <stdio.h> 
#include "parse.h"

int get_access_points(const char *path, char * buffer) 
{	
	FILE * fp;
    //open file
	fp=fopen(path,"r");
    if(!fp) {
        perror("open file failed");
        return EXIT_FAILURE;
    }

    //dump file to buffer 
	fread(buffer,1024,1,fp);
	fclose(fp);

	return EXIT_SUCCESS;
}

int get_ssid(json_object* object_ap, char * ssid) 
{
    struct json_object *ssid_obj; 
	if(!json_object_object_get_ex(object_ap,"ssid",&ssid_obj)){
        return EXIT_FAILURE;
    }
	sprintf(ssid,"%s",json_object_get_string(ssid_obj));
    return EXIT_SUCCESS;
}

int get_channel(json_object* object_ap, int * channel) 
{
    struct json_object *channel_obj; 
	if(!json_object_object_get_ex(object_ap,"channel",&channel_obj)){
        return EXIT_FAILURE;
    }
	*channel=(int)json_object_get_int(channel_obj);
    return EXIT_SUCCESS;
}

int get_snr(json_object * object_ap, int *snr)
{
    struct json_object *snr_obj; 
    if(!json_object_object_get_ex(object_ap,"snr",&snr_obj)){
        return EXIT_FAILURE;
    }
	*snr=(int)json_object_get_int(snr_obj);
    return EXIT_SUCCESS;
}

int format_ap_list(const char * path, ap_info_list_t * ap_list)
{
    int ret = EXIT_SUCCESS;
    struct json_object *parsed_json; 
    struct json_object * access_points;
    struct json_object *access_point_index;
	unsigned int i,length;
    char buffer[1024];


    if((ret=get_access_points(path,buffer))!=0) {
        return ret;
    }
    //get acces point buffer
	parsed_json = json_tokener_parse(buffer);
	if(!json_object_object_get_ex(parsed_json,"access_points",&access_points)) {
        printf("could not get access points");
    }
	length =(unsigned int)json_object_array_length(access_points);
    ap_list->length = length>MAX_APS? MAX_APS:length;

	for(i=0; i<ap_list->length; i++) {
		access_point_index = json_object_array_get_idx(access_points,i);
        ret &= get_ssid(access_point_index,ap_list->ap[i].ssid);
        ret &= get_snr(access_point_index,&ap_list->ap[i].snr);
        ret &= get_channel(access_point_index,&ap_list->ap[i].channel);
	}
    return ret;
}

int format_response(const char * path, ap_info_list_t *ap_list,char * response)
{
    unsigned int i;
    ap_info_list_t tmp_ap_list;
    if(format_ap_list(path,&tmp_ap_list)!=0) {
        printf("failed to fill access point list");
        return EXIT_FAILURE;
    }

	for(i=0; i< tmp_ap_list.length;i++) {
		if(strcmp(ap_list->ap[i].ssid,tmp_ap_list.ap[i].ssid)==0)
		{
			if(ap_list->ap[i].snr!=tmp_ap_list.ap[i].snr)
			{
				sprintf(response+strlen(response),"%s's SNR has changed from %d to %d\n",
                            ap_list->ap[i].ssid,ap_list->ap[i].snr,tmp_ap_list.ap[i].snr);
				ap_list->ap[i].snr=tmp_ap_list.ap[i].snr;
			}
			if(ap_list->ap[i].channel!=tmp_ap_list.ap[i].channel)
			{
				sprintf(response+strlen(response),"%s's channel has changed from %d to %d\n"
				        ,ap_list->ap[i].ssid,ap_list->ap[i].channel,tmp_ap_list.ap[i].channel);
				ap_list->ap[i].channel=tmp_ap_list.ap[i].channel;
			}
            fprintf(stderr,"the same ssid\n");
		}
		else 
		{
            fprintf(stderr,"not the same\n");
			sprintf(response+strlen(response),"%s's is removed from the list\n",ap_list->ap[i].ssid);
            sprintf(response+strlen(response),"%s is added to the list with SNR %d and channel %d\n",
                    tmp_ap_list.ap[i].ssid,tmp_ap_list.ap[i].snr,tmp_ap_list.ap[i].channel);

			sprintf(ap_list->ap[i].ssid,"%s",tmp_ap_list.ap[i].ssid);
			ap_list->ap[i].snr=tmp_ap_list.ap[i].snr;
			ap_list->ap[i].channel=tmp_ap_list.ap[i].channel;
		}
	}
	return EXIT_SUCCESS;
}
