BareSIP
-------

Baresip is a portable and modular SIP User-Agent with audio and video support
Copyright (c) 2010 - 2013 Creytiv.com

Distributed under BSD license


Design goals:

* Minimalistic and modular VoIP client
* SIP, SDP, RTP/RTCP, STUN/TURN/ICE
* IPv4 and IPv6 support
* RFC-compliancy
* Robust, fast, low footprint
* Portable C89 and C99 source code


Modular Plugin Architecture:

alsa          ALSA audio driver
amr           Adaptive Multi-Rate (AMR) audio codec
audiounit     AudioUnit audio driver for MacOSX/iOS
auloop        Audio-loop test module
avcapture     Video source using iOS AVFoundation video capture
avcodec       Video codec using FFmpeg
avformat      Video source using FFmpeg libavformat
bv32          BroadVoice32 audio codec
cairo         Cairo video source
celt          CELT audio codec
cons          UDP console
contact       Contacts module
coreaudio     Apple Coreaudio driver
evdev         Linux input driver
g711          G.711 audio codec
g722          G.722 audio codec
g7221         G.722.1 audio codec
gsm           GSM audio codec
gst           Gstreamer audio source
ice           ICE protocol for NAT Traversal
ilbc          iLBC audio codec
isac          iSAC audio codec
l16           L16 audio codec
mda           Symbian Mediaserver audio driver
menu          Interactive menu
natbd         NAT Behavior Discovery Module
opengl        OpenGL video output
opengles      OpenGLES video output
opensles      OpenSLES audio driver
opus          OPUS Interactive audio codec
oss           Open Sound System (OSS) audio driver
plc           Packet Loss Concealment (PLC) using spandsp
portaudio     Portaudio driver
presence      Presence module
qtcapture     Apple QTCapture video source driver
quicktime     Apple Quicktime video source driver
rst           Radio streamer using mpg123
sdl           Simple DirectMedia Layer (SDL) video output driver
selfview      Video selfview module
silk          SILK audio codec
sndfile       Audio dumper using libsndfile
speex         Speex audio codec
speex_aec     Acoustic Echo Cancellation (AEC) using libspeexdsp
speex_pp      Audio pre-processor using libspeexdsp
speex_resamp  Audio resampler using libspeexdsp
srtp          Secure RTP encryption
stdio         Standard input/output UI driver
stun          Session Traversal Utilities for NAT (STUN) module
syslog        Syslog module
turn          Obtaining Relay Addresses from STUN (TURN) module
uuid          UUID generator and loader
v4l           Video4Linux video source
v4l2          Video4Linux2 video source
vidloop       Video-loop test module
vpx           VP8/VPX video codec
vumeter       Display audio levels in console
winwave       Audio driver for Windows
x11           X11 video output driver
x11grab       X11 grabber video source


IETF RFC/I-Ds:

* RFC 2190  RTP Payload Format for H.263 Video Streams (Historic)
* RFC 2429  RTP Payload Format for 1998 ver of ITU-T Rec. H.263 Video (H.263+)
* RFC 3016  RTP Payload Format for MPEG-4 Audio/Visual Streams
* RFC 3711  The Secure Real-time Transport Protocol (SRTP)
* RFC 3856  A Presence Event Package for SIP
* RFC 3863  Presence Information Data Format (PIDF)
* RFC 3951  Internet Low Bit Rate Codec (iLBC)
* RFC 3952  RTP Payload Format for iLBC Speech
* RFC 3984  RTP Payload Format for H.264 Video
* RFC 4240  Basic Network Media Services with SIP (partly)
* RFC 4298  Broadvoice Speech Codecs
* RFC 4568  SDP Security Descriptions for Media Streams
* RFC 4574  The SDP Label Attribute
* RFC 4585  Extended RTP Profile for RTCP-Based Feedback (RTP/AVPF)
* RFC 4587  RTP Payload Format for H.261 Video Streams
* RFC 4629  RTP Payload Format for ITU-T Rec. H.263 Video
* RFC 4796  The SDP Content Attribute
* RFC 4867  RTP Payload Format for the AMR and AMR-WB Audio Codecs
* RFC 4961  Symmetric RTP / RTP Control Protocol (RTCP)
* RFC 5168  XML Schema for Media Control
* RFC 5574  RTP Payload Format for the Speex Codec
* RFC 5577  RTP Payload Format for ITU-T Recommendation G.722.1
* RFC 5626  Managing Client-Initiated Connections in SIP
* RFC 5761  Multiplexing RTP Data and Control Packets on a Single Port
* RFC 6263  App. Mechanism for Keeping Alive NAT Associated with RTP / RTCP
* RFC 6716  Definition of the Opus Audio Codec

* draft-valin-celt-rtp-profile-02
* draft-westin-payload-vp8-02
* draft-spittka-payload-rtp-opus-00


Architecture:


                   .------.
                   |Video |
                 _ |Stream|\
                 /|'------' \ 1
                /            \
               /             _\|
 .--. N  .----. M  .------. 1  .-------. 1  .-----.
 |UA|--->|Call|--->|Audio |--->|Generic|--->|Media|
 '--'    '----'    |Stream|    |Stream |    | NAT |
            |1     '------'    '-------'    '-----'
            |         C|       1|   |
           \|/      .-----.  .----. |
        .-------.   |Codec|  |Jbuf| |1
        | SIP   |   '-----'  '----' |
        |Session|     1|       /|\  |
        '-------'    .---.      |  \|/
                     |DSP|    .--------.
                     '---'    |RTP/RTCP|
                              '--------'
                              |  SRTP  |
                              '--------'

   A User-Agent (UA) has 0-N SIP Calls
   A SIP Call has 0-M Media Streams


Supported platforms:

* Linux
* FreeBSD
* OpenBSD
* NetBSD
* Symbian OS
* Solaris
* Windows
* Apple Mac OS X and iOS
* Android


Supported compilers:

* gcc (v2.9x to v4.x)
* gcce
* ms vc2003 compiler
* codewarrior


External dependencies:

libre
librem


Feedback:

- Please send feedback to <libre@creytiv.com>
