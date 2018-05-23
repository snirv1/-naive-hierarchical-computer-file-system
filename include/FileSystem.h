#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"


class FileSystem {
private:
	Directory* rootDirectory;
	Directory* workingDirectory;
	void copy(Directory& otherRoot, Directory& otherWdir);
	void clear();
public:
	FileSystem();//constructor
	FileSystem(const FileSystem&);//copy constructor
	FileSystem& operator=(const FileSystem &other);//Copy Assignment operator
	FileSystem(FileSystem &&other);//move constructor
	FileSystem& operator=(FileSystem &&other);// Move Assignment
	virtual ~FileSystem();//Destructor
	Directory& getRootDirectory() const; // Return reference to the root directory
	Directory& getWorkingDirectory() const; // Return reference to the working directory
	void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system



};



#endif
