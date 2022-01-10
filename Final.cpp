#include <iostream>
#include <map>
#include <set>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>
using namespace std;

class Date {
public:
	Date(){}
	Date(int y, int m, int d) //год, месяц, день
	{
		if(m > 12 | m <= 0)
			throw runtime_error("Month value is invalid: " + to_string(m));
		else if(d > 31 | d <=0)
			throw runtime_error("Day value is invalid: " + to_string(d));
		else
		{	
			year = y;
			month = m;
			day = d;
		}
	}
  	int GetYear() const
	{
  		return year;
	}
  	int GetMonth() const
	{
  		return month;
	}
  	int GetDay() const
	{
  		return day;
	}
private:
	int year, month, day;
};

bool operator<(const Date& lhs, const Date& rhs)
{
	if(lhs.GetYear() < rhs.GetYear())
		return true;
	else if (lhs.GetYear() == rhs.GetYear())
	{
		if(lhs.GetMonth() < rhs.GetMonth())
			return true;
		else if(lhs.GetMonth() == rhs.GetMonth())
		{
			return (lhs.GetDay() < rhs.GetDay());
		}
	}
	return false;
}

ostream& operator<< (ostream& stream, const Date& date) 
{
	stream << setfill('0');
	stream << setw(4) << date.GetYear() << '-';
	stream << setw(2) << date.GetMonth() << '-'<< setw(2) << date.GetDay();
	return stream;
}

istream& operator>> (istream& stream, Date& date) 
{
	int y, m, d;
	char s1, s2;
	
	if (stream)
	{
		string str;
		stream >> str;		// сначала получим дату целиком
		
		stringstream newStream;
		newStream << str;	// переведём дату обратно в потоковый формат
		newStream >> y >> s1 >> m >> s2 >> d;
		
		if(newStream && newStream.peek() == EOF)//если с потоком всё хорошо и там больше ничего нет (приведение потока к формату bool)
		{
			if (s1 == '-' && s2 == '-')
			{
				date = Date(y, m, d);
				return stream;
			}
		}
		throw runtime_error("Wrong date format: " + str);
	}
	
}

class Database {
public:
	// Добавить событие
  	void AddEvent(const Date& date, const string& event){
		DateEvents[date].insert(event);
  	};
  	
  	// Удалить событие
  	bool DeleteEvent(const Date& date, const string& event)
	{
  		if (DateEvents[date].count(event))
		{
  			DateEvents[date].erase(event);
  			if (DateEvents[date].size() == 0)
  				DateEvents.erase(date);	
  			return true;
  		}
		else
  			return false;	
  	}
  	
  	// Удалить все события в указанную дату
  	int  DeleteDate(const Date& date)
  	{
  		int N = DateEvents[date].size();
  		DateEvents.erase(date);
  		return N;
	}
	
	// Вывести события конкретной даты
	string Find(const Date& date) const
	{
		stringstream str;
		for(const auto& i:DateEvents.at(date)) // для константного словаря
			str << i << endl;
				
		return str.str();
	}
  
	// Вывести все даты и события
	void Print() const
	{
		for (const auto& i:DateEvents)
		{
			for (const auto& j:i.second)
			{
				cout << i.first << " " << j << endl;	
			}
		}
	}
	
private:
	map<Date, set<string>> DateEvents;
};

int main() {
  	Database db;
  	string command;
  	
  	while (getline(cin, command)) // в command поступает введённая строка целиком (до переноса строки)
	{
		// преобразуем строку command в поток a для дальнейшей работы:
		stringstream a; 
		a << command;
			
		string c;
		a >> c;
		if (c == "Add" | c=="Del" | c=="Find")
		{
			Date date;
			// Проверяем номер месяца и дня
			try
			{
				a >> date;
			}
			catch(runtime_error& er)
			{
				cout << er.what();
				return 0;
			}
		
			if (c == "Add")
			{
				string event;
				a >> event;
				// Добавляем событие
				db.AddEvent(date, event);
			}
			else if(c == "Del")		//обработать >>
			{	
				string event;
				a >> event;
				// Удаляем событие
				if (a) 
				{
					if (db.DeleteEvent(date, event))
						cout << "Deleted successfully" << endl;
					else
						cout << "Event not found" << endl;
				}
				else // Удаляем все события за текущую дату
					cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
			}
			else if(c == "Find")	//обработать >>
			{
				try // Ловим исключение метода at
				{
					cout << db.Find(date);
				}
				catch(out_of_range& ex)
				{
				}	
			}
		}
		else if(c == "Print")	
		{
			db.Print();
		}
		else if (c == "")		
		{
		}
		else 					
		{
			cout << "Unknown command: " << c << endl;
		}
		
	}

  return 0;
}
