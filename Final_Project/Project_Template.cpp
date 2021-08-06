#include <iostream>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <exception>
#include <stdexcept>
using namespace std;

class Date {
public:
    int GetYear() const { return Year; }
    int GetMonth() const { return Month; }
    int GetDay() const { return Day; }
    friend istream& operator>>(istream& stream, Date& date);
private:
    int Year;
    int Month;
    int Day;
};

istream& operator>>(istream& stream, Date& date)
{
    stream >> date.Year;
    stream.ignore(1);
    stream >> date.Month;
    if (date.Month < 1 || date.Month > 12)
        throw invalid_argument("Month value is invalid: " + to_string(date.Month));
    stream.ignore(1);
    stream >> date.Day;
    if (date.Day < 1 || date.Day > 31)
        throw invalid_argument("Day value is invalid: " + to_string(date.Day));
    return stream;
}

bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() < rhs.GetYear())
        return true;
    else
    {
        if (lhs.GetYear() > rhs.GetYear())
            return false;
        else
        {
            if (lhs.GetMonth() < rhs.GetMonth())
                return true;
            else
            {
                if (lhs.GetMonth() > rhs.GetMonth())
                    return false;
                else
                {
                    if (lhs.GetDay() < rhs.GetDay())
                        return true;
                    else
                        return false;
                }
            }
        }
    }
}

bool operator==(const Date& lhs, const Date& rhs)
{
    if ((lhs.GetYear() == rhs.GetYear()) && (lhs.GetMonth() == rhs.GetMonth()) && (lhs.GetDay() == rhs.GetDay()))
        return true;
    else
       return false;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event) 
    { 
        if (DatesAndEvents.find(date) != DatesAndEvents.end())
        {
            if (DatesAndEvents.find(date)->second.find(event) == DatesAndEvents.find(date)->second.end())
                DatesAndEvents[date].insert(event);
        }
        else
            DatesAndEvents[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const string& event)
    {
        if (DatesAndEvents.find(date) != DatesAndEvents.end())
        {
            if (DatesAndEvents.find(date)->second.find(event) != DatesAndEvents.find(date)->second.end())
            {
                DatesAndEvents.find(date)->second.erase(event);
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }

    int DeleteDate(const Date& date)
    {
        if (DatesAndEvents.find(date) != DatesAndEvents.end())
        {
            int Quantity = 0;
            Quantity = DatesAndEvents.find(date)->second.size();
            if (Quantity)
                DatesAndEvents.find(date)->second.erase(DatesAndEvents.find(date)->second.begin(), DatesAndEvents.find(date)->second.end());
            return Quantity;
        }
        else
           return 0;
    }

    void Find(const Date& date) const
    {
        if (DatesAndEvents.find(date) != DatesAndEvents.end())
        {
            for (const auto& item1 : DatesAndEvents)
            {
                if (item1.first == date)
                {
                    for (const auto& item2 : item1.second)
                    {
                        cout << item2 << endl;
                    }
                }
            }
        }  
    }

    void Print() const
    {
        for (const auto& item1 : DatesAndEvents)
        {
            for (const auto& item2 : item1.second)
            {
                cout << setfill('0') << setw(4);
                cout << item1.first.GetYear();
                cout << '-' << setw(2) << item1.first.GetMonth() << '-' << setw(2) << item1.first.GetDay() << ' ' << item2 << endl;
            }
        }
    }
private:
    map<Date, set<string>> DatesAndEvents;
};

void SplitCommand(const string& Command, string& SplitedStr, int& Counter)
{
    while (Command[Counter] != ' ' && Counter < Command.size())
    {
        SplitedStr += Command[Counter];
        Counter++;
    }
    if (Counter < Command.size())
        Counter++;
}

void CheckDate(const string& date)
{
    bool IsUnacceptableSymbol = false;
    bool Separator = false;
    if (!isdigit(date[0]))
    {
        if (date[0] == '-')
            if (!isdigit(date[1]))
                throw invalid_argument("Wrong date format: " + date);
        else
            throw invalid_argument("Wrong date format: " + date);
    }
    int QuantityOfSeparators = 2, Separators = 0;
    for (int i = 0; i < date.size(); i++)
    {
        if (date[i] == '-' && !Separator)
        {
            Separator = true;
            Separators++;
            continue;
        }
        else
        {
            if (Separator && !isdigit(date[i]) && !IsUnacceptableSymbol)
            {
                if (date[i] != '-' && date[i] != '+')
                    throw invalid_argument("Wrong date format: " + date);
                IsUnacceptableSymbol = true;
                continue;
            }
            else
                Separator = false;
            if (IsUnacceptableSymbol && !isdigit(date[i]))
                throw invalid_argument("Wrong date format: " + date);
            else
                IsUnacceptableSymbol = false;
        }
        if ((!isdigit(date[i]) && date[i] != '+') || (!isdigit(date[i]) && Separator && date[i] != '+'))
            throw invalid_argument("Wrong date format: " + date);
    }
    if (Separators < QuantityOfSeparators || Separators > QuantityOfSeparators || Separator)
        throw invalid_argument("Wrong date format: " + date);
}

void Add(Database& obj, const string& date, const string& event)
{
    istringstream Input_Date(date);
    Date Current_Date;
    Input_Date >> Current_Date;
    obj.AddEvent(Current_Date, event);
}

void DelEvent(Database& obj, const string& date, const string& event)
{
    istringstream Input_Date(date);
    Date Current_Date;
    Input_Date >> Current_Date;
    if (obj.DeleteEvent(Current_Date, event))
        cout << "Deleted successfully" << endl;
    else
        cout << "Event not found" << endl;
}

void DelDate(Database& obj, const string& date)
{
    istringstream Input_Date(date);
    Date Current_Date;
    Input_Date >> Current_Date;
    cout << "Deleted " << obj.DeleteDate(Current_Date) << " events" << endl;
}

void FindDate(Database& obj, const string& date)
{
    istringstream Input_Date(date);
    Date Current_Date;
    Input_Date >> Current_Date;
    obj.Find(Current_Date);
}

int main() {
    Database db;

    string command;
    try
    {
        while (getline(cin, command)) {
            if (command.empty())
                continue;
            else
            {
                if (command == "Print")
                    db.Print();
                else
                {
                    string ExactCommand, ExactDate, ExactEvent;
                    int Counter = 0;
                    SplitCommand(command, ExactCommand, Counter);
                    if (ExactCommand == "Add")
                    {
                        SplitCommand(command, ExactDate, Counter);
                        CheckDate(ExactDate);
                        SplitCommand(command, ExactEvent, Counter);
                        Add(db, ExactDate, ExactEvent);
                    }
                    else
                    {
                        if (ExactCommand == "Del")
                        {
                            SplitCommand(command, ExactDate, Counter);
                            CheckDate(ExactDate);
                            SplitCommand(command, ExactEvent, Counter);
                            if (!ExactEvent.empty())
                                DelEvent(db, ExactDate, ExactEvent);
                            else
                                DelDate(db, ExactDate);
                        }
                        else
                        {
                            if (ExactCommand == "Find")
                            {
                                SplitCommand(command, ExactDate, Counter);
                                CheckDate(ExactDate);
                                FindDate(db, ExactDate);
                            }
                            else
                                throw invalid_argument("Unknown command: " + ExactCommand);
                        }
                    }
                }
            }
        }
    }
    catch (invalid_argument& arg)
    {
        cout << arg.what() << endl;
    }
    system("pause");
    return 0;
}