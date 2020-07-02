//Code by Rogier Walraven. Do not distribute.

#include <stdio.h>

#include <SDL.h>

#include "system.h"

void SYSTEM::init()
{
    SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);
}

void SYSTEM::quit()
{
    SDL_Quit();
}

double SYSTEM::seconds_since_startup()
{
	return((double)SDL_GetTicks()/1000);
}

SYSTEM::RFILE::RFILE(const char *filename)
{
	FILE *file;

	if (!fopen_s(&file,filename,"rb"))
	{
		open=true;
	}
	else
	{
		open=false;
	}

	stream=(void*)file;
}

SYSTEM::RFILE::~RFILE()
{
	if (open)
	{
		fclose((FILE*)stream);
	}
}

bool SYSTEM::RFILE::isopen() const
{
	return(open);
}

SYSTEM::BYTE  SYSTEM::RFILE::readbyte() const
{
	BYTE byte;

	fread(&byte,1,1,(FILE*)stream);

	return(byte);
}

SYSTEM::WORD  SYSTEM::RFILE::readword() const
{
	WORD word;

	fread(&word,2,1,(FILE*)stream);

	return(word);
}

SYSTEM::DWORD SYSTEM::RFILE::readdword() const
{
	DWORD dword;

	fread(&dword,4,1,(FILE*)stream);

	return(dword);
}

SYSTEM::QWORD SYSTEM::RFILE::readqword() const
{
	QWORD qword;

	fread(&qword,8,1,(FILE*)stream);

	return(qword);
}

SYSTEM::WFILE::WFILE(const char *filename)
{
	FILE *file;

	if (!fopen_s(&file,filename,"wb"))
	{
		open=true;
	}
	else
	{
		open=false;
	}

	stream=(void*)file;
}

SYSTEM::WFILE::~WFILE()
{
	if (open)
	{
		fclose((FILE*)stream);
	}
}

bool SYSTEM::WFILE::isopen() const
{
	return(open);
}

SYSTEM::WFILE& SYSTEM::WFILE::writebyte(BYTE byte)
{
	fwrite(&byte,1,1,(FILE*)stream);

	return(*this);
}

SYSTEM::WFILE& SYSTEM::WFILE::writeword(WORD word)
{
	fwrite(&word,2,1,(FILE*)stream);

	return(*this);
}

SYSTEM::WFILE& SYSTEM::WFILE::writedword(DWORD dword)
{
	fwrite(&dword,4,1,(FILE*)stream);

	return(*this);
}

SYSTEM::WFILE& SYSTEM::WFILE::writeqword(QWORD qword)
{
	fwrite(&qword,8,1,(FILE*)stream);

	return(*this);
}
