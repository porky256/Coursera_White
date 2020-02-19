#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <optional>

class Date {
public:
    int GetYear() const { return year;};
    int GetMonth() const { return month;};
    int GetDay() const {return day;};
    Date(int year=0,int month=1,int day=1){
        if (month<1||month>12) throw std::runtime_error("Month value is invalid: "+std::to_string(month));
        if (day<1||day>31) throw std::runtime_error("Day value is invalid: "+std::to_string(day));
        this->year=year;
        this->month=month;
        this->day=day;
    };

private:
    int year;
    int day;
    int month;
};

bool operator<(const Date& lhs, const Date& rhs) {
    if (&lhs== nullptr||&rhs== nullptr) return false;
    if (lhs.GetYear()!=rhs.GetYear()) return lhs.GetYear()<rhs.GetYear();
    else if (lhs.GetMonth()!=rhs.GetMonth()) return lhs.GetMonth()<rhs.GetMonth();
    else return lhs.GetDay()<rhs.GetDay();
};

class Database {
public:
    void AddEvent(const Date& date, const std::string& event) {
        data[date].insert(event);
    };
    bool DeleteEvent(const Date& date, const std::string& event) {
        return data[date].erase(event);
    };
    int  DeleteDate(const Date& date) {
        int sz=data[date].size();
        data.erase(date);
        return sz;
    };

    std::optional<std::set<std::string>> Find(const Date& date) const {
        if (data.find(date)==data.end()) { return VOID;}
        return data.find(date)->second;
    };

    void Print() const{
        for (const auto& x:data) {
            for (const auto& y:x.second) {
                std::cout <<std::setfill('0')<<std::setw(4)<<x.first.GetYear()<<'-'
                          <<std::setw(2)<<x.first.GetMonth()<<'-'
                          <<std::setw(2)<<x.first.GetDay()<<' '<<y<<'\n';
            }
        }
    };

private:
    std::map<Date,std::set<std::string>> data;
};

Date MakeDate(std::string str) {
    int year,month,day;
    std::string integers = "0123456789+";
    std::stringstream stream(str);
    try {
        if (integers.find(stream.peek())==std::string::npos) throw std::exception();
        stream >> year;
        if (stream.peek()!='-') throw std::exception();
        stream.ignore(1);
        if (integers.find(stream.peek())==std::string::npos) throw std::exception();
        stream >> month;
        if (stream.peek()!='-') throw std::exception();
        stream.ignore(1);
        if (integers.find(stream.peek())==std::string::npos) throw std::exception();
        stream >> day;
        if (!stream.eof()) throw std::exception();
    } catch(...) {
        throw std::runtime_error("Wrong date format: "+str);
    }
    return {year,month,day};
}

int main() {
    Database db;
    std::string command;
    //std::ifstream ifstr("input.txt");
    std::vector<std::string> command_list = {"Add","Find","Del","Print"};
    try {
        while (getline(std::cin, command)) {
            if (command.empty()) continue;
            command += " ";
            std::string instruction;
            instruction = command.substr(0, command.find(' '));
            command.erase(0, command.find(' ') + 1);
            if (std::find(command_list.begin(), command_list.end(), instruction) == command_list.end())
                throw std::runtime_error("Unknown command: " + instruction);
            if (instruction == command_list[3]) {
                db.Print();
                continue;
            }
            if (instruction == command_list[2]) {
                std::string date;
                date = command.substr(0, command.find(' '));
                command.erase(0, command.find(' ') + 1);
                if (command.empty()) {

                    int sz = db.DeleteDate(MakeDate(date));
                    std::cout << "Deleted " << sz << " events\n";
                } else {
                    command.erase(command.size() - 1);
                    bool deleted = db.DeleteEvent(MakeDate(date), command);
                    if (deleted) {
                        std::cout << "Deleted successfully\n";
                    } else {
                        std::cout << "Event not found\n";
                    }
                }
                continue;
            }
            if (instruction == command_list[1]) {
                std::string date;
                date = command.substr(0, command.find(' '));
                for (const auto &i:db.Find(MakeDate(date))) {
                    std::cout << i << '\n';
                }
                continue;
            }

            if (instruction == command_list[0]) {
                std::string date;
                date = command.substr(0, command.find(' '));
                command.erase(0, command.find(' ') + 1);
                command.erase(command.size() - 1);
                db.AddEvent(MakeDate(date), command);
            }

        }
    }
    catch (std::runtime_error& err) {
        std::cout <<err.what()<<'\n';
    }

    return 0;
}