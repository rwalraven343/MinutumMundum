TARGETFILE=RobotFighter.exe
INSTALLDIR=Binaries

COMPILERCOMMAND=cl
LINKERCOMMAND=link

COMPILEROPTIONS=/c /MD /O2
LINKEROPTIONS=

INCLUDEDIRS=
LIBRARYDIRS=/LIBPATH:"..\GameEngine\GameLib" /LIBPATH:"..\GameEngine" /LIBPATH:"..\GameEngine\GameLib\ThirdParty\SDL-1.2.14\lib" /LIBPATH:"..\GameEngine\GameLib\ThirdParty\OpenAL 1.1 SDK\libs\Win32" /LIBPATH:"..\GameEngine\GameLib\ThirdParty\freealut-1.1.0-bin\lib" /LIBPATH:"..\GameEngine\GameLib\ThirdParty\vorbis-sdk-1.0rc2\lib"

SOURCEFILES=globals.cpp main.cpp RobotFighter_mainloop.cpp RobotFighter_mainloop_level.cpp
OBJECTFILES=$(SOURCEFILES:.cpp=.obj)
LIBRARYFILES="GameLib.lib" "GameEngine.lib" "SDL.lib" "OpenGL32.lib" "OpenAL32.lib" "alut.lib" "ogg_static.lib" "vorbis_static.lib" "vorbisfile_static.lib" "legacy_stdio_definitions.lib"

all: $(TARGETFILE)

$(TARGETFILE): $(OBJECTFILES)
	$(LINKERCOMMAND) $(LINKEROPTIONS) $(LIBRARYDIRS) $(OBJECTFILES) $(LIBRARYFILES) /OUT:$(TARGETFILE)

$(OBJECTFILES):
	$(COMPILERCOMMAND) $(COMPILEROPTIONS) $(INCLUDEDIRS) $(SOURCEFILES)

install: $(TARGETFILE)
	mkdir $(INSTALLDIR)
	copy "..\GameEngine\GameLib\ThirdParty\SDL-1.2.14\lib\SDL.dll" $(INSTALLDIR)
	copy "..\GameEngine\GameLib\ThirdParty\OpenAL 1.1 SDK\libs\Win32\OpenAL32.dll" $(INSTALLDIR)
	copy "..\GameEngine\GameLib\ThirdParty\OpenAL 1.1 SDK\libs\Win32\wrap_oal.dll" $(INSTALLDIR)
	copy "..\GameEngine\GameLib\ThirdParty\freealut-1.1.0-bin\lib\alut.dll" $(INSTALLDIR)
	xcopy ..\Data $(INSTALLDIR)\Data\* /e
	copy $(TARGETFILE) $(INSTALLDIR)

clean:
	del $(TARGETFILE)
	del *.obj

uninstall:
	rmdir $(INSTALLDIR) /s /q
