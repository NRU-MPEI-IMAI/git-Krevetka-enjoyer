#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "string"
#include "Perehod.h"
#include "vector"
#include "fstream"
#include <iostream>
#include "unordered_set"
#include "set"
#include <algorithm>
#include <codecvt>

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
typedef std::unordered_set<string> set;
typedef std::unordered_map<string,string> smena;
typedef std::unordered_map<string, Perehod> tabl;
typedef std::pair<string, string> nk;
using std::endl;

class Aftomat
{
	vector<char> alf;
	vector<string> ver;
	string nach;
	vector<string> con;
	tabl p;

	std::string UnicodeToUTF8(const std::wstring& wstr) const
	{
		std::string ret;
		try {
			std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
			ret = wcv.to_bytes(wstr);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		return ret;
	}
	bool plov(const char& x) const
	{
		for (int i = 0; i < alf.size(); ++i)
		{
			if (x == alf[i])
				return true;
		}
		return false;
	}
	string MakeNaz(int& i) const
	{
		string a(1, 97 + i / 10);
		a+= 48 + i% 10;
		return a;
	}
	string MakeVer(int& i) 
	{
		++i;
		ver.push_back(MakeNaz(i));
		p[ver[i]]= Perehod(ver[i], alf);
		return ver[i];
	}
	nk makeaft(const string& x,int& i)
	{
		vector<string> stack;
		int s = 0;
		stack.push_back("");
		int k = 0;
		for (int j = 0; j < x.length(); ++j)
		{
			if ((s == 0) && (x[j] == '+'))
			{
				++k;
				stack.push_back("");
			}
			else
			{
				if (x[j] == 40)
					++s;
				else if (x[j] == 41)
					--s;
				else if ((x[j] > 96) && (x[j] < 123))
					if (!plov(x[j]))
						alf.push_back(x[j]);
				stack[k] += x[j];
			}
		}
		if (stack.size() > 1)
		{
			string o= MakeVer(i);
			string y = MakeVer(i);;
			nk a("", "");
			for (int j = 0; j < stack.size(); ++j)
			{
				a = makeaft(stack[j], i);
				p[o].Dob('~', a.first);
				p[a.second].Dob('~', y);
			}
			return nk(o, y);
		}
		else
		{
			string o = MakeVer(i);
			string y = o;
			nk a("", "");
			for (int j = 0; j < x.length(); ++j)
			{
				char w = x[j];
				if (w == '(')
				{
					string u;
					++j;
					int s = 1;
					for (; s > 0; ++j)
					{
						if (x[j] == 40)
							++s;
						else if (x[j] == 41)
							--s;
						if (s!=0)
						u += x[j];
					}
					--j;
					if (((j + 1 < x.length()) && (x[j + 1] != '*'))||(j + 1 >= x.length()))
					{
						a = makeaft(u, i);
						p[o].Dob('~', a.first);
						o = a.second;
					}
					if ((j + 1 < x.length()) && ((x[j + 1] == '*')|| (x[j + 1] == '^')))
					{
						a = makeaft(u, i);
						p[o].Dob('~', a.first);
						string k = MakeVer(i);
						p[o].Dob('~', k);
						p[a.second].Dob('~', k);
						p[a.second].Dob('~', a.first);
						o = k;
						++j;
					}
				}
				else if ((w > 96) && (w < 123))
				{
					if (((j + 1 < x.length()) && (x[j + 1] != '*')) || (j + 1 >= x.length()))
					{
						string k=MakeVer(i);
						p[o].Dob(w, k);
						o = k;
					}
					if ((j + 1 < x.length()) && ((x[j + 1] == '*') || (x[j + 1] == '^')))
					{
						string k = o;
						p[o].Dob(w, k);
						++j;
					}
				}
			}
			return nk(y, o);
		}
	}
	string Clean(const string& x) const
	{
		set a;
		string s;
		for (int i = 0; i < x.length(); ++i)
			if (x[i] == ',')
			{
				if (s!="")
					a.insert(s);
				s = "";
			}
			else
				s += x[i];
		if (s != "")
			a.insert(s);
		vector<string> q;
		if (a.empty())
		{
			return "";
		}
		else
		{
			for (auto it = a.begin(); it != a.end(); ++it)
				q.push_back(*it);
			std::sort(q.begin(), q.end(), [](const string& a, const string& b) { return a < b; });
			s = q[0];
			for (int i = 1; i < q.size(); ++i)
				s += ',' + q[i];
			return s;
		}
	}
	string UltraShag(const string& a,const char& x) const
	{
		if (x == '~')
		{
			string w = "";
			string y = "";
			for (int i = 0; i < a.length(); ++i)
				if (a[i] == ',')
				{
					w += ',' + p.at(y).Shag(x);
					y = "";
				}
				else
					y += a[i];
			if (y!="")
				w += ',' + p.at(y).Shag(x);
			w = Clean(w);
			if (w == "")
				return a;
			else if (a != w)
				return Clean(a+','+ UltraShag(w, x));
			else
				return a;
		}
		else
		{
			string w = "";
			string y = "";
			string z = "";
			for (int i = 0; i < a.length(); ++i)
				if (a[i] == ',')
				{
					w += ',' + p.at(y).Shag(x);
					z += ',' + p.at(y).Shag('~');
					y = "";
				}
				else
					y += a[i];
			if (y != "")
			{
				w += ',' + p.at(y).Shag(x);
				z += ',' + p.at(y).Shag('~');
			}
			y = Clean(w + ',' + z);
			if (y == "")
				return "";
			else if (a != Clean(w + ',' + z))
			{
				w = UltraShag(Clean(w), '~');
				z = UltraShag(Clean(z), x);
				return Clean(w + ',' + z);
			}
			else
				return a;
		}
	}
	bool vhod(const string& a,const string& x) const
	{
		string d;
		for (int i = 0; i < a.length(); ++i)
			if (a[i] == ',')
			{
				if (x == d)
					return true;
				d = "";
			}
			else
				d += a[i];
		if (x == d)
			return true;
		else
			return false;
	}
	vector<int> Ekv(string& a, vector<set>& x) const
	{
		vector<int> b;
		for (int i = 0; i < alf.size(); ++i)
		{
			string y = p.at(a).Shag(alf[i]);
			b.push_back(0);
			for (; b[i] < x.size(); ++b[i])
				if (x[b[i]].count(y) == 1)
					break;
		}
		return b;
	}
	bool plov(const string& a,const vector<int>& b,const vector<set>& x) const
	{
		for (int i = 0; i < alf.size(); ++i)
		{
			string y = p.at(a).Shag(alf[i]);
			if (b[i] == x.size())
			{
				if (y != "")
					return false;
			}
			else if (x[b[i]].count(y) == 0)
				return false;
		}
		return true;
	}
	bool plov(const vector<nk>& x,const nk& y) const
	{
		for (int i = 0; i < x.size(); ++i)
		{
			if (y == x[i])
				return true;
		}
		return false;
	}
	void tablica(tabl& tab, vector<string>& v,string& a)
	{
		if (tab.find(a)==tab.end())
		{
			v.push_back(a);
			tab[a] = Perehod(a, alf);
			for (int i = 0; i < alf.size(); ++i)
			{
				string s = UltraShag(a, alf[i]);
				tab[a].Dob(alf[i], s);
				if (s!="")
					tablica(tab,v, s);
			}
		}
	}
	vector<char> ObAlf(const vector<char>& a) const
	{
		vector<char> al;
		for (int i = 0; i < a.size(); ++i)
			al.push_back(alf[i]);
		for (int i = 0; i < a.size(); ++i)
		{
			bool x = false;
			for (int j = 0; (j < alf.size()) && (!x); ++j)
				x = (alf[j] == a[i]);
			if (!x)
				al.push_back(a[i]);
		}
		return al;
	}
	void Obtab(const Aftomat& a,const vector<char>& al, tabl& y, vector<string>& x) const
	{
		vector<nk> s;
		s.push_back(make_pair(nach, a.nach));
		for (int i = 0; i < s.size(); ++i)
		{
			string t = s[i].first + ',' + s[i].second;
			y[t] = Perehod(t, al);
			for (int j = 0; j < al.size(); ++j)
			{
				string e = p.at(s[i].first).Shag(al[j]);
				string w = a.p.at(s[i].second).Shag(al[j]);
				if ((e != "") && (w != ""))
				{
					y[t].Dob(al[j], e + ',' + w);
					if (!plov(s, make_pair(e, w)))
						s.push_back(make_pair(e, w));
				}
			}
			x.push_back(s[i].first + ',' + s[i].second);
		}
	}
	int kol(string& x) const
	{
		if (x == "")
			return 0;
		else
		{
			int k = 1;
			for (int i = 0; i < x.length(); ++i)
				if (x[i] == ',')
					++k;
			return k;
		}
	}
	string Naz(string& x,int& i) const
	{
		string y = "";
		for (; i < x.length(); ++i)
			if (x[i] == ',')
			{
				++i;
				return y;
			}
			else
				y += x[i];
		return y;
	}
	bool svinka(const Aftomat& a, smena::iterator it, smena& x) const
	{
		for (int i = 0; i < alf.size(); ++i)
		{
			string q = p.at(it->first).Shag(alf[i]);
			string w = a.p.at(it->second).Shag(alf[i]);
			if (((q == "") && (w != "")) || ((q != "") && (w == "")))
				return false;
			if (x.find(q)!=x.end())
			{
				if (x.at(q) != w)
					return false;
			}
			else if (!svinka(a, x.insert(make_pair(q, w)).first, x))
				return false;
		}
		string ql = "";
		ql+= p.at(it->first).Shag('~');
		string wl = "";
		wl+=a.p.at(it->second).Shag('~');
		if (kol(ql) != kol(wl))
			return false;
		else if (ql != "")
		{
			string v;
			string m;
			int i = 0;
			int j = 0;
			for (; v != "";)
			{
				v = Naz(ql, i);
				m = Naz(wl, j);
				if (x.find(v)!=x.end())
				{
					if (x.at(v) != m)
						return false;
				}
				else if (!svinka(a, x.insert(make_pair(v, m)).first, x))
					return false;
			}
		}
		return true;
	}


	public:

	Aftomat(vector<char> a, vector<string> v, string n, vector<string> c, tabl p) :alf(a), ver(v), nach(n), con(c), p(p) {}
	Aftomat(const string& x)
		{
			int i = -1;
			nk a = makeaft(x, i);
			nach = a.first;
			con.push_back(a.second);
		}
	Aftomat(const Aftomat& x) {

	}
	Aftomat()
	{}
	Aftomat(ifstream& f)
		{
			string x;
			f >> x;
			for (int i = 0; i < x.length(); i += 2)
				alf.push_back(x[i]);
			f >> x;
			string q;
			string w;
			for (int i = 0; i < x.length(); ++i)
			{
				q = x[i];
				i += 1;
				for (; (x[i] != ',') && (i < x.length()); ++i)
					q += x[i];
				ver.push_back(q);
				p[q] = Perehod(q, alf);
			}
			f >> x;
			nach = x;
			f >> x;
			for (int i = 0; i < x.length(); ++i)
			{
				q = x[i];
				i += 1;
				for (; (x[i] != ',') && (i < x.length()); ++i)
					q += x[i];
				con.push_back(q);
			}
			for (; !f.eof();)
			{
				f >> x;
				int i = 1;
				q = "";
				for (; (x[i] != ','); ++i)
					q += x[i];
				i += 1;
				w = "";
				for (; (x[i] != ','); ++i)
					w += x[i];
				i += 1;
				p[q].Dob(x[i], w);
			}
		}

	void Perenaz()
	{
		smena x;
		for (int i = 0; i < ver.size(); ++i)
		{
			x[ver[i]] = MakeNaz(i);
			ver[i] = x[ver[i]];
		}
		tabl n;
		for (auto it = p.begin(); it != p.end(); ++it)
		{
			n[x[it->first]] = Perehod(x[it->first], alf);
			for (int j = 0; j < alf.size(); ++j)
			{
				string a = it->second.Shag(alf[j]);
				if (a != "")
					n[x[it->first]].Dob(alf[j], x[a]);
			}
		}
		p = n;
		nach = x[nach];
		for (int i = 0; i < con.size(); ++i)
			con[i] = x[con[i]];
	}
	void Mini()
	{
		vector<set> x2;
		x2.push_back(set());
		x2.push_back(set());
		for (int i = 0; i < con.size(); ++i)
			x2[0].insert(con[i]);
		for (int i = 0; i < ver.size(); ++i)
			if (x2[0].count(ver[i]) == 0)
				x2[1].insert(ver[i]);
		vector<set> x1;
		vector<vector<int>> ekviv;
		for (; x1.size() != x2.size();)
		{
			ekviv.clear();
			x1 = x2;
			x2.clear();
			vector<set> x3 = x1;
			for (int i = 0; i < x1.size(); ++i)
			{
				for (; !x3[i].empty();)
				{
					auto it = x3[i].begin();
					string a = *it;
					vector<int> e = Ekv(a, x1);
					ekviv.push_back(e);
					x3[i].erase(a);
					x2.push_back(set());
					int k = x2.size() - 1;
					x2[k].insert(a);
					if (!x3[i].empty())
						for (it = x3[i].begin(); it != x3[i].end();)
						{
							if (plov(*it, e, x1))
							{
								x2[k].insert(*it);
								string g = *it;
								++it;
								x3[i].erase(g);
							}
							else
								++it;
						}
				}

			}
		}
		vector<string> v;
		for (int i = 0; i < x2.size(); ++i)
		{
			auto it = x2[i].begin();
			string z = *it;
			++it;
			for (; it != x2[i].end(); ++it)
				z += "," + *it;
			v.push_back(z);
		}
		tabl q;
		for (int i = 0; i < v.size(); ++i)
		{
			q[v[i]] = Perehod(v[i], alf);
			for (int j = 0; j < alf.size(); ++j)
				if (ekviv[i][j] != v.size())
					q[v[i]].Dob(alf[j], v[ekviv[i][j]]);
		}
		for (int i = 0;;++i)
			if (vhod(v[i], nach))
			{
				nach=v[i];
				break;
			}
		ver = v;
		p = q;
		vector<string> c;
		for (int i = 0; i < v.size(); ++i)
		{
			for (int j = 0; j < con.size(); ++j)
				if (vhod(v[i], con[j]))
				{
					c.push_back(v[i]);
					break;
				}
		}
		con = c;
	}
	void NKAvDKA()
	{
		vector<char> a = alf;
		vector<string> v;
		string n = UltraShag(nach, '~');
		vector<string> c;
		tabl tab;
		tablica(tab, v, n);
		for (int i = 0; i < v.size(); ++i)
		{
			for (int j = 0; j < con.size(); ++j)
				if (vhod(v[i], con[j]))
				{
					c.push_back(v[i]);
					break;
				}
		}
		ver = v;
		nach = n;
		con = c;
		p = tab;
	}
	string takenaz(string& x) const
	{
		int i = x.length() - 1;
		for (; i >= 0; --i)
			if (x[i] == '.')
				break;
		string a = "";
		for (int j = 0; j < i; ++j)
			a += x[j];
		return a;
	}
	void Vivod(const string& s) const
	{
		string z = s + ".dot";
		ofstream f;
		f.open(z);
		f << "digraph {" << endl;
		f << "	rankdir=LR;" << endl;
		f << "	margin=0.2;" << endl;
		f << "	size=5;" << endl;
		f << "	node [margin=0.1 shape=circle];" << endl;
		f << "	";
		f << "\"" + con[0] + "\"";
		for (int i = 1; i < con.size(); ++i)
			f << ',' << "\"" + con[i] + "\"";
		f << "[margin=0 shape=doublecircle];" << endl;
		f << "	\" \"" << "[color=white width=0];" << endl;
		f << "	\" " << "\" -> \"" << nach << "\";" << endl;
		for (int i = 0; i < ver.size(); ++i)
		{
			for (int j = 0; j < alf.size(); ++j)
			{
				string x = p.at(ver[i]).Shag(alf[j]);
				if (x != "")
					f << "	\"" << p.at(ver[i]).Naz() << "\" -> " << " \"" + x + "\" " << " [label=\"" << alf[j] << "\"];" << endl;
			}
			string e = "";
			e+=p.at(ver[i]).Shag('~');
			for (int j = 0; j < e.length(); ++j)
			{
				string a = "";
				for (; (e[j] != ',') && (j < e.length()); ++j)
					a += e[j];
				f << "	\"" << p.at(ver[i]).Naz() << "\" -> " << a << " [label=" << "<&#955;>" << "];" << endl;
			}
		}
		f << "}";
		f.close();
		string r = "dot -Tsvg < " + z + " > " + s + ".svg";
		system(r.c_str());
	}
	void Draw(ofstream& f,char x) const
	{
		char X = x - 32;
		f << "$"<<X<<"= \\langle \\sum_" << x << ", Q_a, s_" << x << ", T_" << x << ", \\sigma_" << x << " \\rangle$" << endl << "\\" << endl;
		f << "$\\sum_"<<x<<"= {";
		f << alf[0];
		for (int i = 1; i < alf.size(); ++i)
			f << ',' << alf[i];
		f << "}$\\" << endl;
		f << "$Q_" << x << " = {";
		f << ver[0];
		for (int i = 1; i < ver.size(); ++i)
			f << ',' << ver[i];
		f << "}$\\" << endl;
		f << "$s_" << x << " = {" << nach << "}$\\" << endl;
		f << "$T_" << x << " = {";
		f << con[0];
		for (int i = 1; i < con.size(); ++i)
			f << ',' << con[i];
		f << "}$\\" << endl;
		f << "$\\sigma_" << x << " = $";
	}
	void DrawTable(ofstream& f) const
	{
		f << " | |";
		for (int i = 0; i < alf.size(); ++i)
			f << alf[i] << "|";
		f << "$\\lambda$ |";
		f << endl;
		f << " | -- | ";
		for (int i = 0; i < alf.size(); ++i)
			f << "-- | ";
		f << "-- | ";
		f << endl;
		for (int i = 0; i < ver.size(); ++i)
		{
			f << "|$\\langle " << ver[i] << " \\rangle$|";
			for (int i = 0; i < alf.size(); ++i)
				f <<"$\\langle "<< p.at(ver[i]).Shag(alf[i])<<" \\rangle$|";
			f << "$\\langle " << p.at(ver[i]).Shag('~') << " \\rangle$|";
			f << endl;
		}
		f << endl;
	}
	const Aftomat& AmazingProizvedenie(const Aftomat& x, const string& file, char flag)
	{
		this->Vivod(file + "(A)");
		x.Vivod(file + "(B)");
		ofstream f;
		f.open(file + ".md");
		if (flag == '*')
			f << UnicodeToUTF8(L"### Пересечение автоматов")<<endl<<"\\" << endl;
		else if (flag == '+')
			f << UnicodeToUTF8(L"### Объединение автоматов") << endl << "\\" << endl;
		else 
			f << UnicodeToUTF8(L"### Разность автоматов") << endl << "\\" << endl;
		f << UnicodeToUTF8(L"Первый автомат:") << endl;
		Draw(f, 'a');
		f << "![GraphA]("<< file + "(A)" << ".svg)\\" << endl;
		f << UnicodeToUTF8(L"Второй автомат:") << endl;
		x.Draw(f, 'b');
		f << "![GraphB](" << file + "(B)" << ".svg)\\" << endl;
		if (flag == '*')
			f << UnicodeToUTF8(L"Пересечение автоматов:") << endl << "\\" << endl;
		else if (flag == '+')
			f << UnicodeToUTF8(L"Объединение автоматов:") << endl << "\\" << endl;
		else
			f << UnicodeToUTF8(L"Разность автоматов:") << endl << "\\" << endl;
		if (flag == '*')
			*this *= x;
		else if (flag == '+')
			*this += x;
		else if (flag == '-')
			*this -= x;
		Vivod(file + "(Result)");
		Draw(f, 'd');
		DrawTable(f);
		f << "![GraphD](" << file + "(Result)" << ".svg)";
		f.close();
		return *this;
	}
	const Aftomat& AmazingNKAvDKA(const string& file)
	{
		Aftomat d;
		ofstream f;
		f.open(file + ".md");
		f << UnicodeToUTF8(L"### Преобразование НКА в ДКА") << endl << "\\" << endl;
		f << UnicodeToUTF8(L"Исходный автомат:") << endl << "\\" << endl;
		Draw(f, 'a');
		f << endl<<'\\' << endl;
		Vivod("NKA" + file);
		f << "![GraphA](" << "NKA" + file << ".svg)\\" << endl;
		f << UnicodeToUTF8(L"Преобразуем в ДКА:") << endl;
		NKAvDKA();
		Vivod("DKA" + file);
		Draw(f, 'a');
		f << endl;
		DrawTable(f);
		Perenaz();
		f << "![GraphB](" << "DKA" + file << ".svg)\\" << endl;
		f << UnicodeToUTF8(L"минимизируем ДКА и переименуем вершины:") << endl;
		Mini();
		Perenaz();
		Draw(f, 'a');
		f << endl << '\\' << endl;
		Vivod("min_DKA" + file);
		f << "![GraphD](" << "min_DKA" + file << ".svg)";
		f.close();
		return *this;
	}
	const Aftomat& operator =(const Aftomat& x)
	{
		alf = x.alf;
		ver = x.ver;
		nach = x.nach;
		con = x.con;
		p = x.p;
		return *this;
	}
	const Aftomat& operator *(const Aftomat& a) const
	{
		vector<char> al = ObAlf(a.alf);
		tabl y;
		vector<string> x;
		Obtab(a, al, y, x);
		vector<string> c;
		for (int i = 0; i < con.size(); ++i)
			for (int j = 0; j < a.con.size(); ++j)
				c.push_back(con[i] + ',' + a.con[j]);
		return *(new Aftomat(al, x, x[0], c, y));
	}
	const Aftomat& operator *=(const Aftomat& a) 
	{
		return *this=a**this;
	}
	const Aftomat& operator -()
	{
		vector<string> a;
		for (int i = 0; i < ver.size(); ++i)
		{
			bool b = false;
			for (int j = 0; (j < con.size()) && (!b); ++j)
				if (ver[i] == con[i])
					b = true;
			if (!b)
				a.push_back(ver[i]);
		}
		con = a;
		return *this;
	}
	const Aftomat& operator +(const Aftomat& a) const
	{
		vector<char> al = ObAlf(a.alf);
		tabl y;
		vector<string> x;
		Obtab(a, al, y, x);
		vector<string> c;
		for (int i = 0; i < x.size(); ++i)
		{
			for (int j = 0; j < con.size(); ++j)
				if (vhod(x[i], con[j]))
				{
					c.push_back(x[i]);
					break;
				}
			for (int j = 0; j < a.con.size(); ++j)
				if (vhod(x[i], a.con[j]))
				{
					c.push_back(x[i]);
					break;
				}
		}
		return *(new Aftomat(al, x, x[0], c, y));
	}
	const Aftomat& operator +=(const Aftomat& a)
	{
		return *this = *this + a;
	}
	const Aftomat& operator -(const Aftomat& a) const
	{
		Aftomat x(a);
		return *this * ( -x);
	}
	const Aftomat& operator -=(const Aftomat& a)
	{
		return *this = *this - a;
	}
	bool operator ==(const Aftomat& a) const
	{
		if ((a.alf != alf) || (a.ver.size() != ver.size()) || (a.con.size() != con.size()))
			return false;
		else
		{
			smena q;
			return svinka(a,q.insert(make_pair(nach, a.nach)).first,q);
		}
	}
	bool operator !=(const Aftomat& a) const
	{
		return !(*this == a);
	}
};

