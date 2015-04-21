#include<fstream>
#include <windows.h>
#include <boost/filesystem.hpp>
#include <tchar.h>
#include <iostream>
#include <ctime>
#include <Encoder.hh>
#include <Compiler.hh>
#include <Specific.hh>
#include <ValidSchema.hh>
#include "file.h"
#include <fstream>
#include <DataFile.hh>
#include <Decoder.hh>
#include "sha1.h"
#include <string.h>

#pragma comment(lib,"avrocpp.lib")


using namespace std;
void firstTask();
void secondTask();
void proverka(boost::filesystem::path path ,vector<c::file> A, bool * checkA);
string hash_sha1(const char * hash_str, int length){ 
	unsigned char * hash = new unsigned char[length];
	char * hexstring = new char [41]; // Строка хэш-суммы всегда одной длины
	sha1::calc(hash_str, length, hash);
	sha1::toHexString(hash, hexstring);

	return hexstring;
	delete[] hash;
	delete[] hexstring;
  }


void recursiveSearch(boost::filesystem::path path, avro::DataFileWriter<c::file>& fw)
{
	namespace fs = boost::filesystem;
	fs::directory_iterator end_iter;
	if ( fs::exists(path) && fs::is_directory(path))
	{
		for( fs::directory_iterator dir_iter(path) ; dir_iter != end_iter ; ++dir_iter)
		{
			if (fs::is_regular_file(dir_iter->status()) )
			{	
				c::file f1;
				string content;
				f1.filename = dir_iter->path().filename().string();
				f1.filepath = dir_iter->path().parent_path().string();
				f1.filesize = fs::file_size(dir_iter->path());
				f1.hash=(hash_sha1(content.c_str(), content.length()));				
				
				fw.write(f1);
				
			}
			else if (fs::is_directory(dir_iter->status()))
			{
				recursiveSearch(dir_iter->path(), fw);
			}
		}
	}
}

int main() 
{ 
	
  int a;

  cout<<"1 for first task, 2 for second task"<<endl;
  cin>>a;
  if (a == 1)
  {
	  firstTask();
  }
  else if (a == 2)
  {
	  secondTask();
  }
  else
  {
	  
  }

  cin>>a;
  cin.get();
  return 0;
} 

void firstTask()
{
	namespace fs = boost::filesystem;
	string g;
	cout<<"vvedite"<<endl;
	std::cin>>g;
	fs::path someDir(g);
	if ( fs::exists(someDir) && fs::is_directory(someDir))
	{

	
	ifstream ifs("file.json");
	avro::ValidSchema schema;
	avro::compileJsonSchema(ifs,schema);
	avro::DataFileWriter<c::file> dfw("test.bin",schema);
	
  recursiveSearch(someDir, dfw);

  fs::path fullpath(fs::current_path());
  dfw.close();
  cout<<"\nPath: "<<fullpath.string()<<"\\test.bin"<<endl;
	}
	else
	{
		cout<<"net takoy directorii"<<endl;
	}
}

void secondTask()
{
    
	ifstream ifs("file.json");
	avro::ValidSchema schema;
	avro::compileJsonSchema(ifs,schema);
    avro::DataFileReader<c::file> dfr("test.bin", schema);
	vector<c::file> A;
	c::file f2;
	while (dfr.read(f2)) {
		A.push_back(f2);
		}
	cout<<A.size()<<endl;


	bool* checkA=new bool[A.size()];
	int i;
	for (i=0; i< A.size(); i++)
		{
			checkA[i]=false;
		}
	namespace fs = boost::filesystem;
	string g;
	cout<<"vvedite name"<<endl;
	std::cin>>g;
	fs::path someDir(g);
	proverka(someDir,A,checkA);

		int a;
	cin>>a;
}

void recursive(boost::filesystem::path path,vector<c::file> * B)
{
	namespace fs = boost::filesystem;
	fs::directory_iterator end_iter;
	if ( fs::exists(path) && fs::is_directory(path))
	{
		for( fs::directory_iterator dir_iter(path) ; dir_iter != end_iter ; ++dir_iter)
		{
			if (fs::is_regular_file(dir_iter->status()) )
			{	
				c::file f1;
				string content;

				f1.filename = dir_iter->path().filename().string();
				f1.filepath = dir_iter->path().parent_path().string();
				f1.filesize = fs::file_size(dir_iter->path());
				f1.hash=(hash_sha1(content.c_str(), content.length()));				
				
				 B->push_back(f1);
				
			}
			else if (fs::is_directory(dir_iter->status()))
			{
				recursive(dir_iter->path(), B);
			}
		}
	}
}
void proverka(boost::filesystem::path path ,vector<c::file> A, bool * checkA){
	vector <c::file> papka;
	recursive(path,&papka);

	for(int i=0;i<papka.size();i++){
		bool x=false;
		c::file f0=papka[i];
		for(int j=0;j<A.size();j++){
			c::file f1=A[j];
	
			if (f0.filename.compare(f1.filename) == 0 && f0.filepath.compare(f1.filepath)){
				x=true;
				checkA[i]=true;
				if (f0.hash.compare(f1.hash) !=0){
				cout<<f0.filename<<" izmenen"<<endl;
				}
				else
				{
					cout<<f0.filename<<" ne izmenen"<<endl;
				}
			}
		}

		if (!x)
		{
			cout<<f0.filename<<" dobavlen"<<endl;
		}
	}

	for(int j=0; j<A.size(); j++){
	  if (checkA[j]==false) {
		 cout<<A[j].filename<<" deleted"<<endl;
		}
	}
}