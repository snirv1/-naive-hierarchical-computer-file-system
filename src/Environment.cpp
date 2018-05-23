#include "../include/Environment.h"
#include <iostream>

using namespace std;



//****constructor******
Environment::Environment():commandsHistory() ,fs(FileSystem()) {}//TODO sv

//destructor
Environment::~Environment() {
    if(verbose == 1 || verbose == 3){
        cout <<"Environment::~Environment()"<< endl;
    }
    clear();
}

//copy constructor
Environment::Environment(const Environment &other):commandsHistory(),fs() {
    if(verbose == 1 || verbose == 3){
        cout <<"Environment::Environment(const Environment &)"<< endl;
    }
    vector<BaseCommand*> otherCmdH = other.commandsHistory;
    FileSystem otherFs = other.fs;
    clone(otherFs,otherCmdH);

}

//copy assignment
Environment& Environment::operator=(const Environment &other) {
    if (verbose == 1 || verbose == 3) {
        cout << "Environment& Environment::operator=(const Environment &other)" << endl;
    }
    if (this != &other) {


        clear();
        clone(other.fs, other.commandsHistory);
    }return *this;
}

//move constructor
Environment::Environment(Environment &&other): commandsHistory(other.commandsHistory), fs(other.fs) {
    if(verbose == 1 || verbose == 3){
        cout <<"Environment::Environment(Environment &&other)"<< endl;
    }
}

//move assignment
Environment& Environment::operator=(Environment &&other) {
    if(verbose == 1 || verbose == 3){
        cout <<"Environment& Environment::operator=(Environment &&other)"<< endl;
    }
    if (this != &other)
    {
        clear();
        fs = other.fs;
        commandsHistory = other.commandsHistory;
        other.clear();
    }

    return *this;
}




//----------methods-------------



void Environment::start() {
    string input = "";
    while (input != "exit") {
        SV_DEBUG("")
        std::cout << (fs.getWorkingDirectory()).getAbsolutePath() << ">";
        getline(cin, input);
        if (input.find_first_of(" ") == std::string::npos) {
            if (input == "exit") {}
            else if (input == "history") {
                HistoryCommand* history = new HistoryCommand("",getHistory());
                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }
                (*history).execute(getFileSystem());
                addToHistory(history);

            }
            else if (input == "pwd") {
                PwdCommand* pwd = new PwdCommand("");
                addToHistory(pwd);
                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }
                (*pwd).execute(getFileSystem());
            }
            else if (input == "ls") {
                LsCommand* ls = new LsCommand("");
                addToHistory(ls);
                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*ls).execute(getFileSystem());
                SV_DEBUG("")

            }
            else {
                ErrorCommand* error = new ErrorCommand(input);
                addToHistory(error);

                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*error).execute(getFileSystem());

            }
        }
        else {
            string args = input.substr(input.find_first_of(" ") + 1);
            string command = input.substr(0, input.find_first_of(" "));
            if (command == "exit") {}


        else if (command == "cd") {
            CdCommand* cd = new CdCommand(args);
            addToHistory(cd);

                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*cd).execute(getFileSystem());

        }
        else if (command == "ls") {
            LsCommand* ls = new LsCommand(args);
            addToHistory(ls);

                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*ls).execute(getFileSystem());
                SV_DEBUG(""<< fs.getRootDirectory().getName() )

        }
        else if (command == "mkdir") {
            MkdirCommand* mkdir = new MkdirCommand(args);
            addToHistory(mkdir);

                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*mkdir).execute(getFileSystem());


        }
        else if (command == "mkfile") {
            MkfileCommand* mkfile = new MkfileCommand(args);
            addToHistory(mkfile);

                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*mkfile).execute(getFileSystem());

        }
        else if (command == "cp") {
            CpCommand* cp = new CpCommand(args);
            addToHistory(cp);

                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*cp).execute(getFileSystem());

        }
        else if (command == "mv") {
            MvCommand* mv = new MvCommand(args);
                addToHistory(mv);

                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*mv).execute(getFileSystem());

        }
        else if (command == "rename") {
            RenameCommand* rename = new RenameCommand(args);
                addToHistory(rename);

                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*rename).execute(getFileSystem());

        }
        else if (command == "rm") {
            RmCommand* rm = new RmCommand(args);
                addToHistory(rm);

                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                } (*rm).execute(getFileSystem());

        }
        else if (command == "verbose") {
            VerboseCommand* verbose = new VerboseCommand(args);
                addToHistory(verbose);
                (*verbose).execute(getFileSystem());
              //TODO verbose sv
        }
        else if (command == "exec") {
            ExecCommand* exec = new ExecCommand(args,getHistory());
                addToHistory(exec);

                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*exec).execute(getFileSystem());

        }
        else {
                ErrorCommand* error = new ErrorCommand(input);
                addToHistory(error);
                if(verbose == 2 || verbose == 3){
                    std::cout << input << endl;
                }(*error).execute(getFileSystem());

        }
    }
        SV_DEBUG("")
}
    SV_DEBUG("")
}

FileSystem& Environment::getFileSystem()  { return fs;}

void Environment::addToHistory(BaseCommand *command) { (commandsHistory).push_back(command);}

const vector<BaseCommand*>& Environment::getHistory() const {return commandsHistory;}

void Environment::clear() {
    for (std::vector<BaseCommand*>::iterator it = commandsHistory.begin(); it != commandsHistory.end(); ++it){
        delete *it;
        (*it) = nullptr;
    }
    commandsHistory.clear();
}


void Environment::clone(FileSystem  otherFs, vector<BaseCommand *> otherCmdH) {
    fs =  otherFs;
    for(size_t i = 0;i < otherCmdH.size();i++ ){
        BaseCommand* cmd = otherCmdH[i]->clone();
        commandsHistory.push_back(cmd);
    }

}

