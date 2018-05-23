#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <string>
#include "FileSystem.h"
#include "GlobalVariables.h"
#include <vector>


class BaseCommand {
private:
	string args;

public:
	BaseCommand(string args);
	string getArgs();
	virtual void execute(FileSystem & fs) = 0;
	virtual string toString() = 0;
	vector<string> breakDownPath(string args);
	Directory* getDirectory(vector<string> pathV, Directory& workingD,FileSystem& fs);
	Directory* getDirectory(vector<string> pathV, Directory& workingD,FileSystem& fs,int a);//TODO delete after create new get directory
    bool getDirectoryFileInpath(vector<string> pathV, Directory& workingD,FileSystem& fs);
	BaseFile* isChild(string name , Directory* directory);
	virtual ~BaseCommand();
	virtual BaseCommand* clone() = 0;
    vector<string> getActualPath(FileSystem& fs,  vector<string>& path);

};

class PwdCommand : public BaseCommand {
private:
public:
	PwdCommand* clone();

	PwdCommand(string args);
	void execute(FileSystem & fs); // Every derived class should implement this function according to the document (pdf)
	virtual string toString();
};

class CdCommand : public BaseCommand {
private:
public:
	CdCommand* clone();

	CdCommand(string args);
	void execute(FileSystem & fs);
	string toString();
//	string getNextDir(string args);
//	string getRestOfPath(string args);
//	Directory* isChild(string name, Directory directory);
};

class LsCommand : public BaseCommand {
private:
public:
	LsCommand* clone();

	LsCommand(string args);
	void execute(FileSystem & fs);
	string toString();
};

class MkdirCommand : public BaseCommand {
private:
public:
	MkdirCommand* clone();

	MkdirCommand(string args);
	void execute(FileSystem & fs);
	string toString();

    vector<string> getMaximalMatchingPath(FileSystem &fs, const vector<string>& path);

};

class MkfileCommand : public BaseCommand {
private:
public:
	MkfileCommand* clone();

	MkfileCommand(string args);
	void execute(FileSystem & fs);
	string toString();
};

class CpCommand : public BaseCommand {
private:
public:
	CpCommand* clone();

	CpCommand(string args);
	void execute(FileSystem & fs);
	string toString();
};

class MvCommand : public BaseCommand {
private:
public:
	MvCommand* clone();

	MvCommand(string args);
	void execute(FileSystem & fs);
	string toString();
    bool isAfatherOfWd(FileSystem &fs,Directory& parent);
};

class RenameCommand : public BaseCommand {
private:
public:
	RenameCommand* clone();

	RenameCommand(string args);
	void execute(FileSystem & fs);
	string toString();
};

class RmCommand : public BaseCommand {
private:
public:
	RmCommand* clone();

	RmCommand(string args);
	void execute(FileSystem & fs);
	string toString();
};

class HistoryCommand : public BaseCommand {
private:
	const vector<BaseCommand *> & history;
public:
	HistoryCommand* clone();

	HistoryCommand(string args, const vector<BaseCommand *> & history);
	void execute(FileSystem & fs);
	string toString();
};


class VerboseCommand : public BaseCommand {
private:
public:
	VerboseCommand* clone();

	VerboseCommand(string args);
	void execute(FileSystem & fs);
	string toString();
};

class ErrorCommand : public BaseCommand {
private:
public:
	ErrorCommand* clone();

	ErrorCommand(string args);
	void execute(FileSystem & fs);
	string toString();
};

class ExecCommand : public BaseCommand {
private:
	const vector<BaseCommand *> & history;
public:
	ExecCommand* clone();

	ExecCommand(string args, const vector<BaseCommand *> & history);
	void execute(FileSystem & fs);
	string toString();
};


#endif
