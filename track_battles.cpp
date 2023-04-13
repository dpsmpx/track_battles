#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <vector>
#include <math.h>
using namespace std;
/* 
   TODO:
   + сохранение аккаунта на диске
   + загрузка аккаунта с диска
   + ведение текущей сэссии
   + отслеживание прогресса по танкам
   + отображать всегда сэссию за n боев, отдельно за произвольный период по танку и в общем
   + писать количество сэссий и танков перед записью
   + открытие последнего профиля при запуске проги
   + автоматическое определение следующего уровня
   + отображать сэссию по опред танку
   + отображать рекорд
   + убрать баг при 100% побед
   - сброс сэссии
   - красивая смена танка(список)
*/

//данные
  string nickname;
  bool needToChangeAcc = false;
  double
    win, lose, allDMG;
	int tankID;
	int showSBattles=100;//отображать статистику последних X боев
	
struct battle//отслеживает изменения
{
  string tankname;
	int win;
	int dmg;
};vector<battle> session;
int sessnn(){return session.size();}
int sessid(){return session.size()-1;}

struct tank//содержит актуальную информацию
{
  string name;
  int wins;
  int loses;
  int dmg;
};vector<tank>hangar;
int tanks(){return hangar.size();}
int tanid(){return hangar.size()-1;}

//регистрация боев
void addWin()
{
  cout << "Введите урон: ";
  int dmg; cin >> dmg; hangar[tankID].dmg+=dmg;
  hangar[tankID].wins++;
  win++;allDMG+=dmg;
  battle btl;
  btl.dmg=dmg;
  btl.win=1;
  btl.tankname=hangar[tankID].name;
  session.push_back(btl);
}
void addLose()
{
  cout << "Введите урон: ";
  int dmg; cin >> dmg; hangar[tankID].dmg+=dmg;
  hangar[tankID].loses++;
  lose++;allDMG+=dmg;
  battle btl;
  btl.dmg=dmg;
  btl.win=0;
  btl.tankname=hangar[tankID].name;
  session.push_back(btl);
}

//вычисляемые значения
double battles(){return win + lose;}
double winrate(){return win / (win + lose) * 100;}
double needWinRate(double wins, double loses){
  double iss = (wins/(wins+loses))*100;
  double half = modf(iss, &half);
  if (iss-half+1 < 100)
    return iss-half+1;
  else return 100;
}
double needDamage(double dmg, double btls){
  double iss = dmg/btls/100;
  double half = modf(iss,&half);
  return (iss-half)*100+100;
}
double beAVGdmg(double dmg, double btls){
  double iss = dmg/btls/100;
  double half = modf(iss,&half);
  return (iss-half)*100+600;
}
double winrate(double win, double lose)
{
  if (win==0)
    return 0;
  else
    return win/(win+lose)*100;
}
int avgDMG(){return allDMG / battles();}
double avgDMG(double battles, double dmg){return dmg/battles;}
int leftWins(double win, double lose, double need)
{
	double add = 0;
	for (; ((win + add) / (win + add + lose)) * 100 < needWinRate(win,lose); add++);
	return add;
}
int leftDMG(double allDMG, double battls, double need, double beAVGdmg)
{
	double add = 0;
	for (; (allDMG + beAVGdmg * add) / (add + battls) < needDamage(allDMG,battls); add++);
	return add;
}

//преобразованиие чисел в текст
#include <string>
#include <sstream>
template <typename T>
std::string toString(T val)
{
  std::ostringstream oss;
  oss<<val;
  return oss.str();
}

void showSession()
{
  int sBattles = showSBattles;
  int record = 0, seria = 0, max = 0;
  if (sessnn() < sBattles) sBattles = sessnn();
  if (sBattles == 0) cout << "Начните сэссию!" << endl; else
  {
    double sWins, sLoses, sDMG,
    allSWins, allSLoses, allSDMG;
    for (int i = 0; i < sessnn(); i++)
    if (i >= sessnn()-sBattles)
    {
      if (session[i].win==1)
      {
        sWins++;
        allSWins++;
        if (seria > max)
          max = seria;
        seria++;
      }
      else
      {
        sLoses++;
        allSLoses++;
        if (seria > max)
          max = seria;
        seria = 0;
      }
      sDMG += session[i].dmg;
      allSDMG += session[i].dmg;
      if (session[i].dmg > record)
        record = session[i].dmg;
   } else
   {
      if (session[i].win==1)
        allSWins++;
      else
        allSLoses++;
      allSDMG += session[i].dmg;
   }
   cout << "Сэссия: " << sBattles << " боев, " << winrate(atof(toString(sWins).c_str()), atof(toString(sLoses).c_str())) << "%, " << sDMG/(sWins+sLoses) << " с/у\nРекорд: " << record;
   if (max > 0)
     cout << ", Серия: " << max;
   cout << endl;
   cout << "Фулл: " << sessnn() << " боев, " << winrate(atof(toString(allSWins).c_str()), atof(toString(allSLoses).c_str())) << "%, " << allSDMG/(allSWins+allSLoses) << " с/у\n";

  }
}

void showTSession()
{
  int record = 0, seria = 0, max = 0;
  int factBattles = 0;
  int sBattles = showSBattles;
  if (sessnn() < sBattles) sBattles = sessnn();
  if (sBattles == 0) cout << "Начните сэссию!" << endl; else
  {
    double sWins, sLoses, sDMG;
    for (int i = 0; i < sessnn(); i++)
    if (session[i].tankname == hangar[tankID].name)
    {
      factBattles ++;
      if (session[i].win==1)
      {
        sWins++;
        if (seria > max)
          max = seria;
        seria++;
      }
      else
      {
        sLoses++;
        if (seria > max)
          max = seria;
        seria = 0;
      }
      sDMG += session[i].dmg;
      if (session[i].dmg > record)
        record = session[i].dmg;
   }
   if (factBattles == 0)
     cout << "Начните сэссию!" << endl;
   else
   {
     cout << "Сэссия: " << factBattles << " боев, " << winrate(atof(toString(sWins).c_str()), atof(toString(sLoses).c_str())) << "%, " << sDMG/(sWins+sLoses) << " с/у\nРекорд: " << record;
     if (max > 0)
       cout << ", Серия: " << max;
     cout << endl;
   }
  }
}

//вывод информации
void showStats()
{
	cout << "   " << nickname << ":\n";
	showSession();
	cout << "Всего: " <<  win+lose << " боев, " << winrate() << "%, " << avgDMG() << " с/у" << endl;
}
void showTank(int ID)
{
  cout << "   танк " << hangar[ID].name << ":\n";
  showTSession();
  cout << "Всего: " << hangar[tankID].wins+hangar[tankID].loses << " боев, " << winrate(hangar[ID].wins,hangar[ID].loses) << "%, " << avgDMG(hangar[ID].wins+hangar[ID].loses, hangar[ID].dmg) << " с/у" << endl;
}

//добавить танк
void addTank(string tankname)
{
  hangar.resize(tanks()+1);
  int battls;
  cout << "Количество боев: ";
  cin >> battls;
  cout << "Количество побед: ";
  cin >> hangar[tanid()].wins;
  hangar[tanid()].loses = battls-hangar[tanid()].wins;
  cout << "Введите урон: ";
  cin >> hangar[tanid()].dmg;
  hangar[tanid()].name = tankname;
  tankID = tanks()-1;
}

//сменить танк
void changeTank()
{
  int i;
  system("clear");
  cout << "Список танков:\n";
  for (i = 0; i < hangar.size(); i++)
    cout << hangar[i].name << endl;
  string tankname;
  bool hasTank=false;
  cout << "Введите танк: ";
  cin >> tankname;
  if (hangar.size()!=0)
  for (i = 0; i < hangar.size() && !hasTank; i++)
    if (hangar[i].name == tankname)
    {
      tankID = i;
      hasTank = true;
    }
  if (!hasTank) addTank(tankname);
}

//работа с сохранениями
void WriteAllLines(string path, vector<string> txt)
{
  char tc[80];
  FILE *fp=fopen(path.c_str(),"w");
  for (int i = 0; i < txt.size(); i++)
  {
    sprintf(tc,"%s",txt[i].c_str());
    fputs(tc,fp);fputs("\n",fp);
  }
  fclose(fp);
}

vector<string> ReadAllLines(string path)
{
  vector<string> res;
  FILE *fp;
  if ((fp = fopen(path.c_str(),"r")) == NULL)
  {
    fclose(fp);
    return res;
  } else fp = fopen(path.c_str(),"r");
  string line;
  char ch;
  while (!feof(fp))
  {
    ch = getc(fp);
    if (ch == '\n')
    {
      res.push_back(line);
      line = "";
    } else line += ch;
  }
  fclose(fp);
  return res;
}

void saveProfile()
{
  vector<string> pf;
  pf.push_back(toString(win).c_str());
  pf.push_back(toString(lose).c_str());
  pf.push_back(toString(allDMG).c_str());
  pf.push_back(toString(tankID).c_str());
  pf.push_back(toString(sessnn()).c_str());
  for (int i = 0; i < sessnn(); i++)
  {
    pf.push_back(session[i].tankname.c_str());
    pf.push_back(toString(session[i].win).c_str());
    pf.push_back(toString(session[i].dmg).c_str());
  }
  pf.push_back(toString(tanks()).c_str());
  for (int i = 0; i < tanks(); i++)
  {
    pf.push_back(hangar[i].name.c_str());
    pf.push_back(toString(hangar[i].wins).c_str());
    pf.push_back(toString(hangar[i].loses).c_str());
    pf.push_back(toString(hangar[i].dmg).c_str());
  }
  WriteAllLines(nickname.c_str(),pf);
  pf.resize(0);
  pf.push_back(nickname.c_str());
  WriteAllLines("lastProfile.txt",pf);
}

void loadProfile(string nickname)
{
  vector<string> profile = ReadAllLines(nickname);
  win = atof(profile[0].c_str());
  lose = atof(profile[1].c_str());
  allDMG = atof(profile[2].c_str());
  tankID = atoi(profile[3].c_str());
  session.resize(0);
  for (int i = 0; i < atoi(profile[4].c_str()); i++)
  {
    battle btl;
    btl.tankname = profile[5+i*3];
    btl.win = atoi(profile[6+i*3].c_str());
    btl.dmg = atoi(profile[7+i*3].c_str());
    session.push_back(btl);
  }
  int pos = 5+atoi(profile[4].c_str())*3;
  hangar.resize(0);
  for (int i = 0; i < atoi(profile[pos].c_str()); i++)
  {
    tank tnk;
    tnk.name = profile[pos+1+i*4];
    tnk.wins = atoi(profile[pos+2+i*4].c_str());
    tnk.loses = atoi(profile[pos+3+i*4].c_str());
    tnk.dmg = atoi(profile[pos+4+i*4].c_str());
    hangar.push_back(tnk);
  }
}

//записать данные аккаунта
void createNewAccount()
{
  int battls;
  cout << "Введите кол-во боев: ";
  cin >> battls;
  cout << "Введите кол-во побед: ";
  cin >> win;
  lose = battls-win;
  cout << "Введите весь урон: ";
  cin >> allDMG;
  session.resize(0);
  hangar.resize(0);
  changeTank();
}

//выбрать аккаунт
void login()
{
  FILE *fp;
  if (needToChangeAcc || (fp = fopen("lastProfile.txt","r")) == NULL)
  {
    cout << "Введите ник: ";
  	cin >> nickname;
  	needToChangeAcc = false;
  	if ((fp = fopen(nickname.c_str(), "r")) == NULL)
  	  createNewAccount();
  	else
  	{
  	  loadProfile(nickname);
  	  fclose(fp);
  	}
  } else
  {
    fclose(fp);
    nickname = ReadAllLines("lastProfile.txt")[0];
    loadProfile(nickname);
  }
  saveProfile();
}

void changeAll()
{
  string change[100] = {
    "Количество побед ("+(int)win+')',
    "Количество поражений ("+(int)lose+')',
    "Урон ("+(int)allDMG+')'
  };
}

void changeStats()
{
  string change[100] = {
    "Общая стата",
    "Стата танка",
    "Пересчитать",
    "Назад"
  };
  char p = '*';
  while (p != '4')
  {
    system("clear");
    cout << "Править стату, выберите опцию:\n";
    for (int i = 0; i < 4; i ++)
      cout << i+1 << " - " << change[i] << endl;
    cout << endl;
    while (!(strchr("1234", p))) p = getch();
	  	switch (p){
 		   case '1': changeAll();break;
    		case '2': addLose();break;
  		  case '3': changeTank();break;
	  	  case '4': needToChangeAcc = true;login();break;
	  	  case '5': changeStats();break;
    }
  }
}

//управление
string functions[10] = {
	"Победа",
	"Слив",
	"Сменить танк",
	"Сменить аккаунт",
	"Переписать стату"
};
int main()
{
	login();
	while (1)
	{
		system("clear");
		//пункты меню
		for (int i = 0; i < 5; i++)
			cout << i+1 << " - " << functions[i] << endl;
	  cout << endl;
	  //стата
		showStats();
		cout << ">" << leftWins(win,lose,needWinRate(win,lose)) << " побед до " << needWinRate(win,lose) << " % побед" << endl;
		cout << ">" << leftDMG(allDMG,battles(), needDamage(allDMG,battles()), beAVGdmg(allDMG,battles())) << " боев на " << beAVGdmg(allDMG,battles()) << " урона до " << needDamage(allDMG,battles()) << " с/у" << endl;
		showTank(tankID);
		cout << ">" << leftWins(hangar[tankID].wins,hangar[tankID].loses,needWinRate(hangar[tankID].wins,hangar[tankID].loses)) << " побед до " << needWinRate(hangar[tankID].wins,hangar[tankID].loses) << "% побед" << endl;
		cout << ">" << leftDMG(hangar[tankID].dmg,hangar[tankID].wins+hangar[tankID].loses, needDamage(hangar[tankID].dmg,hangar[tankID].wins+hangar[tankID].loses), beAVGdmg(hangar[tankID].dmg,hangar[tankID].wins+hangar[tankID].loses)) << " боев на " << beAVGdmg(hangar[tankID].dmg,hangar[tankID].wins+hangar[tankID].loses) << " урона до " << needDamage(hangar[tankID].dmg,hangar[tankID].wins+hangar[tankID].loses) << " с/у" << endl;
		//управление
		char p = '*';
		while (!(strchr("12345", p))) p = getch();
		switch (p){
		  case '1': addWin();break;
		  case '2': addLose();break;
		  case '3': changeTank();break;
		  case '4': needToChangeAcc = true;login();break;
		  case '5': changeStats();break;
		}
		saveProfile();
	}
}