#include "TextClassifier.h"

using namespace std;

int main()
{
	vector<TextClassifier>::iterator iter;
	vector<TextClassifier> classifiers;
	classifiers.push_back(TextClassifier("English"));
	classifiers.push_back(TextClassifier("Italian"));
	classifiers.push_back(TextClassifier("French"));
	classifiers.push_back(TextClassifier("Spain"));

	TextClassifier NewDoc;
	string filename;
		
	for (iter = classifiers.begin(); iter != classifiers.end(); iter++)
	{
		if (!(*iter).InitClassifier(*iter))
		{
			cout << "File learn_" << (*iter).getCategory() << " doesn't exist"<<endl;
			return 1;
		}
	}
	
	/*	Texts for testing:
	*	
	*	English : _En
	*	French :  _Fr
	*	Spain :   _Sp
	*	Italian:  _It
	*/

	cout << "Enter the filename :\n";
	cin >> filename;
	if (filename[filename.length()] != 't'
		&& filename[filename.length() - 1] != 'x'
		&& filename[filename.length() - 2] != 't'
		&& filename[filename.length() - 3] != '.')
	{
		filename += ".txt";
	}
	ifstream file(filename);
	
	if (!file.is_open()) 
	{
		cout << "File doesn't exist!" << endl;
	}
	else 
	{
		NewDoc.learn(file);
		NewDoc = NewDoc.Calculate(classifiers, NewDoc);
		cout << "File classification :  " << NewDoc.getCategory() << endl;
	}	
	system("pause");
	return 0;
}

