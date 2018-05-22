#pragma  once
#include<map>
#include<vector>
#include<algorithm>
#include<sstream>
#include<iostream>
#include<fstream>
#include<cmath>
#include <regex>



using namespace std;

#define MAX_SCORE 150

#define LOWER_NGRAM_LEN	2
#define UPPER_NGRAM_LEN	5
//Довжина профілів категорій текстів, що розпізнаються
#define CATEGORY_LEN 1000

typedef vector<pair<string, int>> frequency_map ;

class TextClassifier
{
public:
	/* Nameof category defaults to 'Unknown.' */
	TextClassifier() ;
	~TextClassifier() {};
	/*Param classification – name of the category (e.g., "English").    */
	TextClassifier(const string category);
	/* returns the map N-grams and their frequencies.     */
	frequency_map freqs() const { return _frequencyMap; }
	/* returns the name of the classifier.*/
	string getCategory() const { return _category; }
	/*Param classification – name of the category.*/
	void setCategory(const string& cat) { _category = cat; }
	/*learns the frequencies of N-grams in a corpus.*/
	void learn(istream& in);
	/*dumps the frequencies of N-grams in a corpus.
	File structure is as follows:
	* First line of the file - classification
	* lines below :  N-grams + their quantity 
	* N-gram and it quantity separated with space or tab*/
	void dumpProfile(ostream& out) const;
	/*learns the frequencies of N-grams from a file createdby.
	dumpProfile() function */
	void learnFromDump(istream& in);
	/*returns distance measure between 2 TextСlassifiers*/
	long int score(const TextClassifier& classifier) const;
	/*Initialization Category samples*/
	bool InitClassifier(TextClassifier& classifier);
	/*Function for calculating what type of text*/
	TextClassifier Calculate(vector<TextClassifier>& classifiers, TextClassifier NewDoc);

private:
	vector<string> makeNGrams(const string& str);
	string _category;
	frequency_map _frequencyMap;
};

