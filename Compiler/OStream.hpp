#pragma once

#include <cstdint>
#include <cstring>
#include <format>

namespace Callable
{
	struct End;
};

namespace Library
{
	enum ColourLevel:uint8_t
	{
		None,
		Basic,
		Complex,
		True
	};
	enum BasicColour
	{
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		White
	};
	class OStream
	{
	public:
		static char const*Value[8];
	private:
		friend struct Callable::End;
		char Buffer[4096];
		ColourLevel TheColour;

		void End();
	public:
		OStream();
		~OStream();

		ColourLevel &Colour();

		template<uint8_t Red,uint8_t Green,uint8_t Blue>
		OStream &True()
		{
			static const auto Colour=std::format("\x1b[38;2;{};{};{}m",Red,Green,Blue);
			static const auto Colour1=std::format(" TrueRGB({},{},{}): ",Red,Green,Blue);
			if(TheColour==ColourLevel::True)
				strcat(Buffer,Colour.c_str());
			else
				strcat(Buffer,Colour1.c_str());
			return *this;
		};
		template<uint8_t Red,uint8_t Green,uint8_t Blue>
		OStream &Complex()
		{
			static_assert(Red<=5&&Green<=5&&Blue<=5,"RGB must be 0..5");
			static const auto Colour=std::format("\x1b[38;5;{}m",16+Red*36+Green*6+Blue);
			static const auto Colour1=std::format(" ComplexRGB({},{},{}): ",Red,Green,Blue);
			if(TheColour>=ColourLevel::Complex)
				strcat(Buffer,Colour.c_str());
			else
				strcat(Buffer,Colour1.c_str());
			return *this;
		};
		template<uint8_t Depth>
		OStream &Gray()
		{
			static_assert(Depth<=23,"Depth must be 0..23");
			static const auto Colour=std::format("\x1b[38;5;{}m",232+Depth);
			static const auto Colour1=std::format(" Gray({}): ",Depth);
			if(TheColour>=ColourLevel::Complex)
				strcat(Buffer,Colour.c_str());
			else
				strcat(Buffer,Colour1.c_str());
			return *this;
		};
		template<BasicColour Value>
		OStream &Plain()
		{
			static_assert(Value<=White,"The Value must be 0..7");
			static const auto Colour=std::format("\x1b[{}m",30+static_cast<int>(Value));
			static const auto Colour1=std::format(" {}: ",OStream::Value[static_cast<int>(Value)]);
			if(TheColour>=Basic)
				strcat(Buffer,Colour.c_str());
			else
				strcat(Buffer,Colour1.c_str());
			return *this;
		};
		template<BasicColour Value>
		OStream &BrightPlain()
		{
			static_assert(Value<=White,"The Value must be 0..7");
			static const auto Colour=std::format("\x1b[{}m",90+static_cast<int>(Value));
			static const auto Colour1=std::format(" Bright{}: ",OStream::Value[static_cast<int>(Value)]);
			if(TheColour>=Basic)
				strcat(Buffer,Colour.c_str());
			else
				strcat(Buffer,Colour1.c_str());
			return *this;
		};

		template<uint8_t Red,uint8_t Green,uint8_t Blue>
		OStream &BackTrue()
		{
			static const auto Colour=std::format("\x1b[48;2;{};{};{}m",Red,Green,Blue);
			static const auto Colour1=std::format(" BackTrueRGB({},{},{}): ",Red,Green,Blue);
			if(TheColour==ColourLevel::True)
				strcat(Buffer,Colour.c_str());
			else
				strcat(Buffer,Colour1.c_str());
			return *this;
		};
		template<uint8_t Red,uint8_t Green,uint8_t Blue>
		OStream &BackComplex()
		{
			static_assert(Red<=5&&Green<=5&&Blue<=5,"RGB must be 0..5");
			static const auto Colour=std::format("\x1b[48;5;{}m",16+Red*36+Green*6+Blue);
			static const auto Colour1=std::format(" BackComplexRGB({},{},{}): ",Red,Green,Blue);
			if(TheColour>=ColourLevel::Complex)
				strcat(Buffer,Colour.c_str());
			else
				strcat(Buffer,Colour1.c_str());
			return *this;
		};
		template<uint8_t Depth>
		OStream &BackGray()
		{
			static_assert(Depth<=23,"Depth must be 0..23");
			static const auto Colour=std::format("\x1b[48;5;{}m",232+Depth);
			static const auto Colour1=std::format(" BackGray({}): ",Depth);
			if(TheColour>=ColourLevel::Complex)
				strcat(Buffer,Colour.c_str());
			else
				strcat(Buffer,Colour1.c_str());
			return *this;
		};
		template<BasicColour Value>
		OStream &BackPlain()
		{
			static_assert(Value<=White,"The Value must be 0..7");
			static const auto Colour=std::format("\x1b[{}m",40+static_cast<int>(Value));
			static const auto Colour1=std::format(" Back{}: ",OStream::Value[static_cast<int>(Value)]);
			if(TheColour>=Basic)
				strcat(Buffer,Colour.c_str());
			else
				strcat(Buffer,Colour1.c_str());
			return *this;
		};
		template<BasicColour Value>
		OStream &BackBrightPlain()
		{
			static_assert(Value<=White,"The Value must be 0..7");
			static const auto Colour=std::format("\x1b[{}m",100+static_cast<int>(Value));
			static const auto Colour1=std::format(" BackBright{}: ",OStream::Value[static_cast<int>(Value)]);
			if(TheColour>=Basic)
				strcat(Buffer,Colour.c_str());
			else
				strcat(Buffer,Colour1.c_str());
			return *this;
		};

		OStream &operator<<(char const*TheString);
		template<typename Callable>
		OStream &operator>>(Callable &&TheCallable)
		{
			TheCallable(this);
			return *this;
		};

		static ColourLevel CheckColour();
	};


	extern OStream Stream;
};

namespace Callable
{
	struct End
	{
		void operator()(Library::OStream *TheStream)const;
	};
	struct RedWhite
	{
		void operator()(Library::OStream *TheStream)const;
	};
	struct EndLine
	{
		void operator()(Library::OStream *TheStream)const;
	};
};