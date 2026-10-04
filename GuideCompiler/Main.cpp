#include<iostream>
#include<fstream>

#include"AbstractStream.hpp"

using namespace std;

auto main(int Count,char*Arguments[])->int
{
	if(Count==1)
	{
		cerr<<"Too little arguments!"<<endl;
		return 1;
	};
	auto const TheFile=fopen(Arguments[1],"rb");
	if(!TheFile)
	{
		perror("fopen");
		return 1;
	}
	AbstractInStream TheStream(TheFile);
	char*TheToken=nullptr;
	TheStream>>TheToken;
	while(TheToken)
	{
		cout<<TheToken<<endl;
		TheStream>>TheToken;
	}
	fclose(TheFile);
	return 0;
};