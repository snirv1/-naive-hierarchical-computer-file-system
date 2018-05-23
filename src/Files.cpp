#include "../include/Files.h"
#include <vector>
#include <iostream>

using namespace std;

//*Base File* no need rule of 5


BaseFile::BaseFile(string name) :name(name) {}//Constructor

BaseFile::~BaseFile() {
    if(verbose == 1 || verbose == 3){
        cout <<"BaseFile::~BaseFile()"<< endl;
    }
}//Destructor//TODO sv

string BaseFile::getName() const { return name; }

void BaseFile::setName(string newName) { name = newName; }


/*
 bool BaseFile::nameCheck() {
    string name = this->getName();
    bool isLegal = true;
    for (std::string::iterator it = name.begin();it != name.end() && isLegal ; ++it)
    {
        if ((*it >= 'A' && *it <= 'Z') || (*it >= 'a' && *it <= 'z') || (*it >= '0' && *it <= '9'))
            isLegal = true;
        else isLegal = false;
    }
    return isLegal;
}*/



//*File*  no need rule of 5


File::File(string name, int size) :BaseFile(name), size(size) {}//Constructor

File::~File() {}

int File::getSize() { return size; }

BaseFile * File::clone(BaseFile* parent) {
    File* newFile = new File(this->getName(),this->getSize());
    SV_DEBUG("copy newfile: "<< newFile->getName() << " size: " << newFile->size)
    return newFile;
}

string File::getType() { return "FILE"; }

/////**Directory******** !!!!!need rule of 5!!!!


//constructor
Directory::Directory(string name, Directory *parent) : BaseFile(name), children(),parent(parent){}//Constructor

// copy constructor
Directory::Directory(const Directory & other):BaseFile(other.getName()),children(), parent(){ //Directory(other.getName(), nullptr){
    if(verbose == 1 || verbose == 3){
        cout <<"Directory::Directory(const Directory & other)"<< endl;
    }

    for (size_t i = 0 ;i < other.children.size(); i++){

        children.push_back(((other.children)[i]->clone(this)));
    }


}//TODO

//Destructor
Directory::~Directory() {
    if(verbose == 1 || verbose == 3){
        cout <<"Directory::~Directory()"<< endl;
    }
    for (std::vector<BaseFile*>::iterator it = children.begin(); it != children.end(); ++it){
        delete *it;
        (*it) = nullptr;
    }
    children.clear(); //TODO sv
}//TODO

// copy assignment operator
Directory& Directory::operator=(const Directory &other){
    if(verbose == 1 || verbose == 3){
        cout <<"Directory& Directory::operator=(const Directory &other)"<< endl;
    }
    if (this != &other) {

        clear();
        for (size_t i = 0 ;i < children.size(); ++i){
            children.push_back(((other.children).at(i)->clone(this)));
        }
    }return *this;
    //return *this;//TODO
}//TODO

//move constructor
Directory::Directory(Directory &&other):BaseFile(other.getName()), children(other.children), parent(other.parent) {
    if(verbose == 1 || verbose == 3){
        cout <<"Directory::Directory(Directory &&other)"<< endl;
    }
    other.children.clear();
    other.parent = nullptr;

}//TODO

//move assignment
Directory& Directory::operator=(Directory &&other) {
    if(verbose == 1 || verbose == 3){
        cout <<"Directory& Directory::operator=(Directory &&other)"<< endl;
    }
    if (this != &other) {
        clear();
        for (size_t i = 0 ;i < children.size(); ++i){
            children.push_back(other.children[i]);
            other.children[i] = nullptr;
        }
    }return *this;
    //return *this;

}//TODO

Directory* Directory::getParent() const { return parent; }

void Directory::setParent(Directory *newParent) {
    parent = newParent;

}

void Directory::addFile(BaseFile *file) {
    bool allReadyExists = false;
    SV_DEBUG("")
    //for (std::vector<BaseFile *>::iterator it = children.begin(); !allReadyExists && it != children.end(); ++it) {
        for(size_t i = 0;!allReadyExists && i< children.size();i++){
            SV_DEBUG("")
        if (children[i]->getName() == file->getName()) {
            allReadyExists = true;
            delete file;
            SV_DEBUG("!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        }
    }
    SV_DEBUG("")
        if (!allReadyExists) {
            SV_DEBUG("")
            children.push_back(file);
            if(file->getType() == "DIR"){
                SV_DEBUG("")
                ((Directory*)file)->setParent(this);
            }
        }
    }

void Directory::removeFile(string name) {
    bool found = false;
    for (std::vector<BaseFile*>::iterator it = children.begin(); it != children.end() && !found; ++it) {
        if ((**it).getName() == name) {
            found = true;
            delete *it;
            children.erase(it);
        }
    }
}

void Directory::removeFile(BaseFile* file) {//not correct but not used
    bool found = false;
    for (std::vector<BaseFile*>::iterator it = children.begin(); it != children.end() && !found; ++it) {
        if (*it == file) {
            found = true;
            delete *it;
            children.erase(it);
        }
    }
}

void Directory::sortByName() {
    SV_DEBUG("")
    size_t  chi_size = children.size();
    SV_DEBUG("size: "<<chi_size)
    SV_DEBUG("name"<< this->getName())

    BaseFile* temp;
    for (size_t i = 0; i < chi_size; i++) {
        for (size_t j = 1; j < (chi_size - i); j++)
        {
            if (children[j - 1]->getName() > children[j]->getName()) {
                temp = children[j - 1];
                children[j - 1] = children[j];
                children[j] = temp;
            }
        }

    }

}

void Directory::sortBySize() {
    int chi_size = children.size();
    BaseFile* temp;
    for (int i = 0; i < chi_size; i++) {
        for (int j = 1; j < (chi_size - i); j++)
        {
            if (children[j - 1]->getSize() > children[j]->getSize()) {
                temp = children[j - 1];
                children[j - 1] = children[j];
                children[j] = temp;
            }
        }

    }

}

string Directory::getAbsolutePath() {
    string toReturn = getAbsolutePathHelp();
    if (toReturn == "") {
        return "/";
    }
    else {
        return toReturn;
    }
}

string Directory::getAbsolutePathHelp() {
    if (parent == nullptr) {
        return "";
    }
    else {
        return parent->getAbsolutePathHelp() + "/" + getName();
    }
}

vector<BaseFile*> Directory::getChildren() { return children; }

int Directory::getSize() {
    int mySize = 0;
    for (size_t i=0; i < children.size();i++){
        if (children[i]->getType()=="FILE")
        { File* filePtr = (File*)(children[i]);
        mySize = mySize +filePtr->getSize();
        }
        else{
            Directory* dirPtr = (Directory*)(children[i]);
            mySize = mySize +dirPtr->getSize();
        }
    }
    return mySize;
}

BaseFile* Directory::clone(BaseFile* parent) {
   // Directory* newDir = new Directory(this->getName(),(Directory*)parent);
  //  SV_DEBUG("newdir name: " << newDir->getName() << " new parent: " << parent->getName())
BaseFile* newDir = new Directory(*this);

    return newDir;
    /*
    if(children.empty()){
        SV_DEBUG("")
        return  newDir;
    } else{
        for (size_t i=0; i< children.size();i++) {
            SV_DEBUG("we push in clone: " << (children)[i]->clone(this)->getName() )
            ((*newDir).getChildren()).push_back(children[i]->clone(newDir));
        }
    }
    return newDir;//TODO return
     */
}

string Directory::getType() { return "DIR"; }

void Directory::clear() {
    parent = nullptr;
    for(std::vector<BaseFile*>::iterator it = children.begin(); it != children.end(); ++it){
        delete (*it);
        *it = nullptr;
    }
}