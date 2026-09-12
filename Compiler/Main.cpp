#include <cstring>

#include <vector>
#include <string>

#include <filesystem>
#include <fstream>

#include "OStream.hpp"

using namespace std;
namespace FileSystem=filesystem;

using namespace Library;
using namespace Callable;

static char const *Errors[8]=
{
	"Error: File not found",
	"Unsupported: Is a directory",
	"Unsupported: Block device",
	"Unsupported: Character device",
	"Unsupported: Named pipe (FIFO)",
	"Unsupported: Socket",
	"Unsupported: Unknown file type",
	"Unsupported: Invalid file status"
};

int main(int Count,char*Arguments[])
{
	std::ios::sync_with_stdio(false);
	if(Count==1)
	{
		Stream>>RedWhite{}<<"No Argument. Try to use help.">>EndLine{};
		return 1;
	};
	if(strcmp(Arguments[1],"Compile")==0)
	{
		int Index;
		error_code Error;
		FileSystem::file_status Status;
		ifstream FileStream;
		vector<char>Buffer{0x00,0x00,0x00};
		vector<string>Tokens{};
		string Temp("");
		size_t BytesRead;
		FileSystem::path Path;
		switch(Count)
		{
		case 3:
			Status=FileSystem::status(Arguments[2],Error);
			if(Error)
			{
				Stream>>RedWhite{}<<"FileSystemError: "<<Error.message().c_str()>>EndLine{};
				return 1;
			}
			switch(Status.type())
			{
			case FileSystem::file_type::regular:
				FileStream=ifstream(Arguments[2],ios::binary);
				if(!FileStream.is_open())
				{
					Stream>>RedWhite{}<<"Unsupported: Be denied">>EndLine{};
					return 1;
				};
				FileStream.read(Buffer.data(),3);
				BytesRead=FileStream.gcount();
				if(BytesRead==0)
				{
					Stream>>RedWhite{}<<"Failed to load file: "<<Arguments[2]>>EndLine{};
					return 1;
				};
				if(BytesRead<3)
				{
					Stream>>RedWhite{}<<"Not a valid file: "<<Arguments[2]>>EndLine{};
					return 1;
				};
				if(strcmp(Buffer.data(),"\xEF\xBB\xBF")!=0)
				{
					Stream>>RedWhite{}<<"Not a valid file"<<Arguments[2]>>EndLine{};
					return 1;
				};
				FileSystem::current_path(Path.parent_path());
				FileStream.read(Buffer.data(),4096);
				BytesRead=FileStream.gcount();
				if(BytesRead==4096)
				{
					for(int Index=0,Before=0,Whether=0;Index<=BytesRead;Index++)
					{
						if(Buffer[Index]==' '||Buffer[Index]=='\n'||Buffer[Index]=='\t')
						{
							if(Whether==0)
							{
								if(Before==0)
								{
									Temp+=&Buffer[0];
									Buffer[Index]='\0';
									Tokens.emplace_back(Temp);
									Temp="";
									Whether++;
								}
								else
								{
									Buffer[Index]='\0';
									Tokens.emplace_back(&Buffer[Before]);
									Whether++;
								};
							};
						}
						else if(Index==BytesRead)
						{
							if(Whether>0)
								break;
							Temp+=string(&Buffer[Before],Index-Before);
						}
						else if(Whether!=0)
						{
							Before=Index;
							Whether=0;
						};
					};
				}
				else
				{

				};
				return 0;
			case FileSystem::file_type::not_found:
				Index=0;
				break;
			case FileSystem::file_type::directory:
				Index=1;
				break;
			case FileSystem::file_type::block:
				Index=2;
				break;
			case FileSystem::file_type::character:
				Index=3;
				break;
			case FileSystem::file_type::fifo:
				Index=4;
				break;
			case FileSystem::file_type::socket:
				Index=5;
				break;
			case FileSystem::file_type::unknown:
				Index=6;
				break;
			case FileSystem::file_type::none:
			default:
				Index=7;
			};
			Stream>>RedWhite{}<<Errors[Index]<<":">>EndLine{}>>RedWhite{}<<Arguments[2]>>EndLine{};
			return 1;
		case 2:
			Stream>>RedWhite{}<<"Arguments is too little: "<<Arguments[1]<<" {What}">>EndLine{};
			return 1;
		default:
			Stream>>RedWhite{}<<"Arguments is too many: ";
			for(Index=1;Index<Count-1;Index++)
			{
				Stream<<Arguments[Index];
				Stream<<" ";
			};
			Stream<<Arguments[Index];
			Stream>>EndLine{};
			return 1;
		};
	};
	return 0;
};