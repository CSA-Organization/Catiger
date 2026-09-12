#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#define isatty _isatty
#define STDOUT_FILENO _fileno(stdout)
#else
	#include <unistd.h>
#endif

#include "OStream.hpp"

using namespace Library;
using namespace Callable;

char const*OStream::Value[8]=
{
	"Black",
	"Red",
	"Green",
	"Yellow",
	"Blue",
	"Magenta",
	"Cyan",
	"White"
};

ColourLevel OStream::CheckColour()
{
	#ifdef _WIN32
	HANDLE HOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (HOut == INVALID_HANDLE_VALUE)
		return None;
	DWORD Mode = 0;
	if (!GetConsoleMode(HOut, &Mode))
		return None;
	if (!(Mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING))
	{
		DWORD NewMode = Mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(HOut, NewMode))
			return None;
	}
	#endif
	char const*const TheValue=std::getenv("NO_COLOR");
	if(TheValue!=nullptr&&TheValue[0]!='\0')
		return None;
	char const*const ClientColourForce=std::getenv("CLICOLOR_FORCE");
	if(ClientColourForce&&std::strcmp(ClientColourForce,"0")!=0)
		return ColourLevel::True;
	if(isatty(STDOUT_FILENO)!=0)
	{
		if(char const*const ClientColour=std::getenv("CLICOLOR");ClientColour&&!std::strcmp(ClientColour,"0"))
			return None;
		if(char const*const ColourTerminal=std::getenv("COLORTERM"))
			if(std::strcmp(ColourTerminal,"truecolor")==0||std::strcmp(ColourTerminal,"24bit")==0)
				return ColourLevel::True;
		char const*const Terminal=std::getenv("TERM");
		if(!Terminal)
			return None;
		if(
			std::strcmp(Terminal, "dumb")==0  ||
			std::strcmp(Terminal,"unknown")==0
		  )
			return None;
		if(std::strstr(Terminal,"256color")!=nullptr)
			return ColourLevel::Complex;
		if(
			std::strstr(Terminal,"color")!=nullptr ||
			std::strstr(Terminal,"xterm")!=nullptr ||
			std::strstr(Terminal,"screen")!=nullptr||
			std::strstr(Terminal,"tmux")!=nullptr  ||
			std::strstr(Terminal,"vt100")!=nullptr ||
			std::strstr(Terminal,"linux")!=nullptr
		  )
			return Basic;
		return None;
	}
	return None;
};

void OStream::End()
{
	strcat(Buffer,"\033[0m");
	#ifdef _WIN32
	_write(STDOUT_FILENO,Buffer,strlen(Buffer));
	#else
	write(STDOUT_FILENO,Buffer,strlen(Buffer));
	#endif
	memset(&Buffer,0,4096);
	Buffer[0]='\0';
};

OStream::OStream()
{
	TheColour=CheckColour();
	memset(&Buffer,0,4096);
	Buffer[0]='\0';
};

OStream::~OStream()
{
	End();
};

ColourLevel &OStream::Colour(){return TheColour;};

OStream &OStream::operator<<(char const*TheString)
{
	strcat(Buffer,TheString);
	return *this;
};

void End::operator()(OStream *TheStream) const
{
	TheStream->End();
};

void RedWhite::operator()(OStream *TheStream) const
{
	switch(TheStream->Colour())
	{
	case None:
	case Basic:
		TheStream->BackBrightPlain<Red>().BrightPlain<White>();
		break;
	case Complex:
		TheStream->BackComplex<5,0,0>().Complex<5,5,5>();
		break;
	case True:
		TheStream->BackTrue<255,0,0>().True<255,255,255>();
	};
};

void EndLine::operator()(OStream *TheStream) const
{
	*TheStream>>End{}<<"\n">>End{};
};

OStream Library::Stream;