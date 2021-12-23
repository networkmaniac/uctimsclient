#ifndef EXTERN_VARS_H
#define EXTERN_VARS_H

extern char *filepath;

extern int mode;
extern int state;
extern int current_dialog_id;

/*
extern int ims_qos;
extern int ims_qos_type;
extern int ims_access_network;


extern int media_1st_codec;
extern int media_2nd_codec;
extern int media_dtmf_events;
extern int media_sound_card;

extern char local_ip[128];
extern int local_audio_port;
extern int local_video_port;

extern char profile_name[50];
extern char profile_email_address[50];
extern char profile_organisation[50];

int presence_enabled;

extern char media_interface [50];
extern int media_interface_id;


*/

char presentity[100];

/*
extern char sip_uri[50];
extern char sip_proxy[50];
extern char sip_password[50];

extern char ims_public_ui[50];
extern char ims_private_ui[50];
extern char ims_pcscf[50];
extern char ims_realm[50];
extern char ims_scscf[50];
extern char ims_password[50];

extern char xcap_server_url[100];
extern char xcap_username[50];
extern char xcap_password[50];
*/

extern char *ims_service_route[50];
extern int num_service_routes;

extern char *ims_associated_uris[20];
extern int num_associated_uris;

extern char input[10];
extern int is_message_deregister;
extern int im_window_open;
extern int preferences_open;
extern int preferences_changed;

extern int xdms_download_open;
extern int xdms_upload_open;
extern int xml_file_dialog_open;

extern char *sip_str;
extern char *sip_strs;

extern struct timeval start_time;
extern struct timeval end_time;

extern char display[500];

GList *buddy_list;
GList *watcher_list;

int winfo_subscription_did;

GstElement *backgroundVideoPipeline;

char *access_networks[6];

extern int num_im_tabs;

extern GtkWidget *imsUA;
extern GtkWidget *videoWin; 

extern GtkWindow *im_window;
extern GtkWidget *preferences;

extern GtkWindow *xdms_download_window;
extern GtkWindow *xdms_upload_window;

extern GtkWidget *xml_file_dialog;

extern int time_stamps;
extern int reg_id;
extern int registered;
extern int dtmf_tab[16];

extern int full_screen;

//vod variables
extern GtkWindow *vod_window;
extern libvlc_media_player_t *media_player;
extern libvlc_media_t *media_mrl;
extern int vod_window_open;
extern int vod_state;
extern int vod_full_screen;

//msrp var
extern MsrpEndpoint *local_msrp_endpoint;
int msrp_destroyed;

Preferences *pref;

//IPTV external variables - Robert Marston July 2008
extern GtkWidget *iptv_epg_window;
extern GtkWidget *iptv_vod_epg_window;
extern int iptv_epg_window_open;
extern int iptv_vod_epg_window_open;
extern GtkTreeStore *iptv_vod_treestore;

#endif
