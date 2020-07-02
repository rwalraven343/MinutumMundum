//Code by Rogier Walraven. Do not distribute.

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

#include <AL/alut.h>
#include <vorbis/vorbisfile.h>

#include "audio.h"

static void decode_music_stream_and_fill_buffer(ALuint musicbuffer);

static double mastervolume;
static double mastervolume_sfx;
static double mastervolume_music;

static double spatial_scale;
static double spatial_scale_fixed;

static ALfloat listener_pos[3];
static ALfloat listener_vel[3];
static ALfloat listener_ori[6];

extern const int max_unique_samples;
extern const int max_audio_channels;

struct REFCOUNTEDSAMPLE
{
	const char *filename;
	ALuint sample;
	int refcount;

	REFCOUNTEDSAMPLE() : filename(0) {}
};

struct AUDIOCHANNEL
{
	bool available;
	ALuint source;

	AUDIOCHANNEL() : available(false) {}
};

static REFCOUNTEDSAMPLE *refcountedsamples;
static AUDIOCHANNEL *audiochannels;

static const char *musicfilename;

static OggVorbis_File oggvorbisfile;

static ALuint  musicbuffers[2];
static ALenum  musicformat;
static ALsizei musicfrequency;
static int     musicchannel;

static bool music_stopped;
static bool end_of_music_stream;

static const int pcm_buffer_size=16384;
static char pcm_buffer[pcm_buffer_size];

void AUDIO::init()
{
	alutInit(0,0);

	mastervolume      =1;
	mastervolume_sfx  =1;
	mastervolume_music=1;

	spatial_scale      =1;
	spatial_scale_fixed=1;

	refcountedsamples=new REFCOUNTEDSAMPLE[max_unique_samples];
	audiochannels    =new AUDIOCHANNEL    [max_audio_channels];

	for (int i=0;i<max_audio_channels;i++)
	{
		alGetError();
		alGenSources(1,&(audiochannels[i].source));

		if (alGetError()==AL_NO_ERROR)
		{
			audiochannels[i].available=true;
		}
	}
	alGetError();

	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	for (int i=0;i<max_audio_channels;i++)
	{
		if (audiochannels[i].available)
		{
			alSourcef(audiochannels[i].source,AL_REFERENCE_DISTANCE,32);
			alSourcef(audiochannels[i].source,AL_MAX_DISTANCE,16777216);
			alSourcef(audiochannels[i].source,AL_ROLLOFF_FACTOR,1);
		}
	}

	alSpeedOfSound(512);
	alDopplerFactor(1);
}

void AUDIO::quit()
{
	for (int i=0;i<max_audio_channels;i++)
	{
		if (audiochannels[i].available)
		{
			alSourceRewind(audiochannels[i].source);
			alSourcei(audiochannels[i].source,AL_BUFFER,0);
			alDeleteSources(1,&(audiochannels[i].source));
		}
	}

	delete[] refcountedsamples;
	delete[] audiochannels;

	alutExit();
}

double AUDIO::getmastervolume()
{
	return(mastervolume);
}

void   AUDIO::setmastervolume(double vol)
{
	mastervolume=vol;
}

double AUDIO::getmastervolume_sfx()
{
	return(mastervolume_sfx);
}

void   AUDIO::setmastervolume_sfx(double vol)
{
	mastervolume_sfx=vol;
}

double AUDIO::getmastervolume_music()
{
	return(mastervolume_music);
}

void   AUDIO::setmastervolume_music(double vol)
{
	mastervolume_music=vol;
}

void AUDIO::setspatial_scale      (double sc)
{
	spatial_scale=sc;
}

void AUDIO::setspatial_scale_fixed(double sc)
{
	spatial_scale_fixed=sc;
}

void AUDIO::listener_topdown  (double xpos,double ypos,double xvel,double yvel,double angle)
{
	listener_pos[0]=(ALfloat)( xpos*spatial_scale);
	listener_pos[1]=(ALfloat)(-ypos*spatial_scale);
	listener_pos[2]=0;

	listener_vel[0]=(ALfloat)( xvel*spatial_scale);
	listener_vel[1]=(ALfloat)(-yvel*spatial_scale);
	listener_vel[2]=0;

	double rad=((M_PI)/180)*angle;

	listener_ori[0]=(ALfloat)sin(rad);
	listener_ori[1]=(ALfloat)cos(rad);
	listener_ori[2]=0;
	listener_ori[3]=0;
	listener_ori[4]=0;
	listener_ori[5]=1;

	alListenerf (AL_GAIN,(ALfloat)mastervolume);
	alListenerfv(AL_POSITION,listener_pos);
	alListenerfv(AL_VELOCITY,listener_vel);
	alListenerfv(AL_ORIENTATION,listener_ori);
}

void AUDIO::listener_platform (double xpos,double ypos,double xvel,double yvel)
{
	listener_pos[0]=(ALfloat)( xpos*spatial_scale);
	listener_pos[1]=(ALfloat)(-ypos*spatial_scale);
	listener_pos[2]=0;

	listener_vel[0]=(ALfloat)( xvel*spatial_scale);
	listener_vel[1]=(ALfloat)(-yvel*spatial_scale);
	listener_vel[2]=0;

	listener_ori[0]=0;
	listener_ori[1]=0;
	listener_ori[2]=-1;
	listener_ori[3]=0;
	listener_ori[4]=1;
	listener_ori[5]=0;

	alListenerf (AL_GAIN,(ALfloat)mastervolume);
	alListenerfv(AL_POSITION,listener_pos);
	alListenerfv(AL_VELOCITY,listener_vel);
	alListenerfv(AL_ORIENTATION,listener_ori);
}

AUDIO::SOURCE::SOURCE(const char *filename,double loudness,bool fixed)
{
	this->fixed=fixed;
	this->loudness=loudness;

	channel=-1;

	for (int i=0;i<max_unique_samples;i++)
	{
		if (refcountedsamples[i].filename)
		{
			if (strcmp(refcountedsamples[i].filename,filename)==0)
			{
				refcountedsamples[i].refcount++;

				sample=(void*)(refcountedsamples+i);

				return;
			}
		}
	}

	for (int i=0;i<max_unique_samples;i++)
	{
		if (!refcountedsamples[i].filename)
		{
			refcountedsamples[i].filename=filename;
			refcountedsamples[i].sample=alutCreateBufferFromFile(filename);
			refcountedsamples[i].refcount=1;

			sample=(void*)(refcountedsamples+i);

			return;
		}
	}

	sample=0;
}

AUDIO::SOURCE::~SOURCE()
{
	if (channel!=-1)
	{
		alSourceRewind(audiochannels[channel].source);
		alSourcei(audiochannels[channel].source,AL_BUFFER,0);
	}

	for (int i=0;i<max_unique_samples;i++)
	{
		if (refcountedsamples[i].filename)
		{
			if ((void*)(refcountedsamples+i)==sample)
			{
				refcountedsamples[i].refcount--;

				if (refcountedsamples[i].refcount==0)
				{
					refcountedsamples[i].filename=0;

					alDeleteBuffers(1,&(((REFCOUNTEDSAMPLE*)sample)->sample));
				}

				return;
			}
		}
	}
}

bool AUDIO::SOURCE::initial() const
{
	return(channel==-1);
}

bool AUDIO::SOURCE::playing() const
{
	if (channel!=-1)
	{
		ALint state;
		alGetSourcei(audiochannels[channel].source,AL_SOURCE_STATE,&state);

		return(state==AL_PLAYING);
	}

	return(false);
}

bool AUDIO::SOURCE::stopped() const
{
	if (channel!=-1)
	{
		ALint state;
		alGetSourcei(audiochannels[channel].source,AL_SOURCE_STATE,&state);

		return(state==AL_PAUSED || state==AL_STOPPED);
	}

	return(false);
}

void AUDIO::SOURCE::rewind() const
{
	if (channel!=-1)
	{
		alSourceRewind(audiochannels[channel].source);
		alSourcei(audiochannels[channel].source,AL_BUFFER,0);

		channel=-1;
	}
}

void AUDIO::SOURCE::play(double xpos,double ypos,double xvel,double yvel,double pitch,double volume) const
{
	if (channel==-1)
	{
		for (int i=0;i<max_audio_channels;i++)
		{
			if (audiochannels[i].available)
			{
				ALint state;
				alGetSourcei(audiochannels[i].source,AL_SOURCE_STATE,&state);

				if (state==AL_INITIAL)
				{
					alSourcei(audiochannels[i].source,AL_BUFFER,((REFCOUNTEDSAMPLE*)sample)->sample);

					if (fixed)
					{
						alSourcei(audiochannels[i].source,AL_SOURCE_RELATIVE,AL_TRUE);

						alSource3f(audiochannels[i].source,AL_POSITION,(ALfloat)(xpos*spatial_scale_fixed),(ALfloat)(-ypos*spatial_scale_fixed),0);
						alSource3f(audiochannels[i].source,AL_VELOCITY,(ALfloat)(xvel*spatial_scale_fixed),(ALfloat)(-yvel*spatial_scale_fixed),0);
					}
					else
					{
						alSourcei(audiochannels[i].source,AL_SOURCE_RELATIVE,AL_FALSE);

						alSource3f(audiochannels[i].source,AL_POSITION,(ALfloat)(xpos*spatial_scale),(ALfloat)(-ypos*spatial_scale),0);
						alSource3f(audiochannels[i].source,AL_VELOCITY,(ALfloat)(xvel*spatial_scale),(ALfloat)(-yvel*spatial_scale),0);
					}

					alSourcef(audiochannels[i].source,AL_PITCH,(ALfloat)pitch);
					alSourcef(audiochannels[i].source,AL_GAIN,(ALfloat)(volume*loudness*mastervolume_sfx));

					alSourcePlay(audiochannels[i].source);

					channel=i;

					return;
				}
			}
		}
	}
	else
	{
		if (fixed)
		{
			alSource3f(audiochannels[channel].source,AL_POSITION,(ALfloat)(xpos*spatial_scale_fixed),(ALfloat)(-ypos*spatial_scale_fixed),0);
			alSource3f(audiochannels[channel].source,AL_VELOCITY,(ALfloat)(xvel*spatial_scale_fixed),(ALfloat)(-yvel*spatial_scale_fixed),0);
		}
		else
		{
			alSource3f(audiochannels[channel].source,AL_POSITION,(ALfloat)(xpos*spatial_scale),(ALfloat)(-ypos*spatial_scale),0);
			alSource3f(audiochannels[channel].source,AL_VELOCITY,(ALfloat)(xvel*spatial_scale),(ALfloat)(-yvel*spatial_scale),0);
		}

		alSourcef(audiochannels[channel].source,AL_PITCH,(ALfloat)pitch);
		alSourcef(audiochannels[channel].source,AL_GAIN,(ALfloat)(volume*loudness*mastervolume_sfx));

		ALint state;
		alGetSourcei(audiochannels[channel].source,AL_SOURCE_STATE,&state);

		if (state!=AL_PLAYING)
		{
			alSourcePlay(audiochannels[channel].source);
		}
	}
}

void AUDIO::SOURCE::stop() const
{
	if (channel!=-1)
	{
		ALint state;
		alGetSourcei(audiochannels[channel].source,AL_SOURCE_STATE,&state);

		if (state==AL_PLAYING)
		{
			alSourcePause(audiochannels[channel].source);
		}
	}
}

void AUDIO::MUSIC::init(const char *filename)
{
	musicfilename=filename;

	FILE *file;
	fopen_s(&file,filename,"rb");
	ov_open(file,&oggvorbisfile,0,0);

	vorbis_info *vorbisinfo=ov_info(&oggvorbisfile,-1);

	alGenBuffers(2,musicbuffers);

	if (vorbisinfo->channels==1)
	{
		musicformat=AL_FORMAT_MONO16;
	}
	else if (vorbisinfo->channels==2)
	{
		musicformat=AL_FORMAT_STEREO16;
	}

	musicfrequency=vorbisinfo->rate;

	musicchannel=-1;

	music_stopped      =false;
	end_of_music_stream=false;

	decode_music_stream_and_fill_buffer(musicbuffers[0]);
	decode_music_stream_and_fill_buffer(musicbuffers[1]);
}

void AUDIO::MUSIC::quit()
{
	if (musicchannel!=-1)
	{
		alSourceRewind(audiochannels[musicchannel].source);
		alSourcei(audiochannels[musicchannel].source,AL_BUFFER,0);
	}

	alDeleteBuffers(2,musicbuffers);

	ov_clear(&oggvorbisfile);
}

bool AUDIO::MUSIC::initial()
{
	return(musicchannel==-1);
}

bool AUDIO::MUSIC::playing()
{
	if (musicchannel!=-1)
	{
		return(!music_stopped && !end_of_music_stream);
	}

	return(false);
}

bool AUDIO::MUSIC::stopped()
{
	if (musicchannel!=-1)
	{
		return(music_stopped || end_of_music_stream);
	}

	return(false);
}

void AUDIO::MUSIC::reload(const char *filename)
{
	if (musicchannel!=-1)
	{
		alSourceRewind(audiochannels[musicchannel].source);
		alSourcei(audiochannels[musicchannel].source,AL_BUFFER,0);
	}

	ov_clear(&oggvorbisfile);

	musicfilename=filename;

	FILE *file;
	fopen_s(&file,filename,"rb");
	ov_open(file,&oggvorbisfile,0,0);

	vorbis_info *vorbisinfo=ov_info(&oggvorbisfile,-1);

	if (vorbisinfo->channels==1)
	{
		musicformat=AL_FORMAT_MONO16;
	}
	else if (vorbisinfo->channels==2)
	{
		musicformat=AL_FORMAT_STEREO16;
	}

	musicfrequency=vorbisinfo->rate;

	musicchannel=-1;

	music_stopped      =false;
	end_of_music_stream=false;

	decode_music_stream_and_fill_buffer(musicbuffers[0]);
	decode_music_stream_and_fill_buffer(musicbuffers[1]);
}

void AUDIO::MUSIC::start()
{
	if (musicchannel==-1)
	{
		for (int i=0;i<max_audio_channels;i++)
		{
			if (audiochannels[i].available)
			{
				ALint state;
				alGetSourcei(audiochannels[i].source,AL_SOURCE_STATE,&state);

				if (state==AL_INITIAL)
				{
					alSourcef (audiochannels[i].source,AL_PITCH,1);
					alSourcef (audiochannels[i].source,AL_GAIN,(ALfloat)mastervolume_music);
					alSource3f(audiochannels[i].source,AL_POSITION,0,0,0);
					alSource3f(audiochannels[i].source,AL_VELOCITY,0,0,0);
					alSourcei (audiochannels[i].source,AL_SOURCE_RELATIVE,AL_TRUE);

					alSourceQueueBuffers(audiochannels[i].source,2,musicbuffers);

					alSourcePlay(audiochannels[i].source);

					musicchannel=i;

					return;
				}
			}
		}
	}
	else if (music_stopped && !end_of_music_stream)
	{
		music_stopped=false;
	}
	else if (music_stopped && end_of_music_stream)
	{
		reload(musicfilename);
		start();
	}
}

void AUDIO::MUSIC::stop()
{
	if (musicchannel!=-1)
	{
		music_stopped=true;
	}
}

void AUDIO::MUSIC::update()
{
	if (musicchannel!=-1 && !music_stopped && !end_of_music_stream)
	{
		alSourcef(audiochannels[musicchannel].source,AL_GAIN,(ALfloat)mastervolume_music);

		ALint processed;
		alGetSourcei(audiochannels[musicchannel].source,AL_BUFFERS_PROCESSED,&processed);

		if (processed)
		{
			ALuint musicbuffer;

			for (int i=0;i<processed;i++)
			{
				alSourceUnqueueBuffers(audiochannels[musicchannel].source,1,&musicbuffer);

				decode_music_stream_and_fill_buffer(musicbuffer);

				alSourceQueueBuffers(audiochannels[musicchannel].source,1,&musicbuffer);
			}
		}

		ALint state;
		alGetSourcei(audiochannels[musicchannel].source,AL_SOURCE_STATE,&state);

		if (state!=AL_PLAYING)
		{
			alSourcePlay(audiochannels[musicchannel].source);
		}
	}
}

static void decode_music_stream_and_fill_buffer(ALuint musicbuffer)
{
	int total_bytes_read=0;
	int bytes_read;
	int bitstream;

	while (total_bytes_read<pcm_buffer_size)
	{
		bytes_read=ov_read(&oggvorbisfile,pcm_buffer+total_bytes_read,pcm_buffer_size-total_bytes_read,0,2,1,&bitstream);

		if (bytes_read>0)
			total_bytes_read+=bytes_read;
		else
			break;
	}

	if (total_bytes_read>0)
	{
		alBufferData(musicbuffer,musicformat,pcm_buffer,total_bytes_read,musicfrequency);

		return;
	}

	music_stopped      =true;
	end_of_music_stream=true;
}
