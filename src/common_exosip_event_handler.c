/*
  The UCT IMS Client
  Copyright (C) 2006 - University of Cape Town
  David Waiting <david@crg.ee.uct.ac.za>
  Richard Good <rgood@crg.ee.uct.ac.za>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


#include "includes.h"
#include "common_exosip_event_handler.h"

#include <gst/controller/gstcontroller.h>


GList *call_list = NULL;


int find_call(int did, Call **ca)
{
	GList *li;
	Call *current;
	
	for(li = call_list; li != NULL; li = li->next)
	{
		current = (Call *)li->data;

		if (current->did == did)
		{
			*ca = li->data;
			return 0;
		}
	}

	return -1;
}


void add_call(Call **ca)
{

	*ca = (Call *)malloc(sizeof(Call));

	if (*ca == NULL)
	{
		printf("Could not allocate memory\n");
    		return ;
	}

	memset (*ca, 0, sizeof (Call));

	// printf("Creating new pipelines\n");
	(*ca)->ringingPipeline = NULL;
	(*ca)->videoTxPipeline = NULL;
	(*ca)->videoRxPipeline = NULL;
	(*ca)->audioTxPipeline= NULL;
	(*ca)->audioRxPipeline= NULL;
	(*ca)->iptvVideoPipeline= NULL;

	(*ca)->video_supported = 0;
	(*ca)->audio_codec = 0;
	(*ca)->qos_confirm = 0;
	(*ca)->media_negotiated = 0;
	(*ca)->content_indirected = 0;
	strcpy((*ca)->local_ip, pref->local_audio_ip);
	(*ca)->local_audio_port = pref->local_audio_port;
	(*ca)->local_video_port = pref->local_video_port;

	call_list = g_list_append(call_list, (gpointer)*ca);
}


void delete_call(int did)
{
	GList *li; 	/* an iterator */
	Call *current;
	
	for(li = call_list; li != NULL; li = li->next)
	{
		current = (Call *)li->data;

		if (current->did == did)
		{
			call_list = g_list_remove(call_list, li->data);
			break;
		}
	}
}


void print_calls()
{
	GList *li; 	/* an iterator */
	Call *current;
	
	for(li = call_list; li != NULL; li = li->next)
	{
		current = (Call *)li->data;

		printf("Call %d  To: %s  From: %s \n", current->did, current->to_uri, current->from_uri);
	}
}


void call_released(eXosip_event_t *je)
{

	Call *ca;


	if (find_call(current_dialog_id, &ca) < 0)
	{
		g_warning("no current dialogs");
		return ;
	}


	set_display("Call released");

	// printf("Call ID:  %d   Event ID:  %d   Current Dialog ID: %d  Event Dialog ID: %d\n", ca->cid, je->cid, current_dialog_id, je->did);

	/* check if this is the current call and end it */
	if ((ca->cid == je->cid) || (current_dialog_id == je->did))
	{
		/* Destroy all media pipelines */
		destroyAudioTxPipeline(ca);
		destroyAudioRxPipeline(ca);

		destroyRingingPipeline(ca);
		destroyVideoTxPipeline(ca);
		destroyVideoRxPipeline(ca);

		destroyBackgroundVideoPipeline();

		destroyIptvVideoPipeline();

		/*destroy rtsp session if exists*/
		if(vod_window_open == 1)
		{
			rtsp_end_session();
		}
		/*destroy msrp session if exists*/
		if(ca->remote_msrp_port && pref->session_im_enabled)
		{
			endpointmsrp_end_session(0);
		}
// 		if(im_window_open == 1)
// 		{
// 			gtk_widget_destroy(GTK_WIDGET(im_window));
// 			im_window_open = 0;
// 			num_im_tabs=0;
// 		}

		ca->call_is_active = 0;

		delete_call(je->did);
		
		state = IDLE;

	}


 }


void start_rtp_session(eXosip_event_t *je)
{

	GstState state;

	Call *ca;

	if (find_call(je->did, &ca) < 0)
		return ;

	ca->call_is_active = 1;

	/* Destroy any existing pipeline */
	if (GST_IS_ELEMENT(ca->ringingPipeline))
	{
		destroyRingingPipeline(ca);
	}

	if (GST_IS_ELEMENT(backgroundVideoPipeline))
	{
		destroyBackgroundVideoPipeline();
	}
	
	/* Initialise the media pipelines if they are not already playing */
	/* FUTURE WORK FOR RICHARD: otherwise simply change properties */

	if (GST_IS_ELEMENT(ca->audioRxPipeline))
	{
		printf("Destroying existing audio Rx pipeline\n");
		destroyAudioRxPipeline(ca);
	}

	if (GST_IS_ELEMENT(ca->audioTxPipeline))
	{
		printf("Destroying existing audio Tx pipeline\n");
		destroyAudioTxPipeline(ca);
	}

	if (GST_IS_ELEMENT(ca->videoRxPipeline))
	{
		printf("Destroying existing video Rx pipeline\n");
		destroyVideoRxPipeline(ca);
	}

	if (GST_IS_ELEMENT(ca->videoTxPipeline))
	{
		printf("Destroying existing video Tx pipeline\n");
		destroyVideoTxPipeline(ca);
	}


	/* Must send a small packet to the NAT to open it up from the inside */
	// media_open_nat(ca->remote_audio_ip, ca->remote_audio_port, ca->local_audio_port);
	// media_open_nat(ca->remote_video_ip, ca->remote_video_port, ca->local_video_port);

	// fprintf(stderr, "Starting media session on the following ports:\n  video Tx: %d video Rx: %d\n  Audio Tx: %d Audio Rx: %d\n", ca->remote_video_port, ca->local_video_port, ca->remote_audio_port, ca->local_audio_port);

	/* Check whether we have an IPTV session or a normal call */
	if(imsua_regex_match(ca->to_uri, pref->iptv_server))
	{
		set_display("Starting IPtv session.\n\nPress Alt+Enter for fullscreen\n");
		initialiseAudioRxPipeline(ca);
		initialiseIptvVideoPipeline(ca);
	}
	else
	{

		initialiseAudioRxPipeline(ca);
		initialiseAudioTxPipeline(ca);

		// printf("Video enabled %d  Video supported %d \n", pref->video_enabled, ca->video_supported);

		if (pref->video_enabled && ca->video_supported)
		{
			initialiseVideoTxPipeline(ca);
			initialiseVideoRxPipeline(ca);		
		}
	}
}

void common_process_info(eXosip_event_t *je)
{

	osip_body_t *info_body;
	char display[500];
	osip_body_init(&info_body);
	osip_message_get_body(je->request, 0, &info_body);

	/* send 200ok */
	osip_message_t *answer;
	eXosip_call_build_answer(je->tid, 200, &answer);
	eXosip_call_send_answer(je->tid, 200, answer);
	

	osip_content_type_t *content_type;
	content_type = osip_message_get_content_type(je->request);

	if(strcmp(content_type->type, "application/dtmf-relay"))
	{
		char *c;
		c = strstr(info_body->body, "=") + 1;

		sprintf(display, "Received DMTF tone: %s", c);
		set_display(display);
	}

}

