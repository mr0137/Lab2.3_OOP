#include "TextClassifier.h"
regex Error("(0|1|2|3|4|5|6|7|8|9|,|.|\|:|-|/)");

#pragma warning(disable:4996)

TextClassifier::TextClassifier()
{
	_category = "unknown";
}

TextClassifier::TextClassifier(const string category)
{
	_category = category;
}

bool compare(pair<string, int> value1, pair<string, int> value2) 
{
	return value1.second > value2.second;
}

void TextClassifier::learn(istream& in) 
{
	vector<string>::iterator iter;
	map<string, int>::iterator iter_;
	vector<string> NGrams;
	map<string, int> NGram_map;
	string Temp;

	const char* SingleChar = ".,:;!?+*/|";

	while (in >> Temp) 
	{
		Temp = strtok((char*)Temp.c_str(), SingleChar);
		//if (regex_search((const char*)Temp.c_str(), Error))//защита от мусора
		{
			NGrams = makeNGrams(Temp);
		}

		for (iter = NGrams.begin(); iter != NGrams.end(); iter++)
		{
			if (NGram_map.size() >= CATEGORY_LEN) 
			{
				iter_ = NGram_map.find(*iter);
				if (iter_ != NGram_map.end())
					NGram_map[*iter]++;
			}
			else
			{
				NGram_map[*iter]++;
			}
		}
		Temp.clear();
	}
	for (map<string, int>::iterator iter = NGram_map.begin() ; iter != NGram_map.end(); iter++)
	{
		_frequencyMap.push_back(*iter);
	}
	sort(_frequencyMap.begin(), _frequencyMap.end(), compare);
}

vector<string> TextClassifier::makeNGrams(const string& str) 
{
	int i = 0, j, length = 0;
	string Temp = "_";
	vector<string> NGrams;

	for (i; i < str.length() ; i++) 
	{
		for (j = i ; j < str.length() + UPPER_NGRAM_LEN; j++) 
		{

			if (j < str.length())
			{
				Temp+= str[j];
			}
			else
			{
				Temp += "_";
			}
			length = Temp.length();
			if (length < LOWER_NGRAM_LEN) continue;
			if (length > UPPER_NGRAM_LEN) break;
			NGrams.push_back(Temp);	
		}
		Temp.clear();
	}
	return NGrams;
}

void TextClassifier::dumpProfile(ostream& out) const 
{
	out << _category << endl;
	for (int i = 0; i < (int)_frequencyMap.size(); i++)
	{
		out << _frequencyMap[i].first << " " << _frequencyMap[i].second << endl;
	}
}

void TextClassifier::learnFromDump(istream& in) 
{
	string category, NGram, counts;
	in >> category;
	_category = category;
	while (in >> NGram) 
	{
		in >> counts;
		_frequencyMap.push_back(pair<string,int> (NGram,stoi(counts)));
	}
}

long int TextClassifier::score(const TextClassifier& classifier) const 
{
	int quantity = 0, i, j;
	bool bool_ = false;
	for (i = 0; i <  (int)_frequencyMap.size(); i++) 
	{
		for (j = 0; j < (int)classifier._frequencyMap.size(); j++) 
		{
			if (!strcmp(_frequencyMap[i].first.c_str(),classifier._frequencyMap[j].first.c_str())) 
			{
				quantity+= abs(i - j);
				bool_ = true;
				break;
			}
		}
		if (bool_ == false)
		{
			quantity += MAX_SCORE;
		}
		bool_ = false;
	}
	return quantity;
}

bool TextClassifier::InitClassifier(TextClassifier & classifier)
{
	string name = "learn_" + classifier.getCategory() + ".txt";
	string dump = "dump_" + classifier.getCategory() + ".txt";

	ifstream fileIn(dump);
	if (fileIn.is_open())
	{
		classifier.learnFromDump(fileIn);
	}
	else 
	{
		fileIn.open(name);
		if (!fileIn.is_open())
		{
			return false;
		}
		classifier.learn(fileIn);
		ofstream fileOut(dump);
		classifier.dumpProfile(fileOut);
		fileOut.close();
	}
	fileIn.close();
	return true;
}

TextClassifier TextClassifier::Calculate(vector<TextClassifier>& classifiers, TextClassifier NewDoc)
{
	vector<long int> quantity;
	int min = 0, i = 0;
	vector<TextClassifier>::iterator iter;
	for (iter = classifiers.begin(); iter != classifiers.end(); iter++)
	{
		quantity.push_back((*iter).score(NewDoc));
	}

	for (i = 0; i < quantity.size(); i++)
	{
		if (quantity[i] < quantity[min])
			min = i;
	}
	NewDoc.setCategory(classifiers[min].getCategory());
	return NewDoc;
}
