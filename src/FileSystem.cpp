#include <iostream>
#include "../include/FileSystem.h"
#include "../include/Commands.h"//TODO
using namespace std;

//constructor//
FileSystem::FileSystem():rootDirectory( new Directory("/", nullptr)),workingDirectory(rootDirectory){//TODO

}

//copy constructor
FileSystem::FileSystem(const FileSystem & other):rootDirectory(other.rootDirectory), workingDirectory(rootDirectory) {
    if(verbose == 1 || verbose == 3){
        cout <<"FileSystem::FileSystem(const FileSystem & other)"<< endl;
    }
    copy(other.getRootDirectory(),other.getWorkingDirectory());
}
//destructor//TODO
FileSystem::~FileSystem() {
    clear();
    if(verbose == 1 || verbose == 3){
        cout <<"FileSystem::~FileSystem()"<< endl;
    }
}

//Copy Assignment operator
FileSystem& FileSystem::operator=(const FileSystem &other) {//TODO && check
    if(verbose == 1 || verbose == 3){
        cout <<"FileSystem& FileSystem::operator=(const FileSystem &other)"<< endl;
    }
    if (this != &other) {
        clear();
        rootDirectory = new Directory(*other.rootDirectory);
        workingDirectory = new Directory(*rootDirectory);
        copy(other.getRootDirectory(), other.getWorkingDirectory());

    }  return *this;
}//TODO

//move constructor//TODO

FileSystem::FileSystem(FileSystem &&other):rootDirectory(other.rootDirectory),workingDirectory(other.workingDirectory) {
    if(verbose == 1 || verbose == 3){
        cout <<"FileSystem::FileSystem(FileSystem &&other)"<< endl;
    }
    other.rootDirectory = nullptr;
    other.workingDirectory = nullptr;
}


//move assignment//TODO
FileSystem& FileSystem::operator=(FileSystem &&other) {
    if(verbose == 1 || verbose == 3){
        cout <<"FileSystem& FileSystem::operator=(FileSystem &&other)"<< endl;
    }
    if (this != &other)
    {
       clear();
        rootDirectory = other.rootDirectory;
        workingDirectory = other.workingDirectory;
        other.workingDirectory = nullptr;
        other.workingDirectory = nullptr;
    }

    return *this;
}


Directory& FileSystem::getRootDirectory() const {

    return *rootDirectory;
}



Directory& FileSystem::getWorkingDirectory() const{

    return *workingDirectory;
}


void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {
    workingDirectory = newWorkingDirectory;
}

void  FileSystem::copy(Directory &otherRoot, Directory &otherWdir)  {//TODO
    string path = otherWdir.getAbsolutePath();
    CdCommand* cd = new CdCommand(path);
    cd->execute(*this);
}

void FileSystem::clear() {//TODO
    workingDirectory= nullptr;
    delete rootDirectory;
    rootDirectory = nullptr;
}