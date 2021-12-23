#Makefile for UCT IMS Client version 1.0
#
#Compiler: gcc 4.0.3

PREFIX=/usr/local
BINDIR=$(PREFIX)/bin

CC = gcc
LIBS = -leXosip2 -lxml2 -lcurl -lgstinterfaces-0.10 -lvlc -losipparser2 -lgstreamer-0.10
INCLUDES = -I/usr/include/libxml2 -I/usr/lib/vlc/include/
GTK_LIB_FLAGS = `pkg-config --cflags --libs gtk+-2.0`
GTK_INC_FLAGS = `pkg-config --cflags gtk+-2.0 gstreamer-0.10`

ODIR=obj

_OBJS = callbacks.o common_exosip_event_handler.o common_interface_event_handler.o DigestAKAv1MD5.o ims_exosip_event_handler.o \
ims_interface_event_handler.o interface.o imsUA.o media.o preferences.o presence.o \
sdp_methods.o sound_conv.o support.o useful_methods.o watchers.o xcap.o gstreamer.o uuid.o sysdep.o rtsp.o \
msrp.o msrp_callback.o msrp_message.o msrp_network.o msrp_relay.o msrp_session.o msrp_switch.o msrp_utils.o endpointmsrp.o \
iptv.o iptv_epg.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

SRC_DIR=src

SRCS = imsUA.c callbacks.c common_exosip_event_handler.c common_interface_event_handler.c DigestAKAv1MD5.c ims_exosip_event_handler.c \
ims_interface_event_handler.c interface.c media.c preferences.c presence.c \
sdp_methods.c sound_conv.c support.c useful_methods.c watchers.c xcap.c gstreamer.c uuid.c sysdep.c rtsp.c \
msrp.o msrp_callback.c msrp.message.c msrp_network.c msrp_relay.c msrp_session.c msrp_switch.c msrp_utils.c endpointmsrp.c \
iptv.c iptv_epg.c

PROG = uctimsclient

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -g -o $@ $(OBJS) \
	$(GTK_LIB_FLAGS) $(LIBS) 

$(ODIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(INCLUDES) $(GTK_INC_FLAGS) -o $@ -c $<
	
install: $(PROG)
	mkdir -p $(BINDIR)
	install $(PROG) $(BINDIR)/$(PROG)

debian:
	aptitude install libexosip2-dev libcurl4-dev libxml2-dev libvlc-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libgtk2.0-dev

ubuntu:
	apt-get install libexosip2-dev libcurl4-gnutls-dev libxml2-dev libvlc-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libgtk2.0-dev

clean: 
	rm -f $(PROG) $(ODIR)/*.o *~ $(SRC_DIR)/*~
	


# DO NOT DELETE THIS LINE --
