/* Copyright (C) 2002-2006 Jean-Marc Valin 
   File: speexdec.c

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   
   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
   
   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
   
   - Neither the name of the Xiph.org Foundation nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.
   
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#if !defined WIN32 && !defined _WIN32
#include <unistd.h>
#endif
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#ifndef HAVE_GETOPT_LONG
#include "getopt_win.h"
#endif
#include <stdlib.h>
#include <string.h>

#include <speex/speex.h>
#include <ogg/ogg.h>

#if defined WIN32 || defined _WIN32
#include "wave_out.h"
/* We need the following two to set stdout to binary */
#include <io.h>
#include <fcntl.h>
#endif
#include <math.h>
#include <windows.h>

#ifdef __MINGW32__
#include "wave_out.c"
#endif

#ifdef HAVE_SYS_SOUNDCARD_H
#include <sys/soundcard.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#elif defined HAVE_SYS_AUDIOIO_H
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/audioio.h>
#ifndef AUDIO_ENCODING_SLINEAR
#define AUDIO_ENCODING_SLINEAR AUDIO_ENCODING_LINEAR /* Solaris */
#endif

#endif

#include "wav_io.h"
#include <speex/speex_header.h>
#include <speex/speex_stereo.h>
#include <speex/speex_callbacks.h>
#include "DDlog.h"
#define MAX_FRAME_SIZE 2000

ogg_sync_state oy;
SpeexBits bits;
HWND g_hTeamTalkWnd = 0;
HWND g_hMsgWindow = NULL;
extern CRITICAL_SECTION  cs;

#define readint(buf, base) (((buf[base+3]<<24)&0xff000000)| \
                           ((buf[base+2]<<16)&0xff0000)| \
                           ((buf[base+1]<<8)&0xff00)| \
  	           	    (buf[base]&0xff))

FILE *out_file_open(char *outFile, int rate, int *channels)
{
   FILE *fout=NULL;
   if (strlen(outFile)==0)
   {
        unsigned int speex_channels = *channels;
        if (Set_WIN_Params (INVALID_FILEDESC, rate, SAMPLE_SIZE, speex_channels))
        {
            WriteDebugLog("out_file_open -Can't access-WAVE OUT");
            //fprintf (stderr, "Can't access %s\n", "WAVE OUT");
            return 0;
        }
   } 
   return fout;
}

BOOL sendTeamTalkMessage(const PCOPYDATASTRUCT cpyData)
{
#ifdef _DEBUG
	g_hTeamTalkWnd = FindWindow("TeamTalkMainDialog_debug", 0);
#else
	g_hTeamTalkWnd = FindWindow("TeamTalkMainDialog", 0);
#endif
    
    if (g_hTeamTalkWnd && cpyData)
    {
        SendMessage(g_hTeamTalkWnd,WM_COPYDATA,0,(LPARAM)cpyData);
        return TRUE;
	}
	else
	{
		WriteDebugLog("Can't find the window.");
	}

    return FALSE;
}

static void *process_header(ogg_packet *op, spx_int32_t enh_enabled, spx_int32_t *frame_size, int *granule_frame_size, spx_int32_t *rate, int *nframes, int forceMode, int *channels, SpeexStereoState *stereo, int *extra_headers, int quiet)
{
   void *st;
   const SpeexMode *mode;
   SpeexHeader *header;
   int modeID;
   SpeexCallback callback;
      
   header = speex_packet_to_header((char*)op->packet, op->bytes);
   if (!header)
   {
      WriteDebugLog("Cannot read header");
      //fprintf (stderr, "Cannot read header\n");
      return NULL;
   }
   if (header->mode >= SPEEX_NB_MODES || header->mode<0)
   {
       WriteDebugLog("Mode number %d does not (yet/any longer) exist in this version");
      //fprintf (stderr, "Mode number %d does not (yet/any longer) exist in this version\n", 
      //         header->mode);
      free(header);
      return NULL;
   }
      
   modeID = header->mode;
   if (forceMode!=-1)
      modeID = forceMode;

   mode = speex_lib_get_mode (modeID);
   
   if (header->speex_version_id > 1)
   {
       char Error[256] = {0};
       sprintf(Error,"This file was encoded with Speex bit-stream version %d, which I don't know how to decode\n", header->speex_version_id);
       WriteDebugLog(Error);
      //fprintf (stderr, "This file was encoded with Speex bit-stream version %d, which I don't know how to decode\n", header->speex_version_id);
      free(header);
      return NULL;
   }

   if (mode->bitstream_version < header->mode_bitstream_version)
   {
      WriteDebugLog("The file was encoded with a newer version of Speex. You need to upgrade in order to play it.");
      //fprintf (stderr, "The file was encoded with a newer version of Speex. You need to upgrade in order to play it.\n");
      free(header);
      return NULL;
   }
   if (mode->bitstream_version > header->mode_bitstream_version) 
   {
      WriteDebugLog("The file was encoded with an older version of Speex. You would need to downgrade the version in order to play it.");
      //fprintf (stderr, "The file was encoded with an older version of Speex. You would need to downgrade the version in order to play it.\n");
      free(header);
      return NULL;
   }
   
   st = speex_decoder_init(mode);
   if (!st)
   {
      WriteDebugLog("Decoder initialization failed.");
      //fprintf (stderr, "Decoder initialization failed.\n");
      free(header);
      return NULL;
   }
   speex_decoder_ctl(st, SPEEX_SET_ENH, &enh_enabled);
   speex_decoder_ctl(st, SPEEX_GET_FRAME_SIZE, frame_size);
   *granule_frame_size = *frame_size;

   if (!*rate)
      *rate = header->rate;
   /* Adjust rate if --force-* options are used */
   if (forceMode!=-1)
   {
      if (header->mode < forceMode)
      {
         *rate <<= (forceMode - header->mode);
         *granule_frame_size >>= (forceMode - header->mode);
      }
      if (header->mode > forceMode)
      {
         *rate >>= (header->mode - forceMode);
         *granule_frame_size <<= (header->mode - forceMode);
      }
   }


   speex_decoder_ctl(st, SPEEX_SET_SAMPLING_RATE, rate);

   *nframes = header->frames_per_packet;

   if (*channels==-1)
      *channels = header->nb_channels;

   if (!(*channels==1))
   {
      *channels = 2;
      callback.callback_id = SPEEX_INBAND_STEREO;
      callback.func = speex_std_stereo_request_handler;
      callback.data = stereo;
      speex_decoder_ctl(st, SPEEX_SET_HANDLER, &callback);
   }
   
   if (!quiet)
   {
      fprintf (stderr, "Decoding %d Hz audio using %s mode", 
               *rate, mode->modeName);

      if (*channels==1)
         fprintf (stderr, " (mono");
      else
         fprintf (stderr, " (stereo");
      
      if (header->vbr)
         fprintf (stderr, ", VBR)\n");
      else
         fprintf(stderr, ")\n");
      /*fprintf (stderr, "Decoding %d Hz audio at %d bps using %s mode\n", 
       *rate, mode->bitrate, mode->modeName);*/
   }

   *extra_headers = header->extra_headers;

   free(header);
   return st;
}

DWORD playSpeexAudio(LPVOID pParam)
{
    int option_index = 0;
    char *inFile = "", *outFile = "";
    FILE *fin = 0;
    short out[MAX_FRAME_SIZE];
    short output[MAX_FRAME_SIZE];
    int frame_size=0, granule_frame_size=0;
    void *st=NULL;
    int packet_count=0;
    int stream_init = 0;
    int quiet = 0;
    ogg_int64_t page_granule=0, last_granule=0;
    int skip_samples=0, page_nb_packets;
    ogg_page       og;
    ogg_packet     op;
    ogg_stream_state os;
    int enh_enabled;
    int nframes=2;
    int print_bitrate=0;
    int eos=0;
    int forceMode=-1;
    int audio_size=0;
    float loss_percent=-1;
    SpeexStereoState stereo = SPEEX_STEREO_STATE_INIT;
    int channels=-1;
    int rate=0;
    int extra_headers=0;
    int wav_format=0;
    int lookahead;
    int speex_serialno = -1;
    enh_enabled = 1;

    inFile = (char*)pParam;
    /*Open input file*/
    fin = fopen(inFile, "rb");
    if (!fin)
    {
        WriteDebugLog("Can't open the File.");
        goto end;
    }
        
    fseek(fin,4L,SEEK_SET);//��ǰ�����Ҫ��ָ���ƶ����뿪��ͷ4���ֽڴ�

    /*Main decoding loop*/
    while (1)
    {
        char *data;
        long i, j,nb_read;
        /*Get the ogg buffer for writing*/
        data = ogg_sync_buffer(&oy, 200);
        /*Read bitstream from input file*/
        nb_read = (long)fread(data, sizeof(char), 200, fin);      
        ogg_sync_wrote(&oy, nb_read);

        /*Loop for all complete pages we got (most likely only one)*/
        while (ogg_sync_pageout(&oy, &og)==1)
        {
            int packet_no;
            if (stream_init == 0) 
            {
                ogg_stream_init(&os, ogg_page_serialno(&og));
                stream_init = 1;
            }
            if (ogg_page_serialno(&og) != os.serialno) 
            {
                /* so all streams are read. */
                ogg_stream_reset_serialno(&os, ogg_page_serialno(&og));
            }
            /*Add page to the bitstream*/
            ogg_stream_pagein(&os, &og);
            page_granule = ogg_page_granulepos(&og);
            page_nb_packets = ogg_page_packets(&og);
            if (page_granule>0 && frame_size)
            {
                /* FIXME: shift the granule values if --force-* is specified */
                skip_samples = frame_size*(page_nb_packets*granule_frame_size*nframes - (page_granule-last_granule))/granule_frame_size;
                if (ogg_page_eos(&og))
                skip_samples = -skip_samples;
                /*else if (!ogg_page_bos(&og))
                skip_samples = 0;*/
            } else
            {
                skip_samples = 0;
            }
            /*printf ("page granulepos: %d %d %d\n", skip_samples, page_nb_packets, (int)page_granule);*/
            last_granule = page_granule;
            /*Extract all available packets*/
            packet_no=0;
            while (!eos && ogg_stream_packetout(&os, &op) == 1)
            {
                if (op.bytes>=5 && !memcmp(op.packet, "Speex", 5)) 
                {
                    speex_serialno = os.serialno;
                }
                if (speex_serialno == -1 || os.serialno != speex_serialno)
                    break;
                /*If first packet, process as Speex header*/
                if (packet_count==0)
                {
                     st = process_header(&op, enh_enabled, &frame_size, &granule_frame_size, &rate, &nframes, forceMode, &channels, &stereo, &extra_headers, quiet);
                     if (!st)
                     {
                         WriteDebugLog("process_header error!");
                         goto end;
                     }
                         
                     speex_decoder_ctl(st, SPEEX_GET_LOOKAHEAD, &lookahead);
                     if (!nframes)
                         nframes=1;
                     out_file_open(outFile, rate, &channels);
                }
                else
                {
                    int lost=0;
                    packet_no++;
                    if (loss_percent>0 && 100*((float)rand())/RAND_MAX<loss_percent)
                        lost=1;

                    /*End of stream condition*/
                    if (op.e_o_s && os.serialno == speex_serialno) /* don't care for anything except speex eos */
                        eos=1;

                    /*Copy Ogg packet to Speex bitstream*/
                    speex_bits_read_from(&bits, (char*)op.packet, op.bytes);
                    for (j=0;j!=nframes;j++)
                    {
                        int ret;
                        /*Decode frame*/
                        if (!lost)
                            ret = speex_decode_int(st, &bits, output);
                        else
                            ret = speex_decode_int(st, NULL, output);

                        /*for (i=0;i<frame_size*channels;i++)
                        printf ("%d\n", (int)output[i]);*/
                        if (ret==-1)
                            break;
                        if (ret==-2)
                        {
                            WriteDebugLog("Decoding error: corrupted stream?");
                            //fprintf (stderr, "Decoding error: corrupted stream?\n");
                            break;
                        }
                        if (speex_bits_remaining(&bits)<0)
                        {
                            WriteDebugLog("Decoding overflow: corrupted stream?");
                            //fprintf (stderr, "Decoding overflow: corrupted stream?\n");
                            break;
                        }
                        if (channels==2)
                            speex_decode_stereo_int(output, frame_size, &stereo);

                        if (print_bitrate) 
                        {
                            spx_int32_t tmp;
                            char ch=13;
                            char szLog[100] = {0};
                            speex_decoder_ctl(st, SPEEX_GET_BITRATE, &tmp);
                            fputc (ch, stderr);
                            sprintf(szLog,"Bitrate is use: %d bps     ", tmp);
                            WriteDebugLog(szLog);
                            //fprintf (stderr, "Bitrate is use: %d bps     ", tmp);
                        }
                        /*Convert to short and save to output file*/
                        for (i=0;i<frame_size*channels;i++)
                            out[i]=output[i];
                        
                        {
                            int frame_offset = 0;
                            int new_frame_size = frame_size;
                            /*printf ("packet %d %d\n", packet_no, skip_samples);*/
                            /*fprintf (stderr, "packet %d %d %d\n", packet_no, skip_samples, lookahead);*/
                            if (packet_no == 1 && j==0 && skip_samples > 0)
                            {
                                /*printf ("chopping first packet\n");*/
                                new_frame_size -= skip_samples+lookahead;
                                frame_offset = skip_samples+lookahead;
                            }
                            if (packet_no == page_nb_packets && skip_samples < 0)
                            {
                                int packet_length = nframes*frame_size+skip_samples+lookahead;
                                new_frame_size = packet_length - j*frame_size;
                                if (new_frame_size<0)
                                 new_frame_size = 0;
                                if (new_frame_size>frame_size)
                                 new_frame_size = frame_size;
                                /*printf ("chopping end: %d %d %d\n", new_frame_size, packet_length, packet_no);*/
                            }
                            if (new_frame_size>0)
                            {  
                                #if defined WIN32 || defined _WIN32
                                 WIN_Play_Samples (out+frame_offset*channels, sizeof(short) * new_frame_size*channels);
                                #endif
                                 audio_size+=sizeof(short)*new_frame_size*channels;
                            }
                        }
                    }
                }
                packet_count++;

                if (g_bEnd)
                    break;
            }
        }
        if (feof(fin))
            break;
        if (g_bEnd)
            break;
    }

end:
    if (st)
        speex_decoder_destroy(st);
    else 
    {
        WriteDebugLog("play Speex file failed");
    }
    if (stream_init)
        ogg_stream_clear(&os);

    if(fin)
        fclose(fin);

    WIN_Audio_close ();

    if (!g_bEnd)
    {
        COPYDATASTRUCT cpyData = {0};
        cpyData.lpData = (PVOID)inFile;
        cpyData.cbData = (DWORD)strlen(inFile);
        cpyData.dwData = 0; //��ʾ������������
        sendTeamTalkMessage(&cpyData);
    }

    free(inFile);
    inFile = 0;
    if(g_hMsgWindow)
        PostMessage(g_hMsgWindow,WM_QUIT,0,0);
    else
        exit(1);

    _endthread();

    return 0;
}

int main(int argc, char **argv)
{
    char* inFile = 0;
    inFile = argv[1];
    if(0 == inFile || strlen(inFile) < 1)
        exit(1);

    if ( 0 == waveOutGetNumDevs())
    {
        COPYDATASTRUCT cpyData = {0};
        cpyData.dwData = 2; //��ʾδ�����������豸
        sendTeamTalkMessage(&cpyData);
        WriteDebugLog("No audio device present");
        exit(1);
    }

    WriteDebugLog(inFile);
    InitializeCriticalSection ( &cs );

    CreatWindow(0);
    startPlayAudio(inFile);

    /*Init Ogg data struct*/
    ogg_sync_init(&oy);
    speex_bits_init(&bits);

    MessagePump();

    speex_bits_destroy(&bits);
    ogg_sync_clear(&oy);

    DeleteCriticalSection (&cs);

    return 0;
}
