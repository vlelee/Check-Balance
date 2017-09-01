#include <iostream>
#include <fstream>
#include <stack>
#include <cstring>
#include <list>
#include <string>
#include <sstream>

using namespace std;


stack<char, list<char> > stack1; // doubly linked list
int status = 0;

void clear(stack<char, list<char> > stack)
{
	while (!stack.empty())
	{
		stack.pop();
	}
}

bool matchbracket(char opening, char closing)
{

	if (opening == '(' && closing == ')')
		return true;
	else if (opening == '{' && closing == '}')
		return true;
	else if (opening == '[' && closing == ']')
		return true;
	else if (opening == '\"' && closing == '\"')
		return true;
	else if (opening == '\'' && closing == '\'')
		return true;
	else
		return false;
}

//findbracket will take a line and search char by char for each symbol
int findbracket(int status, int lnum, string line)
{
	int dcount = 0, scount = 0;
	size_t i = 0;

	while (i < line.length())
	{
		
		if (line[i] == '(' || line[i] == '[' || line[i] == '{')
		{
			stack1.push(line[i]);
		}

		else if ((line[i] == '\"'))
		{

			dcount += 1;

			if ((dcount % 2) != 0)
			{
				stack1.push(line[i]);
			}
			else if ((dcount % 2) == 0)
			{

				if (!matchbracket(stack1.top(), line[i]))
				{

					cout << "Error at line " << lnum << ": top()=" << stack1.top()
						<< ";current=" << line[i] << endl;
					status += 1;
				}
				else if (stack1.empty())
				{
					cout << "Error at line " << lnum << ": top()=UNDEFINED;current=" << line[i] << endl;
					status += 1;
				}
				else
				{
					stack1.pop();
				}
			}
		}

		else if ((line[i] == '\''))
		{
			scount += 1;

			if ((scount % 2) != 0)
			{
				stack1.push(line[i]);
			}
			else if ((scount % 2) == 0)
			{
				if (!matchbracket(stack1.top(), line[i]))
				{

					cout << "Error at line " << lnum << ": top()=" << stack1.top()
						<< ";current=" << line[i] << endl;
					status += 1;
				}
				else if (stack1.empty())
				{
					cout << "Error at line " << lnum << ": top()=UNDEFINED;current=" << line[i] << endl;
					status += 1;
				}
				else
				{
					stack1.pop();
				}
			}
		}
		else if (line[i] == ')' || line[i] == ']' || line[i] == '}')
		{
			if (!stack1.empty() && !matchbracket(stack1.top(), line[i]))
			{

				cout << "Error at line " << lnum << ": top()=" << stack1.top()
					<< ";current=" << line[i] << endl;
				status += 1;
			}
			else if (stack1.empty())
			{
				cout << "Error at line " << lnum << ": top()=UNDEFINED;current=" << line[i] << endl;
				status += 1;
			}
			else
			{
				stack1.pop();
			}
		}
		else{}
		i++;
	}
	if (dcount % 2 != 0)
	{
		if (stack1.top() == '\"'){
			stack1.pop();
		}
	}
	if (scount % 2 != 0)
	{
		if (stack1.top() == '\''){
			stack1.pop();
		}
	}
	return status;
}

//readfile will open a file and read it
void readfile(const char *filename)
{
	string line;
	int num = 0;
	int istatus = 0;
	int checklines = 0;

	ifstream infile;
	infile.open(filename);
	while (!infile.eof())
	{
		num++;
		getline(infile, line);

		status += findbracket(istatus, num, line);
		if (line.length() == 0)
		{
			checklines += 1;
		}
	}//end while loop

	if (!infile.is_open())
	{
		cerr << "Unable to open file" << endl;
	}

	infile.clear();
	infile.close();

	//cout << "status: " << status << endl;
	if (status == 0 && stack1.empty())
	{
		cout << "Correct" << endl;
	}
	else if (checklines == 0 && !stack1.empty())
	{
		cout << "Error at line " << (num + 1) << ": top()=" << stack1.top()
			<< ";current=$" << endl;
		stack1.pop();
	}
	else if (!stack1.empty())
	{
		if (line.length() > 0)
		{
			cout << "Error at line " << (num + 1) << ": top()=" << stack1.top()
				<< ";current=$" << endl;
			stack1.pop();
		}
		else
		{
			cout << "Error at line " << num << ": top()=" << stack1.top()
				<< ";current=$" << endl;
			stack1.pop();
		}
	}
}

int main(int argc, char *argv[])
{
	char delims[] = "=";
	char filename[40];
	char *pchar;
	char *file;
	//if two arguments are not provided at the command line
	if (argc != 2) 
	{
		cerr << "Please provide the required inputs." << endl;
		return 0;
	}
	//if two arguments are provided at the command line
	else if (argc > 0){

		strcpy(filename, argv[1]);
		pchar = strtok(filename, delims);

		if (pchar != NULL)
		{
			file = strtok(NULL, "=");
		}

		readfile(file);

		if (!stack1.empty())
		{
			clear(stack1);
		}
	}
	
	return 0;
}