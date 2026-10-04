#include<cstdlib>
#include<cstring>

#include"AbstractStream.hpp"

AbstractInStream::AbstractInStream(FILE*TheFile)noexcept
{
	this->TheFile=TheFile;
	Buffer=new char[16384]();
	TokenBuffer=new char*[16384](nullptr);
	PreTreat();
}

AbstractInStream::~AbstractInStream()
{
	delete[] Buffer;
	for(auto Index=0;Index<16384;Index++)
		delete[] TokenBuffer[Index];
	delete[] TokenBuffer;
	delete[] TheTempToken;
	TheTempToken=nullptr;
}

auto AbstractInStream::PreTreat()noexcept->bool
{
	while(TokenBufferSize<16384)
	{
		if(BufferPosition>=BufferSize)
		{
			BufferSize=0;
			BufferPosition=0;
			while(BufferSize<16384)
			{
				auto const TempSize=fread(&Buffer[BufferSize],1,16384-BufferSize,TheFile);
				if(!TempSize)
				{
					if(ferror(TheFile))
					{
						perror("fread");
						exit(1);
					}
					break;
				}
				BufferSize+=TempSize;
			}
			if(BufferSize==0)
			{
				if(Temp)
				{
					if(TokenBufferSize<16384)
					{
						TokenBuffer[TokenBufferSize]=new char[Temp+1];
						memcpy(TokenBuffer[TokenBufferSize],TheTempToken,Temp);
						TokenBuffer[TokenBufferSize][Temp]='\0';
						TokenBufferSize++;
					}
					delete[] TheTempToken;
					TheTempToken=nullptr;
					TheTempToken=nullptr;
					Temp=0;
				}
				return true;
			}
		}
		for(;BufferPosition<BufferSize&&TokenBufferSize<16384;BufferPosition++)
			switch(Buffer[BufferPosition])
			{
				case' ':
				case'\t':
					if(SingleQuotation||DoubleQuotation)
						TokenSize++;
					else if(Blank)
						break;
					else
					{
						Blank=true;
						if(Temp)
						{
							TokenBuffer[TokenBufferSize]=new char[Temp+TokenSize+1];
							TokenBuffer[TokenBufferSize][Temp+TokenSize]='\0';
							memcpy(TokenBuffer[TokenBufferSize],TheTempToken,Temp);
							memcpy(&TokenBuffer[TokenBufferSize++][Temp],Buffer,TokenSize);
							TokenSize=0;
							delete[] TheTempToken;
							TheTempToken=nullptr;
							Temp=0;
							break;
						}
						TokenBuffer[TokenBufferSize]=new char[TokenSize+1];
						TokenBuffer[TokenBufferSize][TokenSize]='\0';
						memcpy(TokenBuffer[TokenBufferSize++],&Buffer[BufferPosition-TokenSize],TokenSize);
						TokenSize=0;
					}
					break;
				case'\r':
				case'\n':
					if(SingleQuotation||DoubleQuotation)
					{
						if(BackSlash)
							BackSlash=false;
						if(SingleQuotation)
							SingleQuotation=false;
						else
							DoubleQuotation=false;
						Blank=true;
						if(Temp)
						{
							TokenBuffer[TokenBufferSize]=new char[Temp+TokenSize+1];
							TokenBuffer[TokenBufferSize][Temp+TokenSize]='\0';
							memcpy(TokenBuffer[TokenBufferSize],TheTempToken,Temp);
							memcpy(&TokenBuffer[TokenBufferSize++][Temp],Buffer,TokenSize);
							TokenSize=0;
							delete[] TheTempToken;
							TheTempToken=nullptr;
							Temp=0;
							break;
						}
						TokenBuffer[TokenBufferSize]=new char[TokenSize+1];
						TokenBuffer[TokenBufferSize][TokenSize]='\0';
						memcpy(TokenBuffer[TokenBufferSize++],&Buffer[BufferPosition-TokenSize],TokenSize);
						TokenSize=0;
						break;
					}
					if(Blank)
						break;
					Blank=true;
					if(Temp)
					{
						TokenBuffer[TokenBufferSize]=new char[Temp+TokenSize+1];
						TokenBuffer[TokenBufferSize][Temp+TokenSize]='\0';
						memcpy(TokenBuffer[TokenBufferSize],TheTempToken,Temp);
						memcpy(&TokenBuffer[TokenBufferSize++][Temp],Buffer,TokenSize);
						TokenSize=0;
						delete[] TheTempToken;
						TheTempToken=nullptr;
						TheTempToken=nullptr;
						Temp=0;
						break;
					}
					TokenBuffer[TokenBufferSize]=new char[TokenSize+1];
					TokenBuffer[TokenBufferSize][TokenSize]='\0';
					memcpy(TokenBuffer[TokenBufferSize++],&Buffer[BufferPosition-TokenSize],TokenSize);
					TokenSize=0;
					break;
				case'\'':
					if(BackSlash)
					{
						TokenSize++;
						BackSlash=false;
					}
					else if(SingleQuotation)
					{
						SingleQuotation=false;
						Blank=true;
						if(Temp)
						{
							TokenBuffer[TokenBufferSize]=new char[Temp+TokenSize+1];
							TokenBuffer[TokenBufferSize][Temp+TokenSize]='\0';
							memcpy(TokenBuffer[TokenBufferSize],TheTempToken,Temp);
							memcpy(&TokenBuffer[TokenBufferSize++][Temp],Buffer,TokenSize+1);
							TokenSize=0;
							delete[] TheTempToken;
							TheTempToken=nullptr;
							Temp=0;
							break;
						}
						TokenBuffer[TokenBufferSize]=new char[TokenSize+1];
						TokenBuffer[TokenBufferSize][TokenSize]='\0';
						memcpy(TokenBuffer[TokenBufferSize++],&Buffer[BufferPosition-TokenSize],TokenSize+1);
						TokenSize=0;
					}
					else if(DoubleQuotation)
						TokenSize++;
					else if(Blank)
					{
						Blank=false;
						SingleQuotation=true;
						TokenSize++;
					}
					else
					{
						if(Temp)
						{
							TokenBuffer[TokenBufferSize]=new char[Temp+TokenSize+1];
							TokenBuffer[TokenBufferSize][Temp+TokenSize]='\0';
							memcpy(TokenBuffer[TokenBufferSize],TheTempToken,Temp);
							memcpy(&TokenBuffer[TokenBufferSize++][Temp],Buffer,TokenSize);
							TokenSize=1;
							delete[] TheTempToken;
							TheTempToken=nullptr;
							Temp=0;
							SingleQuotation=true;
							break;
						}
						TokenBuffer[TokenBufferSize]=new char[TokenSize+1];
						TokenBuffer[TokenBufferSize][TokenSize]='\0';
						memcpy(TokenBuffer[TokenBufferSize++],&Buffer[BufferPosition-TokenSize],TokenSize);
						TokenSize=1;
						SingleQuotation=true;
					}
					break;
				case'\"':
					if(BackSlash)
					{
						TokenSize++;
						BackSlash=false;
					}
					else if(DoubleQuotation)
					{
						DoubleQuotation=false;
						Blank=true;
						if(Temp)
						{
							TokenBuffer[TokenBufferSize]=new char[Temp+TokenSize+1];
							TokenBuffer[TokenBufferSize][Temp+TokenSize]='\0';
							memcpy(TokenBuffer[TokenBufferSize],TheTempToken,Temp);
							memcpy(&TokenBuffer[TokenBufferSize++][Temp],Buffer,TokenSize+1);
							TokenSize=0;
							delete[] TheTempToken;
							TheTempToken=nullptr;
							Temp=0;
							break;
						}
						TokenBuffer[TokenBufferSize]=new char[TokenSize+1];
						TokenBuffer[TokenBufferSize][TokenSize]='\0';
						memcpy(TokenBuffer[TokenBufferSize++],&Buffer[BufferPosition-TokenSize],TokenSize+1);
						TokenSize=0;
					}
					else if(SingleQuotation)
						TokenSize++;
					else if(Blank)
					{
						Blank=false;
						DoubleQuotation=true;
						TokenSize++;
					}
					else
					{
						if(Temp)
						{
							TokenBuffer[TokenBufferSize]=new char[Temp+TokenSize+1];
							TokenBuffer[TokenBufferSize][Temp+TokenSize]='\0';
							memcpy(TokenBuffer[TokenBufferSize],TheTempToken,Temp);
							memcpy(&TokenBuffer[TokenBufferSize++][Temp],Buffer,TokenSize);
							TokenSize=1;
							delete[] TheTempToken;
							TheTempToken=nullptr;
							Temp=0;
							DoubleQuotation=true;
							break;
						}
						TokenBuffer[TokenBufferSize]=new char[TokenSize+1];
						TokenBuffer[TokenBufferSize][TokenSize]='\0';
						memcpy(TokenBuffer[TokenBufferSize++],&Buffer[BufferPosition-TokenSize],TokenSize);
						TokenSize=1;
						DoubleQuotation=true;
					}
					break;
				case'\\':
					if(!SingleQuotation&&!DoubleQuotation)
					{
						if(Temp)
						{
							TokenBuffer[TokenBufferSize]=new char[Temp+TokenSize+1];
							TokenBuffer[TokenBufferSize][Temp+TokenSize]='\0';
							memcpy(TokenBuffer[TokenBufferSize],TheTempToken,Temp);
							memcpy(&TokenBuffer[TokenBufferSize++][Temp],Buffer,TokenSize);
							TokenSize=0;
							delete[] TheTempToken;
							TheTempToken=nullptr;
							Temp=0;
							if(TokenBufferSize==16384)
								return true;
							TokenBuffer[TokenBufferSize]=new char[2];
							TokenBuffer[TokenBufferSize][0]='\\';
							TokenBuffer[TokenBufferSize++][1]='\0';
							break;
						}
						if(TokenSize)
						{
							Blank=true;
							TokenBuffer[TokenBufferSize]=new char[TokenSize+1];
							TokenBuffer[TokenBufferSize][TokenSize]='\0';
							memcpy(TokenBuffer[TokenBufferSize++],&Buffer[BufferPosition-TokenSize],TokenSize);
							TokenSize=0;
						}
						if(TokenBufferSize==16384)
							return true;
						TokenBuffer[TokenBufferSize]=new char[2];
						TokenBuffer[TokenBufferSize][0]='\\';
						TokenBuffer[TokenBufferSize++][1]='\0';
						break;
					}
					if(BackSlash)
					{
						TokenSize++;
						BackSlash=false;
						break;
					}
					TokenSize++;
					BackSlash=true;
					break;
				case'!':
				case'#'...'&':
				case'('...'/':
				case':'...'@':
				case'[':
				case']'...'^':
				case'`':
				case'{'...'~':
					if(!SingleQuotation&&!DoubleQuotation)
					{
						if(Temp)
						{
							TokenBuffer[TokenBufferSize]=new char[Temp+TokenSize+1];
							TokenBuffer[TokenBufferSize][Temp+TokenSize]='\0';
							memcpy(TokenBuffer[TokenBufferSize],TheTempToken,Temp);
							memcpy(&TokenBuffer[TokenBufferSize++][Temp],Buffer,TokenSize);
							TokenSize=0;
							delete[] TheTempToken;
							TheTempToken=nullptr;
							Temp=0;
							if(TokenBufferSize==16384)
								return true;
							TokenBuffer[TokenBufferSize]=new char[2];
							TokenBuffer[TokenBufferSize][0]=Buffer[BufferPosition];
							TokenBuffer[TokenBufferSize++][1]='\0';
							break;
						}
						if(TokenSize)
						{
							Blank=true;
							TokenBuffer[TokenBufferSize]=new char[TokenSize+1];
							TokenBuffer[TokenBufferSize][TokenSize]='\0';
							memcpy(TokenBuffer[TokenBufferSize++],&Buffer[BufferPosition-TokenSize],TokenSize);
							TokenSize=0;
						}
						if(TokenBufferSize==16384)
							return true;
						TokenBuffer[TokenBufferSize]=new char[2];
						TokenBuffer[TokenBufferSize][0]=Buffer[BufferPosition];
						TokenBuffer[TokenBufferSize++][1]='\0';
						break;
					}
					if(BackSlash)
						BackSlash=false;
					TokenSize++;
					break;
				default:
					if(Blank)
						Blank=false;
					if(BackSlash)
						BackSlash=false;
					TokenSize++;
					break;
			}
		if(TokenSize)
		{
			if(Temp)
			{
				auto NewToken=new char[Temp+TokenSize+1];
				memcpy(NewToken,TheTempToken,Temp);
				memcpy(NewToken+Temp,&Buffer[BufferPosition-TokenSize],TokenSize);
				NewToken[Temp+TokenSize]='\0';
				delete[] TheTempToken;
				TheTempToken=nullptr;
				TheTempToken=NewToken;
				Temp+=TokenSize;
			}
			else
			{
				delete[] TheTempToken;
				TheTempToken=nullptr;
				TheTempToken=new char[TokenSize+1];
				TheTempToken[TokenSize]='\0';
				memcpy(TheTempToken,&Buffer[BufferPosition-TokenSize],TokenSize);
				Temp=TokenSize;
			}
			TokenSize=0;
		}
	}
	return true;
}

auto AbstractInStream::operator>>(char*&TheInput)noexcept->AbstractInStream&
{
	if(TokenPosition<TokenBufferSize)
		TheInput=TokenBuffer[TokenPosition++];
	else if(TokenPosition==TokenBufferSize)
	{
		if(TokenBufferSize==16384)
		{
			TheInput=nullptr;
			for(auto Index1=0;Index1<16384;Index1++)
				delete[]TokenBuffer[Index1];
			TokenBufferSize=0;
			PreTreat();
			TokenPosition=0;
			if(TokenBufferSize>0)
				TheInput=TokenBuffer[TokenPosition++];
			else
				TheInput=nullptr;
		}
		else
			TheInput=nullptr;
	}
	return *this;
}


auto AbstractOutStream::End() noexcept->void
{
	*this<<"\n";
	fwrite(Buffer,1,Position,TheFile);
	Position=0;
}

AbstractOutStream::AbstractOutStream(FILE*TheFile)noexcept
{
	this->TheFile=TheFile;
	Buffer=new char[16384]();
};

AbstractOutStream::~AbstractOutStream()
{
	if(Position)
		fwrite(Buffer,1,Position,TheFile);
	fflush(TheFile);
	delete[] Buffer;
};

auto AbstractOutStream::operator<<(char const*TheOutput)noexcept->AbstractOutStream&
{
	auto Index=0;
	while(TheOutput[Index]!='\0')
		Index++;
Treat:
	if(Position+Index<16384)
	{
		memcpy(&Buffer[Position],TheOutput,Index);
		Position+=Index;
	}
	else
	{
		fwrite(Buffer,1,Position,TheFile);
		Position=0;
		goto Treat;
	}
	return *this;
}

auto EndLine::operator()(AbstractOutStream&TheStream)const noexcept->void { TheStream.End(); }