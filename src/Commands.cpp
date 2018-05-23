#include "../include/Commands.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstddef>
#include <algorithm>
#include "../include/Environment.h"

using namespace std;

////******BaseCommand*******
BaseCommand::BaseCommand(string args):args(args) {}//Constructor

BaseCommand::~BaseCommand() {}//Destructor//TODO sv

string BaseCommand::getArgs() { return args;}

vector<string> BaseCommand::breakDownPath(string args) {
    string path;
    vector<string> ret;
		SV_DEBUG("args = " << args)
    int i = (-1);
    if(args == "/")
    {
        ret.push_back("/");
        return ret;
    }
    else
    {
        if (args.substr(0, 1) == "/") {
            ret.push_back("/");
            path = args.substr(1);
        }
        else {path = args;}
        std::size_t found = path.find_first_of("/");
        while (found != std::string::npos){
            ret.push_back(path.substr(i + 1, found - i - 1));
            i = found;
            found = path.find_first_of("/", found + 1);
        }
        ret.push_back(path.substr(i + 1, path.size() - i - 1));
        return ret;

    }
}


Directory* BaseCommand::getDirectory(vector<string> pathV, Directory& workingD, FileSystem& fs,int a) {
		SV_DEBUG("")
    bool foundLast = false;
    if ( pathV.size() == 0) { //TODO REMOVE sv
        return nullptr;
    }
    Directory *ret = &workingD;
    if (pathV.at(0) == "/") {//absolute
        ret = &(fs.getRootDirectory());
        if (pathV.size() == 1) {
            return &fs.getRootDirectory(); //TODO no one to return no father for root(slove in functions)
        } else if (pathV.size() == 2) {
            return &fs.getRootDirectory();
        }
        for (size_t j = 1; (j < pathV.size() - 1); j++) {//loop over path
            bool found = false;
            for (size_t k = 0; k < ret->getChildren().size(); k++) {//loop over ret.children
                if (ret->getChildren()[k]->getName() == pathV[j]) {
                    found = true;
                    if (ret->getChildren()[k]->getType() != "DIR") {
                        return nullptr;
                    }
                    if (j == (pathV.size() - 2)) {
                        foundLast = true;
                    }
                    ret = (Directory *) ret->getChildren()[k];
                }

            }
            if (!found) {
                return nullptr;
            }
        }
    } else {//relative
        if(pathV.size() == 1){
            return &workingD;
        }
        for (size_t j = 0; (j < pathV.size() - 1); j++) {//loop over path
            bool found = false;
            for (size_t k = 0; k < ret->getChildren().size(); k++) {//loop over ret.children
                if (ret->getChildren()[k]->getName() == pathV[j]) {
                    found = true;
                    if (ret->getChildren()[k]->getType() != "DIR") {
                        return nullptr;
                    }
                    if (j == (pathV.size() - 2)) {
                        foundLast = true;
                    }
                    ret = (Directory *) ret->getChildren()[k];
                }

            }
            if (!found) {
                return nullptr;
            }
        }
    }
    if (foundLast == true) {
        return ret;
    } else {
        return nullptr;
    }
}

Directory* BaseCommand::getDirectory(vector<string> pathV, Directory& workingD, FileSystem& fs) {
    bool fileInPath = false;
    if(pathV.size() == 0){ return nullptr;}
		SV_DEBUG("")
    bool foundLast = false;
    bool lastIsFather = (pathV[pathV.size()-1]=="..");
		SV_DEBUG("")
    if (pathV.size() == 0) {
        return nullptr;
    }
		SV_DEBUG("")
    Directory *ret = &workingD;
		
    if (pathV.at(0) == "/") {//absolute
				SV_DEBUG("")
        ret = &(fs.getRootDirectory());
        if (pathV.size() == 1) {
            return &fs.getRootDirectory(); //TODO no one to return no father for root(slove in functions)
        } else if (pathV.size() == 2 && pathV[1] == "..") {
            return nullptr;
        } else if (pathV.size() == 2 && pathV[1] != "..") {
            return &fs.getRootDirectory();

        }
        for (size_t j = 1;!fileInPath && (j < pathV.size() - 1); j++) {//loop over path
            if (pathV[j] != "..") {
								SV_DEBUG("")
                bool found = false;
                for (size_t k = 0; !fileInPath && k < ((ret->getChildren()).size()); k++) {//loop over ret.children
                    if (ret->getChildren()[k]->getName() == pathV[j]) {
                        found = true;

                        if (ret->getChildren()[k]->getType() != "DIR") {
                            //std::cout << "The system cannot find the path specified" << endl;
                            fileInPath = true;

                        }if(!fileInPath) {
                            if (j == (pathV.size() - 2)) {
                                foundLast = true;
                            }
                            ret = (Directory *) ret->getChildren()[k];
                        }
                    }

                }if(fileInPath){
                    std::cout <<"The directory already exists" <<endl;
                    return nullptr;
                }
                if (!found) {
                    return nullptr;
                }
            } else if (pathV[j] == "..") {
								SV_DEBUG("")
                if (ret->getName() != "/") {
                    ret = ret->getParent();
                    if (j == (pathV.size() - 2)) {
                        bool lastIsFatherChild = false;
                        for (size_t i = 0; i < ((ret->getChildren()).size()); i++) {
                            if (ret->getChildren()[i]->getName() == pathV[pathV.size() - 1]) {
                                lastIsFatherChild = true;
                            }

                            if (lastIsFatherChild) { foundLast = true; }
                        }
                    }
                }else if (ret->getName() == "/") {
                    return nullptr;
                }

            }
        }
    }
    else {//relative//
				SV_DEBUG("")
        if ((pathV.size() == 1) && (pathV[0] == "..") && (&workingD == &fs.getRootDirectory())) {
	SV_DEBUG("")
            return nullptr;
        } else if ((pathV.size() == 1)&& (pathV[0] != "..")) {
SV_DEBUG("pathV:"<<printVector(pathV))
            return &workingD;
        }
        else if ((pathV.size() == 1) && (pathV[0] == "..") && (&workingD != &fs.getRootDirectory())) {
SV_DEBUG("")
            ret = ret->getParent();
            return ret;
        }
        for (size_t j = 0;!fileInPath &&(j < pathV.size() - 1); j++) {//loop over path
            if (pathV[j] != "..") {
SV_DEBUG("")
                bool found = false;
                SV_DEBUG("ret: "<<ret)
                SV_DEBUG("pathV: "<< printVector(pathV))
                for (size_t k = 0;!fileInPath && k < ret->getChildren().size(); k++) {//loop over ret.children
                    if (ret->getChildren()[k]->getName() == pathV[j]) {
                        SV_DEBUG("")
                        found = true;
                        if (ret->getChildren()[k]->getType() != "DIR") {
                            //std::cout << "The system cannot find the path specified" << endl;
                            fileInPath = true;
                        }SV_DEBUG("")
                        if(!fileInPath) {
                            if (j == (pathV.size() - 2)) {
                                SV_DEBUG("")
                                foundLast = true;
                            }
                            ret = (Directory *) ret->getChildren()[k];
                        }
                    }

                }SV_DEBUG("")if(fileInPath) {
                    std::cout << "The directory already exists" << endl;
                    return nullptr;
                }
                if (!found) {
                    SV_DEBUG("")
                    return nullptr;
                }
            } else if (pathV[j] == "..") {
								SV_DEBUG("")
                if (ret->getName() != "/") {
SV_DEBUG("")
                    ret = ret->getParent();
                    if (j == (pathV.size() - 2)) {
SV_DEBUG("")
                        bool lastIsFatherChild = false;
                        for (size_t i = 0; i < ((ret->getChildren()).size()); i++) {
SV_DEBUG("")
                            if (ret->getChildren()[i]->getName() == pathV[pathV.size() - 1]) {
                                lastIsFatherChild = true;
                            }
SV_DEBUG("")
                            if (lastIsFatherChild) { foundLast = true; }
                        }
                    }
SV_DEBUG("")
                } else if (ret->getName() == "/") {
SV_DEBUG("")
                    return nullptr;
                }
SV_DEBUG("")
            }
        }
    }
SV_DEBUG("")
    if ((foundLast == true) && !lastIsFather) {
SV_DEBUG("")
        return ret;
    } else if ((foundLast == true) && lastIsFather && (ret->getParent() != nullptr)) {
SV_DEBUG("")
        return ret->getParent();
    }
    else {
SV_DEBUG("")
        return nullptr;
    }
}//TODO sd


bool BaseCommand::getDirectoryFileInpath(vector<string> pathV, Directory& workingD, FileSystem& fs) {
    bool fileInPath = false;
    if(pathV.size() == 0){ return false;}
    SV_DEBUG("")
    bool foundLast = false;
    bool lastIsFather = (pathV[pathV.size()-1]=="..");
    SV_DEBUG("")
    if (pathV.size() == 0) {
        return false;
    }
    SV_DEBUG("")
    Directory *ret = &workingD;

    if (pathV.at(0) == "/") {//absolute
        SV_DEBUG("")
        ret = &(fs.getRootDirectory());
        if (pathV.size() == 1) {
            return false; //TODO no one to return no father for root(slove in functions)
        } else if (pathV.size() == 2 && pathV[1] == "..") {
            return false;
        } else if (pathV.size() == 2 && pathV[1] != "..") {
            return false;

        }
        for (size_t j = 1;!fileInPath && (j < pathV.size() - 1); j++) {//loop over path
            if (pathV[j] != "..") {
                SV_DEBUG("")
                bool found = false;
                for (size_t k = 0; !fileInPath && k < ((ret->getChildren()).size()); k++) {//loop over ret.children
                    if (ret->getChildren()[k]->getName() == pathV[j]) {
                        found = true;

                        if (ret->getChildren()[k]->getType() != "DIR") {
                            //std::cout << "The system cannot find the path specified" << endl;
                            fileInPath = true;
                            return fileInPath;

                        }if(!fileInPath) {
                            if (j == (pathV.size() - 2)) {
                                foundLast = true;
                            }
                            ret = (Directory *) ret->getChildren()[k];
                        }
                    }

                }if(fileInPath){
                    std::cout <<"The directory already exists" <<endl;
                    return true;
                }
                if (!found) {
                    return false;
                }
            } else if (pathV[j] == "..") {
                SV_DEBUG("")
                if (ret->getName() != "/") {
                    ret = ret->getParent();
                    if (j == (pathV.size() - 2)) {
                        bool lastIsFatherChild = false;
                        for (size_t i = 0; i < ((ret->getChildren()).size()); i++) {
                            if (ret->getChildren()[i]->getName() == pathV[pathV.size() - 1]) {
                                lastIsFatherChild = true;
                            }

                            if (lastIsFatherChild) { foundLast = true; }
                        }
                    }
                }else if (ret->getName() == "/") {
                    return false;
                }

            }
        }
    }
    else {//relative//
        SV_DEBUG("")
        if ((pathV.size() == 1) && (pathV[0] == "..") && (&workingD == &fs.getRootDirectory())) {
            SV_DEBUG("")
            return false;
        } else if ((pathV.size() == 1)&& (pathV[0] != "..")) {
            SV_DEBUG("pathV:"<<printVector(pathV))
            return false;
        }
        else if ((pathV.size() == 1) && (pathV[0] == "..") && (&workingD != &fs.getRootDirectory())) {
            SV_DEBUG("")
            ret = ret->getParent();
            return false;
        }
        for (size_t j = 0;!fileInPath &&(j < pathV.size() - 1); j++) {//loop over path
            if (pathV[j] != "..") {
                SV_DEBUG("")
                bool found = false;
                SV_DEBUG("ret: "<<ret)
                SV_DEBUG("pathV: "<< printVector(pathV))
                for (size_t k = 0;!fileInPath && k < ret->getChildren().size(); k++) {//loop over ret.children
                    if (ret->getChildren()[k]->getName() == pathV[j]) {
                        SV_DEBUG("")
                        found = true;
                        if (ret->getChildren()[k]->getType() != "DIR") {
                            //std::cout << "The system cannot find the path specified" << endl;
                            fileInPath = true;
                            return fileInPath;
                        }SV_DEBUG("")
                        if(!fileInPath) {
                            if (j == (pathV.size() - 2)) {
                                SV_DEBUG("")
                                foundLast = true;
                            }
                            ret = (Directory *) ret->getChildren()[k];
                        }
                    }

                }SV_DEBUG("")if(fileInPath) {
                    std::cout << "The directory already exists" << endl;
                    return true;
                }
                if (!found) {
                    SV_DEBUG("")
                    return false;
                }
            } else if (pathV[j] == "..") {
                SV_DEBUG("")
                if (ret->getName() != "/") {
                    SV_DEBUG("")
                    ret = ret->getParent();
                    if (j == (pathV.size() - 2)) {
                        SV_DEBUG("")
                        bool lastIsFatherChild = false;
                        for (size_t i = 0; i < ((ret->getChildren()).size()); i++) {
                            SV_DEBUG("")
                            if (ret->getChildren()[i]->getName() == pathV[pathV.size() - 1]) {
                                lastIsFatherChild = true;
                            }
                            SV_DEBUG("")
                            if (lastIsFatherChild) { foundLast = true; }
                        }
                    }
                    SV_DEBUG("")
                } else if (ret->getName() == "/") {
                    SV_DEBUG("")
                    return false;
                }
                SV_DEBUG("")
            }
        }
    }
    SV_DEBUG("")
    if ((foundLast == true) && !lastIsFather) {
        SV_DEBUG("")
        return false;
    } else if ((foundLast == true) && lastIsFather && (ret->getParent() != nullptr)) {
        SV_DEBUG("")
        return false;
    }
    else {
        SV_DEBUG("")
        return false;
    }
}//TODO sd


BaseFile* BaseCommand::isChild(string name, Directory* directory) {
		SV_DEBUG("directory " << directory)
    ////return ptr to the child(name), if he is indeed a child of directory else return nullptr(check by name)
    if(directory != nullptr) {
        SV_DEBUG("")
        for (size_t i = 0; i < ((directory->getChildren()).size()); i++) {
            SV_DEBUG("")
            //for (std::vector<BaseFile*>::iterator it = directory.getChildren().begin(); it != directory.getChildren().end(); ++it){
            if (((directory->getChildren())[i])->getName() == name) {
                SV_DEBUG("")
                return ((directory->getChildren())[i]);
            }
        }
    }
		SV_DEBUG("")
    return nullptr;
}

vector<string> BaseCommand::getActualPath(FileSystem &fs, vector<string> &path) {//takes a path that have ".." and make it a regular path
    SV_DEBUG("")
    vector<string> absolutePathToAdd;
    SV_DEBUG("path: "<<printVector(path))
    if(path[0] != "/"){
        SV_DEBUG("")
        string absPath = fs.getWorkingDirectory().getAbsolutePath();
        SV_DEBUG("absPath: "<<absPath)
        absolutePathToAdd = breakDownPath(absPath);
        SV_DEBUG("abslolute path to add: " << printVector(absolutePathToAdd))
        for(size_t i = 0; i < path.size(); i++ ){
            absolutePathToAdd.push_back(path[i]);
        }
        path = absolutePathToAdd;
        SV_DEBUG("path after combine: "<<printVector(path))
    }
    vector<string> actualPath;
    for(size_t i =0; i < path.size();i++){
        if (path[i] != ".."){
            actualPath.push_back(path[i]);
        } else{
            if(actualPath.size() != 0){
                actualPath.pop_back();
            } else{
                SV_DEBUG("actualPath:" << printVector(actualPath))
                return actualPath;
            }
        }

    }SV_DEBUG("actualPath:" << printVector(actualPath))
    return actualPath;

}


////******PwdCommand*****
PwdCommand::PwdCommand(string args):BaseCommand(args) {}

void PwdCommand::execute(FileSystem &fs) {
   string toPrint = fs.getWorkingDirectory().getAbsolutePath();
    std::cout<< toPrint << endl;
}

string PwdCommand::toString() { return "pwd ";}

PwdCommand* PwdCommand::clone() {
    PwdCommand* ret = new PwdCommand(getArgs());
    return ret;
}

////*****CdCommand****
CdCommand::CdCommand(string args):BaseCommand(args){}//Constructor

void CdCommand::execute(FileSystem &fs) {
    SV_DEBUG("")
    if(getArgs() == "/"){
        SV_DEBUG("")
        fs.setWorkingDirectory(&fs.getRootDirectory());
        return;
    }
   // if(getArgs() == ".." && (&fs.getWorkingDirectory() == &fs.getRootDirectory())){
      //  std::cout << "The system cannot find the path specified" << endl;
      //  return;
   // } else if(getArgs() == ".." && (&fs.getWorkingDirectory() != &fs.getRootDirectory())){
      //  fs.setWorkingDirectory((fs.getWorkingDirectory().getParent()));
      //  return;
   // }
    SV_DEBUG("")
    vector<string> path = breakDownPath(getArgs());
    SV_DEBUG("")
    size_t found = getArgs().find_first_of("..");
    if(found!=std::string::npos) {
        SV_DEBUG("path: "<< printVector(path))
        path = getActualPath(fs, path);
        SV_DEBUG("path new: "<< printVector(path))
        if(path.size() == 0){
            std::cout << "The system cannot find the path specified" << endl;
            return;
        }
        if(path.size() == 1 && path[0]== "/"){
            fs.setWorkingDirectory(&fs.getRootDirectory());
            return;
        }
    }
    string name = path[path.size() -1];
    SV_DEBUG("")
    Directory* dirTarget = getDirectory(path , fs.getWorkingDirectory(),fs);
    if(dirTarget != nullptr){
       // if(dirTarget->getName() == "/"){
         //   fs.setWorkingDirectory(&fs.getRootDirectory());
            //return;
       // }
        SV_DEBUG("dirTarget:"<<dirTarget->getName())////legal path
        if (name != "..") {
            SV_DEBUG("")
            bool found = false;
            for (size_t i = 0; i < dirTarget->getChildren().size(); ++i) {
                if ((dirTarget->getChildren()[i]->getName() == name) && dirTarget->getChildren()[i]->getType() == "DIR" ) {
                    SV_DEBUG("")
                    fs.setWorkingDirectory((Directory *) dirTarget->getChildren()[i]);
                    found = true;

                } else if((dirTarget->getChildren()[i]->getName() == name) && dirTarget->getChildren()[i]->getType() == "FILE"){
                    std::cout << "The system cannot find the path specified" << endl;
                }
            }if(!found){
                std::cout << "The system cannot find the path specified" << endl;
            }
        }
        else if(dirTarget->getType() == "DIR"){
            SV_DEBUG("")
            fs.setWorkingDirectory(dirTarget->getParent());
        }
    } else{////unlegal path
        std::cout << "The system cannot find the path specified" << endl;
    }
}//TODO sd 1100

string CdCommand::toString() { return "cd ";}

CdCommand* CdCommand::clone() {
    CdCommand* ret = new CdCommand(getArgs());
    return ret;
}

 ////******lsCommand******

LsCommand::LsCommand(string args):BaseCommand(args){}//Ls contructor

string LsCommand::toString() { return "ls "; }// LS toString- return the command name

void LsCommand::execute(FileSystem & fs) {
    SV_DEBUG("wd: "<< fs.getWorkingDirectory().getName())
    vector<string> path;
    bool sortByName = true;
    if (getArgs() != "" && getArgs() != "-s") {
        SV_DEBUG("")
        if (getArgs().substr(0, 2) == "-s") {
            // SV_DEBUG("")
            sortByName = false;
            SV_DEBUG("wd: "<< fs.getWorkingDirectory().getName())
            path = breakDownPath(getArgs().substr(3));
            size_t found = getArgs().find_first_of("..");
            if(found!=std::string::npos){
                path = getActualPath(fs,path);
                if(path.size() == 0){
                    std::cout << "The system cannot find the path specified" << endl;
                    return;
                }

            }
        } else {
            SV_DEBUG("")
            SV_DEBUG("wd: "<< fs.getWorkingDirectory().getName())
            path = breakDownPath(getArgs());
            size_t found = getArgs().find_first_of("..");
            if(found!=std::string::npos){
                path = getActualPath(fs,path);
                if(path.size() == 0){
                    std::cout << "The system cannot find the path specified" << endl;
                    return;
                }
            }
        }SV_DEBUG("wd: "<< fs.getWorkingDirectory().getName())
        Directory *target;
        target = getDirectory(path, fs.getWorkingDirectory(), fs);
        if(target == nullptr){
            std::cout << "The system cannot find the path specified" << endl;
            return;
        }SV_DEBUG("wd: "<< fs.getWorkingDirectory().getName())

        //check last path is file or dir
        BaseFile *lastpath_p = nullptr;
        Directory *lastPathDir_p = nullptr;
       // SV_DEBUG("path:" << printVector(path))//TODO
        SV_DEBUG("wd: "<< fs.getWorkingDirectory().getName())
        if ((path.size() != 1 || path.at(0) != "/")) {//path is relative and grater then one
            SV_DEBUG("")
            string lastpath = path[path.size() - 1];
            SV_DEBUG("lastPath: " << lastpath)
            bool found = false;
            SV_DEBUG("target:" << target)
            //for (std::vector<BaseFile *>::iterator it = (target->getChildren().begin());
            // !(found) && it != (target->getChildren().end()); ++it) {
            SV_DEBUG("target: " << target->getName())
            SV_DEBUG("target: " << target->getChildren().size())
            for (size_t i = 0; !(found) && i < target->getChildren().size(); i++) {

                if (target->getChildren()[i]->getName() == lastpath) {
                    SV_DEBUG("")
                    lastpath_p = target->getChildren()[i];
                    found = true;
                }
            }
            SV_DEBUG("")
            if (found && (lastpath_p != nullptr) && lastpath_p->getType() == "FILE") {//if the last path is a file
                SV_DEBUG("")
                std::cout << "\nThe system cannot find the path specified" << endl;
                return;
            }
            //if dir
            lastPathDir_p = (Directory *) (lastpath_p);

        }else{
                    SV_DEBUG("")
                lastPathDir_p = &fs.getRootDirectory();
            }
        if(lastPathDir_p == nullptr){
            std::cout << "The system cannot find the path specified" << endl;
            return;
        }
            SV_DEBUG("")
            if (sortByName) {SV_DEBUG("lastPathDir: "<<lastPathDir_p->getName()) (*lastPathDir_p).sortByName(); }
            else { SV_DEBUG("") (*lastPathDir_p).sortBySize(); }


            //for (std::vector<BaseFile *>::iterator it = (lastPathDir_p->getChildren().begin());it != (lastPathDir_p->getChildren().end()); ++it) {
        SV_DEBUG("lastPathDir_p->getChildren().size(): "<< lastPathDir_p->getChildren().size())
            for (size_t i = 0; i < lastPathDir_p->getChildren().size(); i++) {

                std::cout << lastPathDir_p->getChildren()[i]->getType() << "\t"
                          << lastPathDir_p->getChildren()[i]->getName() << "\t"
                          << lastPathDir_p->getChildren()[i]->getSize() << endl;//TODO sv remove /n


            }SV_DEBUG("")
        } else {
            if (getArgs() == "-s") {
                fs.getWorkingDirectory().sortByName();
                fs.getWorkingDirectory().sortBySize();

            } else { fs.getWorkingDirectory().sortByName(); }

            for (size_t i = 0; i < (((fs.getWorkingDirectory()).getChildren()).size()); i++) {

                std::cout << ((fs.getWorkingDirectory()).getChildren())[i]->getType();//TODO sv remove /n
                std::cout << "\t" << ((fs.getWorkingDirectory()).getChildren())[i]->getName() << "\t";
                std::cout << ((fs.getWorkingDirectory()).getChildren())[i]->getSize() << endl;

            }
        }
    SV_DEBUG(""<< fs.getWorkingDirectory().getName())
    }

LsCommand* LsCommand::clone() {
    LsCommand* ret = new LsCommand(getArgs());
    return ret;
}

////*****Mkdir******

MkdirCommand::MkdirCommand(string args):BaseCommand(args) {}//Constructor

vector<string> MkdirCommand::getMaximalMatchingPath(FileSystem &fs, const vector<string>& path) {
	Directory& pwd = fs.getWorkingDirectory();
	vector<string> prevParPath;

	for (size_t i = 0; i < path.size(); i++) {
		vector<string> parPath;
		for (size_t j = 0; j < i + 1; j++) {
			parPath.push_back(path[j]);
		}
		
		SV_DEBUG("parPath = " << printVector(parPath))

		if (!getDirectory(parPath, pwd, fs)) {
			return prevParPath;
		}

		prevParPath = parPath;

	}

	return prevParPath;
}



void MkdirCommand::execute(FileSystem &fs) {
		SV_DEBUG("")
    if (getArgs() == "/") {
        std::cout << "The directory already exists" << endl;
        return;
    }
		
    vector<string> path = breakDownPath(getArgs());
    size_t found = getArgs().find_first_of("..");
    if(found!=std::string::npos){
        path = getActualPath(fs,path);
    }
    bool fileInPath = getDirectoryFileInpath(path,fs.getWorkingDirectory(),fs);
    if(fileInPath){
        std::cout << "The directory already exists" << endl;
        return;
    }
    Directory *dirTarget = getDirectory(path, fs.getWorkingDirectory(),fs);
		SV_DEBUG("path = " << printVector(path))
		SV_DEBUG("dirTarget = " << dirTarget)

		vector<string> maxPath = getMaximalMatchingPath(fs, path);
		SV_DEBUG("maxPath = " << printVector(maxPath))

    if (dirTarget != nullptr) {////legal path
				SV_DEBUG("")
        bool allReadyExists = false;
        size_t targetChildrenSize =(dirTarget->getChildren()).size();
        for (size_t i =0; i< targetChildrenSize ;i++){
            if (((dirTarget->getChildren())[i])->getName() == path[path.size()-1]){
                allReadyExists = true;
            }
        }
        if (!allReadyExists) {
            /*Directory* newDir = new Directory(path.back(), dirTarget);*/
            Directory* newDir = new Directory(path[path.size()-1], dirTarget);
            dirTarget->addFile(newDir);
        } else { std::cout << "The directory already exists" << endl; }

    } else { //path does not exits
        SV_DEBUG("")
        vector<string> newPath;
        size_t oldPathIt = 0;
        newPath.push_back(path[0]);
        if ((path[0] != "/") && !isChild(path[0], &fs.getWorkingDirectory())) {//if relative path
            SV_DEBUG("")
            Directory *newDir = new Directory(newPath.back(), &fs.getWorkingDirectory());
            fs.getWorkingDirectory().addFile(newDir);
        }
        if (path.size() != 0) {
        while ((newPath.size()) != path.size()) {
            bool notAchild = false;
            while (oldPathIt < (path.size() - 1) && getDirectory(newPath, fs.getWorkingDirectory(), fs) != nullptr &&
                   !notAchild) {
                oldPathIt++;
                SV_DEBUG("")
                newPath.push_back(path[oldPathIt]);
                SV_DEBUG("")
                if (isChild(newPath[newPath.size() - 1], getDirectory(newPath, fs.getWorkingDirectory(), fs)) ==
                    nullptr) {
                    notAchild = true;
                }
                SV_DEBUG("")
            }
            Directory *target = getDirectory(newPath, fs.getWorkingDirectory(), fs);
            if (target != nullptr) {
                Directory *newDir = new Directory(newPath.back(), target);
                target->addFile(newDir);
            }
        }
    }
        }
		SV_DEBUG("")
}


string MkdirCommand::toString() {return "mkdir ";}

MkdirCommand* MkdirCommand::clone() {

    MkdirCommand* ret = new MkdirCommand(getArgs());
    return ret;
}

////****MkFile******

MkfileCommand::MkfileCommand(string args):BaseCommand(args) {}//Constructor


void MkfileCommand::execute(FileSystem &fs) {
    Directory *dirTarget;
    vector<string> path;
    size_t found = getArgs().find_first_of(" ");
    string pathStr;
    string sizeStr;
    if(found!=std::string::npos) {/*check if input is <path> <size> and break it down*/
        pathStr = getArgs().substr(0, found);
        sizeStr = getArgs().substr(found + 1);
        int size = stoi(sizeStr);
        if (pathStr == "/") {////in case root
            dirTarget = &fs.getRootDirectory();
        } else {////call getDir to find Target
            path = breakDownPath(pathStr);
            dirTarget = getDirectory(path, fs.getWorkingDirectory(),fs);
        }
        if (dirTarget != nullptr) {////if legal path
            bool allReadyExists = false;
            size_t targetChildrenSize =(dirTarget->getChildren()).size();
            for (size_t i =0; i< targetChildrenSize ;i++){
                if (((dirTarget->getChildren())[i])->getName() == path[path.size()-1]){
                    allReadyExists = true;
                }
            }
            if (!allReadyExists) {/*add the file*/
                File* newFile =  new File(path.back(), size);
                dirTarget->addFile(newFile);
            } else { std::cout << "File already exists" << endl; }
        } else {/*If Illegal path*/
            std::cout<< "The system cannot find the path specified" << endl;
        }

}else{
      std::cout<< "The system cannot find the path specified" << endl;
    }
}

string MkfileCommand::toString() { return "mkfile ";}

MkfileCommand* MkfileCommand::clone() {
    MkfileCommand* ret = new MkfileCommand(getArgs());
    return ret;
}

/*****Cp****/

CpCommand::CpCommand(string args):BaseCommand(args) {}//Constructor

void CpCommand::execute(FileSystem &fs) {
    size_t found = getArgs().find_first_of(" ");/*geting ptr for src flie/dir an ptr to dst dir*/
    string srcPathStr;
    string dstPathStr;
    if (found != std::string::npos) {
        SV_DEBUG("")
        srcPathStr = getArgs().substr(0, found);
        dstPathStr = getArgs().substr(found + 1);
        SV_DEBUG("srcPath="<<srcPathStr<<" " <<"dstPath="<< dstPathStr)
    } else {/*wrong input*/
        SV_DEBUG("")
        std::cout << "No such file or directory" << endl;
        return;
    }
    vector<string> srcPathVct = breakDownPath(srcPathStr);
    vector<string> dstPathVct = breakDownPath(dstPathStr);
    size_t foundSrc = srcPathStr.find_first_of("..");
    if(foundSrc!=std::string::npos) {
        srcPathVct = getActualPath(fs, srcPathVct);
    }
        size_t foundDst = dstPathStr.find_first_of("..");
        if(foundDst!=std::string::npos) {
            dstPathVct = getActualPath(fs, dstPathVct);
        }
    Directory *srcPathPtr = getDirectory(srcPathVct, fs.getWorkingDirectory(),fs);
    Directory *dstPathPtr = getDirectory(dstPathVct, fs.getWorkingDirectory(),fs);
    SV_DEBUG("")
    /*Geting the names of src and dst*/
    if(srcPathPtr == nullptr ||dstPathPtr == nullptr ){
        std::cout << "No such file or directory" << endl;
        return;
    }
    string srcName = srcPathVct[srcPathVct.size()-1];
    string dstName = dstPathVct[dstPathVct.size()-1];
    SV_DEBUG("srcName:" << srcName << " "<< "dstName:"<< dstName << " srcPathPtr: "<< srcPathPtr->getName() << " "<< "dstPathPtr: "<< dstPathPtr->getName())
    /*coping src*/
    SV_DEBUG("")
    bool flag = false;
    File* toCopyFile = nullptr;
    Directory* toCopyDir = nullptr;
   // for (std::vector<BaseFile *>::iterator it = srcPathPtr->getChildren().begin();!flag && it != srcPathPtr->getChildren().end(); ++it) {
    for(size_t i = 0;!flag && i < srcPathPtr->getChildren().size();i++){

        SV_DEBUG("")
        if (srcPathPtr->getChildren()[i]->getName() == srcName) {
            SV_DEBUG("")
            flag = true;
            if ((srcPathPtr->getChildren()[i]->getType()) =="FILE"){
                toCopyFile = (File *) srcPathPtr->getChildren()[i];
                SV_DEBUG("found file: " << toCopyFile->getName() << " size: "<< toCopyFile->getSize())
            }else {
                SV_DEBUG("found dir")
                toCopyDir = (Directory*)srcPathPtr->getChildren()[i];
            }

        }
    }
    if (flag == false){
        SV_DEBUG("")
        std::cout << "No such file or directory" << endl;
        return;
    }
    Directory* dstPtr = nullptr;


    if (dstName != "/") {
        SV_DEBUG("")
        if (dstPathVct[dstPathVct.size() - 1] == "..") {
            dstPtr = dstPathPtr;

        } else {
        flag = false;
        for (size_t i = 0; !flag && i < dstPathPtr->getChildren().size(); i++) {
            if (dstPathPtr->getChildren()[i]->getName() == dstName) {
                SV_DEBUG("")
                flag = true;
                if ((dstPathPtr->getChildren()[i]->getType()) == "FILE") {
                    SV_DEBUG("")
                    std::cout << "No such file or directory" << endl;
                    return;
                }

                dstPtr = (Directory *) dstPathPtr->getChildren()[i];
            }

        }
    }
    }
    else {
        SV_DEBUG("")
        dstPtr = &fs.getRootDirectory();
    }
    if(dstPtr == nullptr){
        std::cout << "No such file or directory" << endl;
        return;
    }
    if(toCopyFile != nullptr) {
        File* ret = new File(toCopyFile->getName(),toCopyFile->getSize());
        dstPtr->addFile(ret);
    }
    if (toCopyDir != nullptr){
        SV_DEBUG("toCopydir type: " << toCopyDir->getType() <<" address: "<< toCopyDir)
        Directory* ret = new Directory(*toCopyDir);
        SV_DEBUG("ret:"<< ret->getName()<<" address: "<<ret <<" parent: "<< ret->getParent())
        SV_DEBUG("dstPtr: "<< dstPtr)
        dstPtr->addFile(ret);
    }


}

string CpCommand::toString() { return "cp ";}

CpCommand* CpCommand::clone() {
    CpCommand* ret = new CpCommand(getArgs());
    return ret;
}


/******Mv******/

MvCommand::MvCommand(string args): BaseCommand(args) {}

bool MvCommand::isAfatherOfWd(FileSystem &fs, Directory &parent) {
    if(parent.getName() == "/"){
        return true;
    }
    Directory* dir =&fs.getWorkingDirectory();
    while(dir->getParent() != nullptr){
        if(&parent == dir){
            return true;
        }
        dir = dir->getParent();
    }
    return false;
}

void MvCommand::execute(FileSystem &fs) {
    size_t found = getArgs().find_first_of(" ");/*geting ptr for src flie/dir an ptr to dst dir*/
    string srcPathStr;
    string dstPathStr;
    if (found != std::string::npos) {
        srcPathStr = getArgs().substr(0, found);
        dstPathStr = getArgs().substr(found + 1);
    } else {/*wrong input*/
        std::cout << "No such file or directory" << endl;
        return;
    }
    vector<string> srcPathVct = breakDownPath(srcPathStr);
    vector<string> dstPathVct = breakDownPath(dstPathStr);

    size_t foundSrc = srcPathStr.find_first_of("..");
    if(foundSrc!=std::string::npos) {
        srcPathVct = getActualPath(fs, srcPathVct);
    }
    size_t foundDst = dstPathStr.find_first_of("..");
    if(foundDst!=std::string::npos) {
        dstPathVct = getActualPath(fs, dstPathVct);
    }

    Directory *srcPathPtr = getDirectory(srcPathVct, fs.getWorkingDirectory(),fs);
    Directory *dstPathPtr = getDirectory(dstPathVct, fs.getWorkingDirectory(),fs);
    /*Geting the names of src and dst*/
    if(srcPathPtr == nullptr ||dstPathPtr == nullptr ){
        std::cout << "No such file or directory" << endl;
        return;
    }
    string srcName = srcPathVct[srcPathVct.size()-1];
    string dstName = dstPathVct[dstPathVct.size()-1];
    /*coping src*/
    bool flag = false;
    File* toCopyFile = nullptr;
    Directory* toCopyDir = nullptr;
    for(size_t i = 0;!flag && i < srcPathPtr->getChildren().size();i++){

        SV_DEBUG("")
        if (srcPathPtr->getChildren()[i]->getName() == srcName) {
            SV_DEBUG("")
            flag = true;
            if ((srcPathPtr->getChildren()[i]->getType()) =="FILE"){
                SV_DEBUG("")
                toCopyFile = (File *) srcPathPtr->getChildren()[i];

            }else {
                toCopyDir = (Directory*)srcPathPtr->getChildren()[i];
            }

        }
    }
    if (flag == false){
        std::cout << "No such file or directory" << endl;
        return;
    }
    Directory* dstPtr = nullptr;
    if (dstName != "/") {
        flag = false;
        for(size_t i = 0;!flag && i < dstPathPtr->getChildren().size();i++){
            if (dstPathPtr->getChildren()[i]->getName() == dstName) {
                SV_DEBUG("")
                flag = true;
                if ((dstPathPtr->getChildren()[i]->getType()) == "FILE") {
                    SV_DEBUG("")
                    std::cout << "No such file or directory" << endl;
                    return;
                }

                dstPtr = (Directory *) dstPathPtr->getChildren()[i];
            }

        }
    }
    else {
        dstPtr = &fs.getRootDirectory();
    }
    if(dstPtr== nullptr) {
        std::cout << "No such file or directory" << endl;
        return;
    }
    if(toCopyFile != nullptr) {
        File* ret = new File(toCopyFile->getName(),toCopyFile->getSize());
        dstPtr->addFile(ret);
    }
    if (toCopyDir != nullptr){
        if(isAfatherOfWd(fs,*toCopyDir)){
            std::cout << "Can't move directory" << endl;
            return;
        }
        SV_DEBUG("toCopydir: "<< toCopyDir << " " <<toCopyDir->getName()<< " " <<toCopyDir->getAbsolutePath()<<  "  " <<toCopyDir->getChildren().size()  )
        Directory* ret = new Directory(*toCopyDir);
//        SV_DEBUG("ret: "<< ret << " " <<ret->getName()<< " " <<ret->getAbsolutePath()<<  "  " << ret->getChildren().size()  << " childrens: "<< ret->getChildren()[0]->getName() << " " <<ret->getChildren()[1]->getName() <<" e child: "<< ((Directory*)ret->getChildren()[0])->getChildren()[0]->getName() )
        dstPtr->addFile(ret);
    }
    srcPathPtr->removeFile(srcName);
}

string MvCommand::toString() { return "mv ";}

MvCommand* MvCommand::clone() {
    MvCommand* ret = new MvCommand(getArgs());
    return ret;
}


/*******Rename******/

RenameCommand::RenameCommand(string args):BaseCommand(args) {}

void RenameCommand::execute(FileSystem &fs) {
    size_t foundSpc = getArgs().find_first_of(" ");/***braking down to path , old name , new name */
    string path;
    string oldName;
    string newName;
    if (foundSpc != std::string::npos) {
        path = getArgs().substr(0, foundSpc);
        newName = getArgs().substr(foundSpc + 1);
        /*find the target and change the name*/
        vector<string> pathVct = breakDownPath(path);
        Directory *target = getDirectory(pathVct, fs.getWorkingDirectory(), fs);
        if (target== nullptr){
            std::cout << "No such file or directory" << endl;
            return;
        }
        oldName = pathVct[pathVct.size() - 1];
        bool alreadyExist = false;
        for (size_t i = 0; i < target->getChildren().size() && !alreadyExist; i++) {
            if ((((target->getChildren())[i])->getName()) == newName) {
                alreadyExist = true;
            }
        }
        if (!alreadyExist) {
            bool found = false;
            for(size_t j=0; j<target->getChildren().size() && !found;j++){
                if (target->getChildren()[j]->getName()==oldName){
                    found = true;
                    if (&fs.getWorkingDirectory()==target->getChildren()[j])
                    {
                        std::cout << "Can't rename the working directory" << endl;
                        return;
                    }
                    target->getChildren()[j]->setName(newName);
                }
            }
            if (!found) {
                std::cout << "No such file or directory" << endl;
                return;
            }
        }
        else {
            //std::cout << "No such file or directory2" << endl;
            return;
        }
    }
    else{
        std::cout << "No such file or directory" << endl;
        return;
    }

}//TODO sd 0945


string RenameCommand::toString() {return "rename ";}

RenameCommand* RenameCommand::clone() {
    RenameCommand* ret = new RenameCommand(getArgs());
    return ret;
}

//******** RmCommand******

RmCommand::RmCommand(string args) :BaseCommand(args) {}//rm contructor

string RmCommand::toString() {return "rm ";}// rm toString- return the command name

void RmCommand::execute(FileSystem & fs) {

    if (getArgs() == "/") { //in case trying to delete the root
        std::cout << "Can't remove directory" << endl;
        return;
    }
    vector<string> path = breakDownPath(getArgs());
    Directory* target = getDirectory(path, fs.getWorkingDirectory(), fs);
    if (target == fs.getWorkingDirectory().getParent()) {
        std::cout << "Can't remove directory" << endl;
        return;
    }
    bool found = false;
   // for (std::vector<BaseFile*>::iterator it = (target->getChildren()).begin(); !found&& it != (target->getChildren()).end(); ++it) {
        for(size_t i = 0; i<target->getChildren().size(); i++ ){


        if (target->getChildren()[i]->getName() == path.back()) {
            found = true;
            if (target->getChildren()[i]->getType() == "FILE") {
                target->removeFile(target->getChildren()[i]->getName());//also delete from target children the deleted item
            }
            else {
                Directory* toDeleteDir = (Directory*)target->getChildren()[i];
                //delete toDeleteDir;
                //(target->getChildren()).erase(target->getChildren()[i]);
                target->removeFile(toDeleteDir);
            }


        }

    }
    if(found == false){ std::cout << "No such file or directory" << endl; }
}

RmCommand* RmCommand::clone() {
    RmCommand* ret = new RmCommand(getArgs());
    return ret;
}


//-----------working----------historyCommand-----------

HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> & history) : BaseCommand(args), history(history) {}

string HistoryCommand::toString() { return "history "; }

void HistoryCommand::execute(FileSystem & fs) {

    //std::cout << "\n";
    for (size_t i = 0; i < history.size(); i++) {
        std::cout << i << "\t" << history[i]->toString() <<history[i]->getArgs() << endl;
    }

}

HistoryCommand* HistoryCommand::clone() {
    HistoryCommand* ret = new HistoryCommand(getArgs(),history);
    return ret;
}

/**********Verbose******/

VerboseCommand::VerboseCommand(string args):BaseCommand(args) {}

void VerboseCommand::execute(FileSystem &fs) {
    int tmp = stoi(getArgs());
    if(tmp == 0 || tmp == 1|| tmp == 2 || tmp == 3){
        verbose = tmp;
    } else{
        std::cout << "Wrong verbose input" << endl;
    }

}

string VerboseCommand::toString() {return "verbose ";}

VerboseCommand* VerboseCommand::clone() {
    VerboseCommand* ret = new VerboseCommand(getArgs());
    return ret;
}

//--------------------ErrorCommand---working 2311-------

ErrorCommand::ErrorCommand(string args) :BaseCommand(args) {}
void ErrorCommand::execute(FileSystem & fs) {
    if (getArgs().find_first_of(" ") == std::string::npos) {
        std::cout << getArgs() << ": Unknown command" << endl;
    }
    else {
        std::cout << getArgs().substr(0, (getArgs().find_first_of(" "))) << ": Unknown command" << endl;
    }

}
string ErrorCommand::toString() {return "";}

ErrorCommand* ErrorCommand::clone() {
    ErrorCommand* ret = new ErrorCommand(getArgs());
    return ret;
}

//---------------------execCommand------working 2311--------

ExecCommand::ExecCommand(string args, const vector<BaseCommand *> & history) : BaseCommand(args), history(history) {}//constructor

void ExecCommand::execute(FileSystem & fs) {
    size_t i = stoi(getArgs());
    if(i < history.size()) {
        (history.at(i))->execute(fs);
    } else{
        std::cout << "Command not found"<<endl;
    }
}

string ExecCommand::toString() { return "exec "; }

ExecCommand* ExecCommand::clone() {
    ExecCommand* ret = new ExecCommand(getArgs(),history);
    return ret;
}
