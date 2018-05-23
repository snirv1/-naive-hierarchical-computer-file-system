#ifndef FILES_H_
#define FILES_H_
#include "GlobalVariables.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
using namespace std;


/*#define SV_DEBUG(msg) do { \
	cout << "SV DEBUG " << __FILE__ << ":" << __FUNCTION__ \
  << ":" << __LINE__ \
	<< " (" << msg << ")" << endl; } while(0);
*/

#define SV_DEBUG(msg)




class BaseFile {
private:
	string name;


public:
	BaseFile(string name);

	BaseFile();

	virtual ~BaseFile();//Destructor
	string getName() const;
	void setName(string newName);
	virtual int getSize() = 0;
	bool nameCheck();
    virtual string getType() = 0;
	virtual BaseFile* clone(BaseFile*)=0;

	
};

class File : public BaseFile {
private:
	int size;


public:
    virtual ~File();
	File(string name, int size); // Constructor
	int getSize(); // Return the size of the file
    virtual string getType();
	virtual BaseFile * clone(BaseFile*);

};

class Directory : public BaseFile {
private:
	vector<BaseFile*> children;
	Directory *parent;


public:
	Directory(string name, Directory *parent); // Constructor

	Directory(const Directory&);// Copy Constructor
    virtual ~Directory();//Destructor
	Directory& operator=(const Directory &other);//Copy Assignment operator
	Directory(Directory &&other);//move constructor
	Directory& operator=(Directory &&other);// Move Assignment

	Directory *getParent() const; // Return a pointer to the parent of this directory
	void setParent(Directory *newParent); // Change the parent of this directory
	void addFile(BaseFile* file); // Add the file to children
	void removeFile(string name); // Remove the file with the specified name from children
	void removeFile(BaseFile* file); // Remove the file from children
	void sortByName(); // Sort children by name alphabetically (not recursively)
	void sortBySize(); // Sort children by size (not recursively)
	vector<BaseFile*> getChildren(); // Return children
    int getSize() override; // Return the size of the directory (recursively)
	string getAbsolutePath();  //Return the path from the root to this
	string getAbsolutePathHelp();//Check if it was the root and returns "/"
    string getType() override;
	BaseFile* clone(BaseFile* parent) override;//deep copy of a dir
	void clear();//delete all the members of dir


	
};

#endif
