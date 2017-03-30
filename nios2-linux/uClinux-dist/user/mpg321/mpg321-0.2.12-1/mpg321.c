/*
    mpg321 - a fully free clone of mpg123.
    Copyright (C) 2001 Joe Drew
    Copyright (C) 2006-2010 Nanakos Chrysostomos
    
    Originally based heavily upon:
    plaympeg - Sample MPEG player using the SMPEG library
    Copyright (C) 1999 Loki Entertainment Software
    
    Also uses some code from
    mad - MPEG audio decoder
    Copyright (C) 2000-2001 Robert Leslie
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <libgen.h>
#include <signal.h>
#include <limits.h>
#include <sys/time.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "getopt.h" /* GNU getopt is needed, so I included it */
#include "mpg321.h"
#include <sys/wait.h>

#include <id3tag.h>

/*xterm title setting -- (c) Copyright Nanakos Chrysostomos  06/02/2005 (nanakos@wired-net.gr)*/
#include <termios.h>
#include <config.h>

FILE *ctty, *tty_in, *tty_out;
int TTY_FILENO;

struct termios tty_ts, tty_ts_orig; /* termios*/
struct termios *tty_ts_orig_pt = NULL;
static char temp[1024];
char *ctty_path();
int set_xterm = 0;
char title[BUFSIZ];

int shuffle_play;
int stop_playing_file = 0;
int quit_now = 0;
char *playlist_file;
ao_device *playdevice=NULL;
mad_timer_t current_time;
mpg321_options options = { 0, NULL, NULL, 0 , 0, 0};
int status = MPG321_STOPPED;
int file_change = 0;

char *id3_get_tag (struct id3_tag const *tag, char const *what, unsigned int maxlen);

/* Ignore child processes from the AudioScrobbler helper */
RETSIGTYPE handle_sigchld(int sig)
{
	int stat;
	while(waitpid(-1, &stat, WNOHANG) > 0)
		;
}

static struct {
	int index;
	const char *id;
	const char *name;
} const info_id3[] = {
	{ 0 ,	ID3_FRAME_TITLE,	"Title	: "	},
	{ 1 ,	ID3_FRAME_ARTIST,	" Artist : "	},
	{ 2 ,	ID3_FRAME_ALBUM,	"Album	: "	},
	{ 3 ,	ID3_FRAME_YEAR,		" Year	 : "	},
	{ 4 ,	ID3_FRAME_COMMENT,	"Comment : "	},
	{ 5 ,	ID3_FRAME_GENRE,	" Genre : "	}
};

/* Parse an ID3 tag into TITLE, ARTIST, ALBUM, YEAR, COMMENT, GENRE
 * and return true if at least one of those is present.
 */

static int parse_id3( char *names[], const struct id3_tag *tag)
{
	int found, i;

	found = 0;
	/* Get ID3 tag if available, 30 chars except 4 for year */
	for(i=0; i<=5; i++)
	{
		names[i] = NULL;
		names[i] = id3_get_tag(tag, info_id3[i].id, (i==3) ? 4 : 30);
		if( names[i] != NULL && names[i][0] != '\0' )
			found = 1;
	}

	return (found);
}





void mpg123_boilerplate()
{
    fprintf(stderr,"High Performance MPEG 1.0/2.0/2.5 Audio Player for Layer 1, 2, and 3.\n"
        "Version " FAKEVERSION " (" VERSIONDATE "). Written and copyrights by Joe Drew,\n"
	"now maintained by Nanakos Chrysostomos and others.\n"
        "Uses code from various people. See 'README' for more!\n"
        "THIS SOFTWARE COMES WITH ABSOLUTELY NO WARRANTY! USE AT YOUR OWN RISK!\n");
}

/* print the error in errno */
void mpg321_error(char *file)
{
    char *error = strerror(errno);
    fprintf(stderr, "%s: %s\n",file, error);
}

void usage(char *argv0)
{
    mpg123_boilerplate();
    fprintf(stderr,
        "\nUsage: %s [options] file(s) | URL(s) | -\n\n"
        "Options supported:\n"
        "   --verbose or -v          Increase verbosity\n"
        "   --quiet or -q            Quiet mode (no title or boilerplate)\n"
        "   --gain N or -g N         Set gain (audio volume) to N (0-100)\n"
        "   --skip N or -k N         Skip N frames into the file\n"
        "   --frames N or -n N       Play only the first N frames\n"
        "   -o dt                    Set output devicetype to dt\n" 
        "                                [esd,alsa(09),arts,sun,oss]\n"
        "   --audiodevice N or -a N  Use N for audio-out\n"
        "   --stdout or -s           Use stdout for audio-out\n"
        "   --au N                   Use au file N for output\n"
        "   --cdr N                  Use wave file N for output\n"
        "   --wav N or -w N          Use wave file N for output\n"
        "   --test or -t             Test only; do no audio output\n"
        "   --list N or -@ N         Use playlist N as list of MP3 files\n"
        "   --random or -Z           Play files randomly until interrupted\n"
        "   --shuffle or -z          Shuffle list of files before playing\n"
        "   --loop N or -l N         Play files N times. 0 means until\n"
        "                            interrupted\n"
        "   -R                       Use remote control interface\n"
        "   -F                       Turn on FFT analysis on PCM data. Remote mode only\n"
        "   -B                       Read recursively the given directories\n"
        "   -S                       Report mp3 file to AudioScrobbler\n"
        "   -x                       Set xterm title setting\n"
        "   -p hostname:port         Use proxy server\n"
        "   -u username:password     Use proxy server basic authentication\n"
        "   -U username:password     Use proxy server basic authentication by using environment variables\n"
        "   --aggressive             Try to get higher priority\n"
        "   --help or --longhelp     Print this help screen\n"
        "   --version or -V          Print version information\n"
        "\n"
        "This version of mpg321 has been configured with " AUDIO_DEFAULT " as its default\n"
        "libao output device.\n" , argv0);
}

/* retsigtype is defined by configure;
   pass handle_signals -1 to initialize it */
RETSIGTYPE handle_signals(int sig) 
{
    static struct timeval last_signal = { 0, 0 };
    struct timeval curr_tv;
    
    gettimeofday(&curr_tv, NULL);
    
    if (sig == SIGINT)
    {
        if (((curr_tv.tv_sec - last_signal.tv_sec) * 1000000 +  
            (curr_tv.tv_usec - last_signal.tv_usec)) < 1000000) /* Allow a short delay to
                                                                  kill mpg321 */
        {
            quit_now = 1;
        }
            
        last_signal.tv_sec = curr_tv.tv_sec;
        last_signal.tv_usec = curr_tv.tv_usec;
        
        stop_playing_file = 1;
   }
   else if (sig == -1) /* initialize */
   {
        last_signal.tv_sec = curr_tv.tv_sec;
        last_signal.tv_usec = curr_tv.tv_usec;
   }
}

/*
    Only shows ID3 tags if at least one of
    TITLE, ARTIST, ALBUM, YEAR, COMMENT, GENRE
    is present.
*/
static int show_id3(struct id3_tag const *tag)
{
    unsigned int i;
    char emptystring[31];
    char *names[6];

    memset (emptystring, ' ', 30);
    emptystring[30] = '\0';
    
    if (!parse_id3(names, tag)) {
        return 0;
    }

    if (options.opt & MPG321_REMOTE_PLAY)
    {
        printf("@I ID3:");

        for (i=0; i<=5; i++)
        {
            if(!names[i])
            {
                printf(emptystring);
            }
            
            else
            {
                printf("%s", names[i]);
                free(names[i]);
            }
        }
        printf("\n");
    }
    
    else
    {
        /* Emulate mpg123 original behaviour  */
        for (i=0; i<=5; i++)    {
            fprintf (stderr, "%s", info_id3[i].name);
            if (!names[i])  {
                fprintf (stderr, emptystring);
            }   else    {
                fprintf (stderr, "%s", names[i]);
                free (names[i]);
            }
            if (i%2) fprintf (stderr, "\n");
        }
    }

    return 1;
}

static int get_id3_info( const char *fname, struct id3_file **id3struct, struct id3_tag **id3tag)
{
	struct id3_file *s;
	struct id3_tag *t;

	s = id3_file_open(fname, ID3_FILE_MODE_READONLY);
	if( s == NULL )
		return (0);

	t = id3_file_tag(s);
	if(t == NULL)
	{
		id3_file_close(s);
		return (0);
	}

	*id3struct = s;
	*id3tag = t;
	return (1);
}


int main(int argc, char *argv[])
{
    int fd = 0;
    char *currentfile, old_dir[PATH_MAX];
    playlist *pl = NULL;
    struct id3_file *id3struct = NULL;
    struct id3_tag *id3tag = NULL;

    buffer playbuf;
    
    struct mad_decoder decoder;

    old_dir[0] = '\0';

    playbuf.pl = pl = new_playlist();

    if (!pl)
    {
        fprintf(stderr, "malloc failed at startup!\n");
        exit(1);
    }

    loop_remaining = 1;

    options.volume = MAD_F_ONE;

    status = MPG321_PLAYING;
    
    /* Get the command line options */
    parse_options(argc, argv, pl);

    if(options.opt & MPG321_PRINT_FFT)
	    if(!(options.opt & MPG321_REMOTE_PLAY))
	    {
		    /* printf("FFT analysis can only be used in Remote mode play.\n\n"); */
		    usage(argv[0]);			
		    exit(0);
	    }


    /* If there were no files and no playlist specified just print the usage */
    if (!playlist_file && optind == argc)
    {
        usage(argv[0]);
        exit(0);
    }

    if (playlist_file)
        load_playlist(pl, playlist_file);

    if(options.opt & MPG321_RECURSIVE_DIR)
	    add_cmdline_files_recursive_dir(pl, argv);
    else
	    add_cmdline_files(pl, argv);

    if (shuffle_play)
        shuffle_files(pl);

    ao_initialize();

    check_default_play_device();
    
    if (!(options.opt & MPG321_REMOTE_PLAY))
    {
        handle_signals(-1); /* initialize signal handler */
     
        remote_input_buf[0] = '\0';
    }
    
    if (!(options.opt & MPG321_QUIET_PLAY)) 
        mpg123_boilerplate();
    
    if (options.opt & MPG321_REMOTE_PLAY)
    {
        printf ("@R MPG123\n");
    }
    if(set_xterm)
    {
	    tty_control();
	    get_term_title(title);
    }
    /* Play the mpeg files or zip it! */
    while((currentfile = get_next_file(pl, &playbuf)))
    {
        //printf("Current File: %s\n",currentfile);
	   if (quit_now) 
            break;
        
        signal(SIGINT, SIG_DFL);
        
        playbuf.buf = NULL;
        playbuf.fd = -1;
        playbuf.length = 0;
        playbuf.done = 0;
        playbuf.num_frames = 0;
        current_frame = 0;
        playbuf.max_frames = -1;
        strncpy(playbuf.filename,currentfile, PATH_MAX);
        playbuf.filename[PATH_MAX-1] = '\0';
        
        if (status == MPG321_PLAYING || status == MPG321_STOPPED) 
            file_change = 1;

        mad_timer_reset(&playbuf.duration);
        
        mad_timer_reset(&current_time);

	id3struct = NULL;

        if (!(options.opt & MPG321_QUIET_PLAY) && file_change)
        {
           /* id3struct = id3_file_open (currentfile, ID3_FILE_MODE_READONLY);*/

            if (id3struct == NULL)
		    get_id3_info(currentfile, &id3struct, &id3tag);
	    if(id3tag)
		    show_id3(id3tag);
	}

	scrobbler_time = -1;
	if(options.opt & MPG321_USE_SCROBBLER)
	{
		if(id3struct == NULL)
			get_id3_info(currentfile,&id3struct,&id3tag);
                
	    if (id3tag)
	    {
		    char emptystring[31], emptyyear[5] = "    ";
		    int i;

		    if(parse_id3(scrobbler_args, id3tag))
		    {
			    memset(emptystring, ' ', 30);
			    emptystring[30] = '\0';
	    		    if((options.opt & MPG321_VERBOSE_PLAY) && (options.opt & MPG321_USE_SCROBBLER))
			    {
				    fprintf(stderr, "\nPreparing for the AudioScrobbler:\n");
				    for(i = 0; i < 6; i++)
				    {
					    if(scrobbler_args[i] == NULL)
						    scrobbler_args[i] =
							    ( i == 3 ? emptyyear: emptystring);
					    fprintf(stderr, "- %s\n", scrobbler_args[i]);
				    }
			    }
		    }
	    }
	}
      

        if (options.opt & MPG321_REMOTE_PLAY && file_change)
        {
		if(id3struct == NULL)
			get_id3_info(currentfile, &id3struct, &id3tag);
		if(id3tag)
                {
                    if (!show_id3(id3tag))
                    {
                        /* This shouldn't be necessary, but it appears that
                           libid3tag doesn't necessarily know if there are no
                           id3 tags on a given mp3 */
                        char * basec = strdup(currentfile);
                        char * basen = basename(basec);
                        
                        char * dot = strrchr(basen, '.');
                        
                        if (dot)
                            *dot = '\0';
                        
                        printf("@I %s\n", basen);

                        free(basec);
                    }
                }
                
            
            else
            {
                char * basec = strdup(currentfile);
                char * basen = basename(basec);
                
                char * dot = strrchr(basen, '.');
                
                if (dot)
                    *dot = '\0';
                
                printf("@I %s\n", basen);

                free(basec);
            }
        }

	if(id3struct != NULL)
		id3_file_close(id3struct);

        /* Create the MPEG stream */
        /* Check if source is on the network */
        if((fd = raw_open(currentfile)) != 0 || (fd = http_open(currentfile)) != 0
            || (fd = ftp_open(currentfile)) != 0)
        {
            playbuf.fd = fd;
            playbuf.buf = malloc(BUF_SIZE);
            playbuf.length = BUF_SIZE;
            
            mad_decoder_init(&decoder, &playbuf, read_from_fd, read_header, /*filter*/0,
                            output, handle_error, /* message */ 0);
        }

        /* Check if we are to use stdin for input */
        else if(strcmp(currentfile, "-") == 0)
        {
            playbuf.fd = fileno(stdin);
            playbuf.buf = malloc(BUF_SIZE);
            playbuf.length = BUF_SIZE;

            mad_decoder_init(&decoder, &playbuf, read_from_fd, read_header, /*filter*/0,
                            output, handle_error, /* message */ 0);
        }
            
        /* currentfile is a local file (presumably.) mmap() it */
        else
        {
            struct stat stat;
            
            if((fd = open(currentfile, O_RDONLY)) == -1)
            {
                mpg321_error(currentfile);
		exit(1);
                /* mpg123 stops immediately if it can't open a file */
		/* If sth goes wrong break!!!*/
                break;
            }
            
            if(fstat(fd, &stat) == -1)
            {
                mpg321_error(currentfile);
                continue;
            }
            
            if (!S_ISREG(stat.st_mode))
            {
                continue;
            }
            
            calc_length(currentfile, &playbuf);

	    if((options.opt & MPG321_VERBOSE_PLAY) && (options.opt & MPG321_USE_SCROBBLER))
		    fprintf(stderr, "Track duration: %ld seconds\n",playbuf.duration.seconds);

	    if(options.opt & MPG321_USE_SCROBBLER)
		    scrobbler_set_time(playbuf.duration.seconds);

            if ((options.maxframes != -1) && (options.maxframes <= playbuf.num_frames))
            { 
                playbuf.max_frames = options.maxframes;
            }
            
            playbuf.frames = malloc((playbuf.num_frames + 1) * sizeof(void*));
            playbuf.times = malloc((playbuf.num_frames + 1) * sizeof(mad_timer_t));
    
            if((playbuf.buf = mmap(0, playbuf.length, PROT_READ, MAP_PRIVATE, fd, 0))
                                == MAP_FAILED)
            {
                mpg321_error(currentfile);
                continue;
            }
            
            playbuf.frames[0] = playbuf.buf;
            
            mad_decoder_init(&decoder, &playbuf, read_from_mmap, read_header, /*filter*/0,
                            output, handle_error, /* message */ 0);
        }

        if(!(options.opt & MPG321_QUIET_PLAY))/*zip it!!!*/
        {
            /* Because dirname might modify the argument */
            char * dirc = strdup(currentfile);
            char * basec = strdup(currentfile);

            char * basen = basename(basec);
            char * dirn = dirname(dirc);
            
            /* make sure that the file has a pathname; otherwise don't print out
               a Directory: listing */
            if(strchr(currentfile, '/') && strncmp(old_dir, dirn, PATH_MAX) != 0)
            {
                /* Print information about the file */
                fprintf(stderr, "\n");
                fprintf(stderr,"Directory: %s/\n", dirn);
                
                strncpy(old_dir, dirn, PATH_MAX);
                old_dir[PATH_MAX-1] = '\0';
            }
            
            /* print a newline between different songs only, not after
               Directory: listing */
            else
            {
                fprintf(stderr, "\n");
            }

            fprintf(stderr,"Playing MPEG stream from %s ...\n", basen);
            
			/*Printing xterm title*/
	    if(set_xterm)
	    {
		    osc_print(0,0,basen);
	    }
	    
            free(dirc);
            free(basec);
        }    

        signal(SIGINT, handle_signals);
	signal(SIGCHLD, handle_sigchld);
        /*Give control back so that we can implement SIG's*/
	if(set_xterm)
	{
		set_tty_restore();
	}
        /* Every time the user gets us to rewind, we exit decoding,
           reinitialize it, and re-start it */
      
        while (1)
        {
            decoder.options |= MAD_OPTION_IGNORECRC;
            mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
            
            /* if we're rewinding on an mmap()ed stream */
            if(status == MPG321_REWINDING && playbuf.fd == -1) 
            {
                mad_decoder_init(&decoder, &playbuf, read_from_mmap, read_header, /*filter*/0,
                    output, /*error*/0, /* message */ 0);
            }    
            else
                break;
        } 

        if (!(options.opt & MPG321_QUIET_PLAY))
        {
            char time_formatted[11];
            mad_timer_string(current_time, time_formatted, "%.1u:%.2u", MAD_UNITS_MINUTES,
                       MAD_UNITS_SECONDS, 0);
            fprintf(stderr, "\n[%s] Decoding of %s finished.\n",time_formatted, basename(currentfile));
        }
        
        if (options.opt & MPG321_REMOTE_PLAY && status == MPG321_STOPPED)
        {
            clear_remote_file(pl);
        }

        mad_decoder_finish(&decoder);

        if (playbuf.frames)
             free(playbuf.frames);

        if (playbuf.times)
            free(playbuf.times);
            
        if (playbuf.fd == -1)
        {
            munmap(playbuf.buf, playbuf.length);
            close(fd);
        }

        else
        {
            free(playbuf.buf);
            if (playbuf.fd != fileno(stdin)) 
                close(playbuf.fd);
        }
    }

    if(playdevice)
        ao_close(playdevice);

    ao_shutdown();
    /*Restoring TTY*/
    if(set_xterm)
    {
	    set_tty_restore();
	    osc_print(0,0,title);
	    if (ctty)
		    fclose(ctty);
    }
    return(0);
}


/* Convenience for retrieving already formatted id3 data
 * what parameter is one of
 *  ID3_FRAME_TITLE
 *  ID3_FRAME_ARTIST
 *  ID3_FRAME_ALBUM
 *  ID3_FRAME_YEAR
 *  ID3_FRAME_COMMENT
 *  ID3_FRAME_GENRE
 * It allocates a new string. Free it later.
 * NULL if no tag or error.
 */
char *id3_get_tag (struct id3_tag const *tag, char const *what, unsigned int maxlen)
{
    struct id3_frame const *frame = NULL;
    union id3_field const *field = NULL;
    int nstrings;
    int avail;
    int j;
    int tocopy;
    int len;
    char printable [1024];
    char *retval = NULL;
    id3_ucs4_t const *ucs4 = NULL;
    id3_latin1_t *latin1 = NULL;

    memset (printable, '\0', 1024);
    avail = 1024;
    if (strcmp (what, ID3_FRAME_COMMENT) == 0)
    {
        /*There may be sth wrong. I did not fully understand how to use
            libid3tag for retrieving comments  */
        j=0;
        frame = id3_tag_findframe(tag, ID3_FRAME_COMMENT, j++);
        if (!frame) return (NULL);
        ucs4 = id3_field_getfullstring (&frame->fields[3]);
        if (!ucs4) return (NULL);
        latin1 = id3_ucs4_latin1duplicate (ucs4);
        if (!latin1 || strlen(latin1) == 0) return (NULL);
        len = strlen(latin1);
        if (avail > len)
            tocopy = len;
        else
            tocopy = 0;
        if (!tocopy) return (NULL);
        avail-=tocopy;
        strncat (printable, latin1, tocopy);
        free (latin1);
    }
    
    else
    {
        frame = id3_tag_findframe (tag, what, 0);
        if (!frame) return (NULL);
        field = &frame->fields[1];
        nstrings = id3_field_getnstrings(field);
        for (j=0; j<nstrings; ++j)
        {
            ucs4 = id3_field_getstrings(field, j);
            if (!ucs4) return (NULL);
            if (strcmp (what, ID3_FRAME_GENRE) == 0)
                ucs4 = id3_genre_name(ucs4);
            latin1 = id3_ucs4_latin1duplicate(ucs4);
            if (!latin1) break;
            len = strlen(latin1);
            if (avail > len)
                tocopy = len;
            else
                tocopy = 0;
            if (!tocopy) break;
            avail-=tocopy;
            strncat (printable, latin1, tocopy);
            free (latin1);
        }
    }
    retval = malloc (maxlen + 1);
    if (!retval) return (NULL);

    strncpy (retval, printable, maxlen);
    retval[maxlen] = '\0';

    len = strlen(printable);
    if (maxlen > len)
    {
        memset (retval + len, ' ', maxlen - len);
    }

    return (retval);
}

int tty_control()
{
    ctty = NULL;

    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
      /* get controlling terminal */
      char *program_name = "mpg321";
      if ((ctty = fopen((char *)ctty_path(), "r+")) == NULL) {
	fprintf(stderr, "%s: failed to get controlling terminal\n",program_name );
        exit(EXIT_FAILURE);
      }
    }
    if (!isatty(STDIN_FILENO) && ctty) {
        tty_in = ctty;
        TTY_FILENO = fileno(tty_in);
    } else {
        tty_in = stdin;
        TTY_FILENO = STDIN_FILENO;
    }
    if (!isatty(STDOUT_FILENO) && ctty)
        tty_out = ctty;
    else
        tty_out = stdout;

    set_tty_raw();
    return 0;
	
}

int set_tty_raw()
{
    /* get and backup tty_in termios */
    tcgetattr(TTY_FILENO, &tty_ts);
    tty_ts_orig = tty_ts;
    tty_ts_orig_pt = &tty_ts_orig;

    /* set tty raw */
    tty_ts.c_iflag = 0;
    tty_ts.c_lflag = 0;

    tty_ts.c_cc[VMIN] = 1;
    tty_ts.c_cc[VTIME] = 1;
    tty_ts.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(TTY_FILENO, TCSANOW, &tty_ts);
    return 0;
}

int set_tty_restore(void)
{
    /* restore tty mode */
    if (tty_ts_orig_pt)
	tcsetattr(TTY_FILENO, TCSAFLUSH, tty_ts_orig_pt);
    return 0;
}


/* issue raw escape sequence */
int raw_print(char *ctlseq)
{
   int c;
   while ((c = *ctlseq++)) {
      if (c == '\\' && *ctlseq) {
         switch (c = *ctlseq++) {
            case 'e':
               c = '\033';
               break;
            case 'a':
               c = '\007';
               break;
            case 'b':
               c = '\b';
               break;
            case 'f':
               c = '\f';
               break;
            case 'n':
               c = '\n';
               break;
            case 'r':
               c = '\r';
               break;
            case 't':
               c = '\t';
               break;
            case 'v':
               c = (int) 0x0B;
               break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
               c -= '0';
               if (*ctlseq >= '0' && *ctlseq <= '7')
                  c = (c * 8) + (*ctlseq++ - '0');
               if (*ctlseq >= '0' && *ctlseq <= '7')
                  c = (c * 8) + (*ctlseq++ - '0');
               break;
            case '\\':
               break;
            default:
               fputc('\\', tty_out);
               break;
         }
      }
  fputc(c, tty_out);
   }
   fflush(tty_out);
   return 0;
}

int osc_print(int ps1, int ps2, char* pt)
{

    if (pt && *pt)
	snprintf(temp, sizeof(temp), "\033]%d;%s\007", ps1, pt);
    else
        snprintf(temp, sizeof(temp), "\033]%d;?\007", ps1);
   raw_print(temp);
   return 0;
}

/* find path to controlling terminal */
char *ctty_path()
{
   int i;
   char* tty_path = NULL;
   for (i = 0; i <= 2; i++)
       if ((tty_path = ttyname(i)))
                break;
   return tty_path;
}

int tty_read(char *output,size_t size)
{
	int n;

	n = read(TTY_FILENO,output,size-1);
	if(n == -1)
	{
		perror("read");
	}
	 return n;
}

void get_term_title(char *title)
{
	int n;
	static char buffer[BUFSIZ];
	char temp[20];

	snprintf(temp,sizeof(temp),"\033[%dt",21);

	raw_print(temp);

	n = tty_read(buffer,sizeof(buffer));
	if(n==1)
		n+= tty_read(buffer+1,sizeof(buffer)-1);

	while( !(buffer[n-2] == '\033' && buffer[n-1] == '\\') ) {
		n += tty_read(buffer+n,sizeof(buffer)-n);
	}

	buffer[n-2]= '\0';

	snprintf((char *)title,sizeof(buffer),"%s",buffer+3);
}

static enum mad_flow handle_error(void *data, struct mad_stream *stream, struct mad_frame *frame)
{
	signed long tagsize;

	switch(stream->error)
	{
		case MAD_ERROR_BADDATAPTR:
			return MAD_FLOW_CONTINUE;
		case MAD_ERROR_LOSTSYNC:
			tagsize = id3_tag_query(stream->this_frame,stream->bufend - stream->this_frame);
			if(tagsize > 0)
			{
				mad_stream_skip(stream, tagsize);
				return MAD_FLOW_CONTINUE;
			}

		default:
			break;
	}

	if(stream->error == MAD_ERROR_BADCRC)
	{
		mad_frame_mute(frame);
		return MAD_FLOW_IGNORE;
	}

	return MAD_FLOW_CONTINUE;
}


