#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<algorithm>
using namespace std;
map<string, int> clicks;
map<string, double> ctr(map<string,double> impressions);
vector<string> search(string keyword);
void fillClicksmap();
void updateclicks(string site);
void sort(map<string, double>& score);
map<string, double> imp();
map<string, double> pagerank();
map<string, double> score(map<string, double> ctr, map<string, double>impressions, map<string, double>pagerank);
bool cmp(pair<string, double>& a, pair<string, double>& b);
int main()
{
	fillClicksmap();
	string word;
	string site;
	int key,k;
	vector<string> r;
	map<string, double> i = imp();
	map<string, double> p = pagerank();
	map<string, double> c = ctr(i);
	map<string, double> s = score(c, i, p);
	while (true)
	{
		cout << "welcome!" << endl;
		cout << "would you like to do" << endl;
		cout << "1. New search" << endl;
		cout << "2. exit" << endl;
		cin>>key;
		cin.ignore(1000, '\n');
		if (key == 2)
			break;
		else if (key == 1)
		{
			cout << "please enter the words you want to search for: " << endl;
			getline(cin, word);
			r=search(word);
			cout << "search results: " << endl;
			for (int i = 0; i < r.size(); i++)
				cout<<i+1<<". " << r[i] << endl;
			cout << "would you like to " << endl;
			cout << "1. choose a webpage to open" << endl;
			cout << "2. new search" << endl;
			cout << "3. exit" << endl;
			cin >> k;
			if (k == 3)
			break;
			if (k == 2)
			{
				cin.ignore(1000, '\n');
				cout << "please enter the words you want to search for: " << endl;
				getline(cin, word);
				r = search(word);
			}
			if (k == 1)
			{
				cin.ignore(1000, '\n');
				cout << "enter the webpage you want: " << endl;
				getline(cin, site);
				updateclicks(site);
				cout << "you are now viewing " << site << endl;
				break;
			}
		}
	}
	cout << "the webpages sorted according to their scores: " << endl;
	for (auto x : s)
		cout << x.first<<endl;
}
vector<string> search(string keyword)
{
	vector<string> results;
	vector<string> r;
	string l;
	ifstream cin;
	vector<string> url, kw;
	cin.open("keyword.csv");
	while (!cin.eof()) {
		getline(cin, l, ',');
		r.push_back(l);

	}
	map<string, vector<string>> keywords;
	for (int i = 0; i < r.size(); i++)
	{
		if (r[i].rfind("www", 0) == 0)
			url.push_back(r[i]);
		else {
			if (url.size() == 1)
				keywords[url[0]].push_back(r[i]);
			else if (url.size() == 2)
				keywords[url[1]].push_back(r[i]);
			else if (url.size() == 3)
				keywords[url[2]].push_back(r[i]);
		}
		
	}
	map<string, vector<string>>::iterator it;
	if (keyword.find("AND") != -1)
	{
		size_t pos = keyword.find("AND");
		string s1 = keyword.substr(0, pos-1);
		string s2 = keyword.substr(pos + 4);
		
		for (auto x : keywords)
		{
			it = keywords.find(x.first);
			vector<string> s = it->second;
			if ((find(s.begin(), s.end(), s1) != s.end()) && (find(s.begin(), s.end(), s2) != s.end()))
			{
				results.push_back(it->first);
			}
		}
	}
	else if ((keyword.find("OR") != -1) || ((keyword.find('"') == -1) && (keyword.find("AND") == -1)))
	{
		size_t pos;
		string s1;
		string s2;
		if (keyword.find("OR") != -1)
		{
			pos = keyword.find("OR");
			 s1 = keyword.substr(0, pos - 1);
			 s2 = keyword.substr(pos + 3);

		}
		else if (keyword.find(" ") != -1)
		{
			pos = keyword.find(" ");
			 s1 = keyword.substr(0, pos);
			 s2 = keyword.substr(pos + 1);
		}
		
		for (auto x : keywords)
		{
			it = keywords.find(x.first);
			vector<string> s = it->second;
			if ((find(s.begin(), s.end(), s1) != s.end()) || (find(s.begin(), s.end(), s2) != s.end()))
			{
				results.push_back(it->first);
			}
		}
	}
	else
	{
		for (auto x : keywords)
		{
			it = keywords.find(x.first);
			vector<string> s = it->second;
			keyword.erase(remove(keyword.begin(), keyword.end(), '"'), keyword.end());
			if (find(s.begin(), s.end(), keyword) != s.end())
			{
				results.push_back(it->first);
			}
		}
	}
	return results;
}
void fillClicksmap()
{
	vector<string>url;
	vector<string> r;
	string l;
	ifstream cin;
	cin.open("numberofclicks.csv");
	while (!cin.eof()) {
		getline(cin, l, ',');
		r.push_back(l);

	}
	for (int i = 0; i < r.size(); i++)
	{
		if (i % 2 == 0)
			url.push_back(r[i]);
	}
	for (int i = 0; i <3; i++)
		clicks.insert(pair<string, int>(url[i], 0));

}
void updateclicks(string site)
{
	map<string, int>::iterator it;
	it = clicks.find(site);
	it->second++;
}
map<string, double> imp()
{
	vector<double> n;
	vector<string>url;
	vector<string> r;
	string l;
	ifstream cin;
	vector<double>imp, cl;
	map<string, double> impressions;
	cin.open("numberofclicks.csv");
	while (!cin.eof()) {
		getline(cin, l, ',');
		r.push_back(l);

	}
	for (int i = 0; i < r.size(); i++)
	{
		if (i % 2 == 0)
			url.push_back(r[i]);
		else
			n.push_back(stoi(r[i]));
	}
	for (int i = 0; i < 3; i++)
		impressions.insert(pair<string, double>(url[i], n[i]));
	return impressions;
}
map<string,double> ctr(map<string,double> impressions)
{
	vector<double>imp;
	vector<int> cl;
	vector<string>url;
	map<string, double> ctr;
	for (auto& mapping : impressions)
	{
		url.push_back(mapping.first);
		imp.push_back(mapping.second);
	}
	for (auto& m : clicks)
		cl.push_back(m.second);
	for (int i = 0; i <url.size(); i++)
		ctr.insert(pair<string, double>(url[i], (cl[i]/imp[i])));
	return ctr;
}
map<string, double> pagerank()
{
	vector<string>row, destination;
	vector<string> source;
	vector<string>::iterator ip;
	map <string, vector<string>> g;
	map <string, double> pagerank;
	vector<string> PR;
	string line;
	ifstream fin;
	fin.open("webgraph.csv");
	while (!fin.eof()) {
		getline(fin, line, ',');
		row.push_back(line);
	}
	for (int i = 0; i < row.size(); i++)
	{
		if (i % 2 == 0 && g.find(row[i]) == g.end())
			source.push_back(row[i]);
		else {
			if (source.size() == 1)
				g[source[0]].push_back(row[i]);
			else if (source.size() == 2)
				g[source[1]].push_back(row[i]);
			else if (source.size() == 3 && row[i - 1] == source[2])
				g[source[2]].push_back(row[i]);
			else if (source.size() == 3 && row[i - 1] == source[1])
				g[source[1]].push_back(row[i]);
			else if (source.size() == 3 && row[i - 1] == source[0])
				g[source[0]].push_back(row[i]);
		}
	}
	for (auto& mapping : g) {
		PR.push_back(mapping.first);
	}
	double s = 4.0;
	map<string, vector<string>>::iterator it2;
	vector <double> oldPR;
	vector <double> rank;
	double d = 0.85;
	int count = -1;
	int count2 = 0;
	int count3 = 0;
	double rankStep;
	for (int i = 0; i < 4; i++)
		oldPR.push_back(0.0);
	for(int i=0;i<100;i++){
		count = -1;
	for (auto& x : g)
	{
		count2 = 0;
		count++;
		if (count3 == 0)
			rankStep = 0.25;
		else
			rankStep = oldPR[count];
		
			for (auto& it : x.second)
			{
				it2 = g.find(x.first);
				int size = it2->second.size();
				if (count2 == 0&& count3==0)
					rank.push_back((d * rankStep) / size);
				else
					rank[count] = rank[count] + ((d * rankStep) / size);
				count2++;
			}

			rank[count] = rank[count] + ((1 - d) / 4);
				oldPR[count] = rank[count];
		
		}
	count3++;
	}
	for (int i = 0; i < rank.size(); i++)
		pagerank.insert(pair<string, double>(PR[i], rank[i]));
	return pagerank;
}
map<string, double> score(map<string, double> ctr, map<string, double>impressions, map<string, double>pagerank)
{
	vector<double> s, p;
	vector<double> im;
	map<string, double> S;
	double pagescore;
	vector<string> urls;
	for (auto c : ctr)
		s.push_back(c.second);
	for (auto I : impressions)
	{
		urls.push_back(I.first);
		im.push_back(I.second);
	}
	for (auto P : pagerank)
		p.push_back(P.second);
	for (int i = 0; i < s.size(); i++)
	{
		pagescore = 0.4 * p[i] + (((1 - (0.1 * im[i]) / (1 + 0.1 * im[i])) * p[i] + ((0.1 * im[i]) / (1 + 0.1 * im[i])) * s[i])) * 0.6;
		S.insert(pair<string, double>(urls[i],pagescore));
	}
	sort(S);
	return S;
}
void sort(map<string, double>& score)
{
	vector<pair<string, double> > A; 
	for (auto& it : score) {
		A.push_back(it);
	}
	sort(A.begin(), A.end(), cmp);
}
bool cmp(pair<string, double>& a, pair<string, double>& b)
{
	return a.second > b.second;
}