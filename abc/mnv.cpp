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
#pragma comment(lib,"avrocpp.lib")


using namespace std;

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
				f1.filename = dir_iter->path().filename().string();
				f1.filepath = dir_iter->path().parent_path().string();
				f1.filesize = fs::file_size(dir_iter->path());
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
	namespace fs = boost::filesystem;
	string g;
	cout<<"vvedite"<<endl;
	std::cin>>g;
	fs::path someDir(g);
	if ( fs::exists(someDir) && fs::is_directory(someDir))
	{
	std::auto_ptr<avro::OutputStream> out = avro::memoryOutputStream();
	ifstream ifs("file.json");
	avro::ValidSchema schema;
	avro::compileJsonSchema(ifs,schema);
	avro::DataFileWriter<c::file> dfw("test.bin",schema);
  recursiveSearch(someDir, dfw);
  dfw.close();

  fs::path fullpath(fs::current_path());

  cout<<"\nPath: "<<fullpath.string()<<"\\test.bin"<<endl;
	}
  int a;
  cin>>a;
  cin.get();
  return 0;
} 
