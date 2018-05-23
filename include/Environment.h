#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
private:
	vector<BaseCommand*> commandsHistory;
	FileSystem fs;
	void clone(FileSystem,vector<BaseCommand*>);
	void clear();

public:
	Environment();//constructor
	void start();

	Environment(const Environment&);//copy constructor
	Environment& operator=(const Environment &other);//Copy Assignment operator
	Environment(Environment &&other);//move constructor
	Environment& operator=(Environment &&other);// Move Assignment
	virtual ~Environment();//Destructor

	FileSystem& getFileSystem(); // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
};

#endif