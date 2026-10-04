#pragma once

#include<cstdint>
#include<cstdio>

class AbstractOutStream;

class AbstractInStream
{
	FILE*TheFile=nullptr;
	char*Buffer=nullptr;
	char*TheTempToken=nullptr;
	char**TokenBuffer=nullptr;
	uint32_t TokenBufferSize=0;
	uint32_t TokenPosition=0;
	uint32_t TokenSize=0;
	uint32_t Temp=0;
	uint32_t BufferPosition=0;
	uint32_t BufferSize=0;
	bool DoubleQuotation=false;
	bool SingleQuotation=false;
	bool BackSlash=false;
	bool Blank=true;

	auto PreTreat()noexcept->bool;
public:
	AbstractInStream(FILE*TheFile)noexcept;
	~AbstractInStream();

	auto operator>>(char*&TheInput)noexcept->AbstractInStream&;
};

class AbstractOutStream
{
	friend struct EndLine;
	FILE*TheFile;
	char*Buffer=nullptr;
	uint32_t Position=0;
	auto End()noexcept->void;
public:
	struct Operation{};
	AbstractOutStream(FILE*TheFile)noexcept;
	~AbstractOutStream();

	auto operator<<(char const*TheOutput)noexcept->AbstractOutStream&;
	template<typename Operation>
	auto operator>>(Operation const&TheOperation)noexcept->AbstractOutStream&
	{
		TheOperation(*this);
		return *this;
	};
};

struct EndLine:AbstractOutStream::Operation
{
	auto operator()(AbstractOutStream&TheStream)const noexcept->void;
};

inline AbstractOutStream Out(stdout);
inline AbstractOutStream Error(stderr);
inline AbstractInStream In(stdin);