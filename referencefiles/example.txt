/* this example is freeware. do whatever, just don't hurt yourself, okay? */


#include <iostream>
#include "BLUnZip.h"
#include "BLZip.h"

int main( int argc, char ** argv )
{
	std::cout << "Hello" << std::endl;

	BLUnZip * zipfile = new BLUnZip( "test.zip" );

	zipfile->List();
	
	std::cout << "README.md is at " << zipfile->IndexOfItem( "README.md" ) << std::endl;
	std::cout << "src/example.cpp is at " << zipfile->IndexOfItem( "src/example.cpp" ) << std::endl;
	std::cout << "dne.txt is at " << zipfile->IndexOfItem( "dne.txt" ) << std::endl;

	std::string readme( zipfile->ExtractToString( "README.md" ));

	std::cout << "-- Readme.txt ------------------" << std::endl;
	std::cout << readme << std::endl;
	std::cout << "--------------------------------" << std::endl;

	delete zipfile;

/* untested 
	BLZip * zf = new BLZip( "test.zip" );

	zf->Add( "fs/readme.txt", "read.txt" );
	zf->Add( "fs/readme.txt" );

	delete zf;
*/

	return 0;
}
