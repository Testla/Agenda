#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "Storage.h"

using std::map;
using std::string;

Storage* Storage::instance_ = 0;

User string_to_user(const string& str);
map<string, string>* JSON_parse(const string& str);
Meeting string_to_meeting(const string& str);

Storage::Storage() {
    readFromFile("agenda.data");
}

bool Storage::readFromFile(const char *fpath) {
    std::fstream input_file(fpath, std::ios::in);
    int n;
    std::string str, in;

    if (!input_file)
        return false;

    while (!input_file.eof() || str.find('}') != string::npos) {
        while (!input_file.eof() && str.find('}') == string::npos) {
            getline(input_file, in);
            str += in;
        }
        if (input_file.eof() && str.find('}') == string::npos)
            break;
        // get the number of users of meetings
        std::stringstream temp(str.substr(str.find("total:") \
                                + std::string("total:").length()));
        temp >> n;

        if (str.substr(0, str.find('}') + 1).find("User")
        != std::string::npos) {
            str.erase(0, str.find('}') + 1);
            for (int i = 1; i <= n; ++i) {
                while (str.find('}') == string::npos) {
                    getline(input_file, in);
                    str += in;
                }
                createUser(string_to_user(str.substr(0, str.find('}') + 1)));
                str.erase(0, str.find('}') + 1);
            }
        } else if (str.substr(0, str.find('}') + 1).find("Meeting")
        != std::string::npos) {
            str.erase(0, str.find('}') + 1);
            for (int i = 1; i <= n; ++i) {
                while (str.find('}') == string::npos) {
                    getline(input_file, in);
                    str += in;
                }
                createMeeting(string_to_meeting(str));
                str.erase(0, str.find('}') + 1);
            }
        }  // otherwise, exception
    }
    input_file.close();
    return true;
}

User string_to_user(const string& str) {
    map<string, string>* member_values = JSON_parse(str);
    return User((*member_values)["name"],
                (*member_values)["password"],
                (*member_values)["email"],
                (*member_values)["phone"]);
}

Meeting string_to_meeting(const string& str) {
    map<string, string>* member_values = JSON_parse(str);
    return Meeting((*member_values)["sponsor"],
                   (*member_values)["participator"],
                   Date::stringToDate((*member_values)["sdate"]),
                   Date::stringToDate((*member_values)["edate"]),
                   (*member_values)["title"]);
}

map<string, string>* JSON_parse(const string& str) {
    string key, value;
    map<string, string>* result = new map<string, string>;
    std::stringstream ss;
    // pcolon is ':', pos1 is the first '\"', pos2 is the second '\"'
    // pcomma is ','
    size_t pcolon, pos1, pos2, pcomma = str.find('{');
    while ((pcolon = str.find(':', pcomma + 1)) != string::npos) {
        pos1 = str.find('\"', pcolon + 1);
        pos2 = str.find('\"', pos1 + 1);

        ss.clear();
        ss << str.substr(pcomma + 1, pcolon - (pcomma + 1));
        ss >> key;

        value = str.substr(pos1 + 1, pos2 - (pos1 + 1));
        (*result)[key] = value;

        pcomma = str.find(',', pos2 + 1);
        if (pcomma == string::npos)  // reaches
            pcomma = str.size();
    }
    return result;
}

bool Storage::writeToFile(const char *fpath) {
    std::fstream output_file(fpath, std::ios::out);

    if (!output_file)
        return false;

    output_file << '{' << "collection:\"User\",total:"
    << userList_.size() << "}\n";
    for (auto it = userList_.begin(); it != userList_.end(); ++it) {
    output_file << "{name:\"" << it->getName()
       << "\",password:\"" << it->getPassword()
       << "\",email:\"" << it->getEmail()
       << "\",phone:\"" << it->getPhone() << "\"}\n";
    }
    output_file << "{collection:\"Meeting\",total:"
    << meetingList_.size() << "}\n";
    for (auto it = meetingList_.begin(); it != meetingList_.end(); ++it) {
    output_file << "{sponsor:\"" << it->getSponsor()
       << "\",participator:\"" << it->getParticipator()
       << "\",sdate:\"" << Date::dateToString(it->getStartDate())
       << "\",edate:\"" << Date::dateToString(it->getEndDate())
       << "\",title:\"" << it->getTitle() << "\"}\n";
    }
    output_file.close();
    return true;
}

Storage* Storage::getInstance(void) {
    if (Storage::instance_ == 0)
        Storage::instance_ = new Storage();
    return Storage::instance_;
}

Storage::~Storage() {
    Storage::instance_->writeToFile("agenda.data");
    Storage::instance_ = 0;
}

void Storage::createUser(const User& new_user) {
    userList_.push_back(new_user);
}

std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) {
    std::list<User> result;
    for (auto it = userList_.begin(); it != userList_.end(); ++it)
        if (filter(*it) == true)
            result.push_back(*it);
    return result;
}

int Storage::updateUser(std::function<bool(const User&)> filter,
std::function<void(User&)> switcher) {
    int count = 0;
    for (auto it = userList_.begin(); it != userList_.end(); ++it)
        if (filter(*it) == true) {
            switcher(*it);
            ++count;
        }
    return count;
}

int Storage::deleteUser(std::function<bool(const User&)> filter) {
    int count = 0;
    for (auto it = userList_.begin(); it != userList_.end();)
        if (filter(*it) == true) {
            userList_.erase(it++);
            ++count;
        } else {
            ++it;
        }
    return count;
}

void Storage::createMeeting(const Meeting& new_meeting) {
    meetingList_.push_back(new_meeting);
}

std::list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting&)> \
filter) {
    std::list<Meeting> result;
    for (auto it = meetingList_.begin(); it != meetingList_.end(); ++it)
        if (filter(*it) == true)
            result.push_back(*it);
    return result;
}

int Storage::updateMeeting(std::function<bool(const Meeting&)> filter, \
                  std::function<void(Meeting&)> switcher) {
    int count = 0;
    for (auto it = meetingList_.begin(); it != meetingList_.end(); ++it)
        if (filter(*it) == true) {
            switcher(*it);
            ++count;
        }
    return count;
}

int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
    int count = 0;
    for (auto it = meetingList_.begin(); it != meetingList_.end();)
        if (filter(*it) == true) {
            meetingList_.erase(it++);
            ++count;
        } else {
            ++it;
        }
    return count;
}

//  File IO

bool Storage::sync(void) {
    return Storage::instance_->writeToFile("agenda.data");
}

