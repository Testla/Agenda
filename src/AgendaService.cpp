#include <list>
#include <iostream>
#include <cctype>
#include "AgendaService.h"

bool is_valid_date_string(std::string dateString) {
    static const int pnumber[12] = {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15};
    int i;
    if (dateString.length() != 16)
        return false;
    for (i = 0; i < 12; ++i)
        if (!isdigit(dateString[pnumber[i]]))
            return false;
    if (dateString[4] != '-'
    || dateString[7] != '-'
    || dateString[10] != '/'
    || dateString[13] != ':')
        return false;
    if (!Date::isValid(Date::stringToDate(dateString)))
        return false;
    return true;
}

AgendaService::AgendaService() {
    storage_ = Storage::getInstance();
}

AgendaService::~AgendaService() {
    delete storage_;
}

bool AgendaService::userLogIn(std::string userName, std::string password) {
    return (!storage_->queryUser([userName, password](const User& user)->bool {
        return (user.getName() == userName && user.getPassword() == password);
    }).empty());
}

// fails if there exist a user that has the same name
bool AgendaService::userRegister(std::string userName, std::string password,
std::string email, std::string phone) {
    if (storage_->queryUser([userName](const User& user)->bool {
        return (user.getName() == userName);
    }).empty() == true) {
        storage_->createUser(User(userName, password, email, phone));
        return true;
    } else {
        return false;
    }
}

// delete all the meetings that involves the user and the user himself
bool AgendaService::deleteUser(std::string userName, std::string password) {
    // maybe don't need to check the password? just for safe...
    if (userLogIn(userName, password) == true) {
        storage_->deleteMeeting(
        [userName, password](const Meeting& meeting)->bool {
            return (meeting.getSponsor() == userName
                   || meeting.getParticipator() == userName);
        });
        storage_->deleteUser([userName](const User& user)->bool {
            return (user.getName() == userName);
        });
        return true;
    } else {
        return false;
    }
}
// a user can only delete itself

std::list<User> AgendaService::listAllUsers(void) {
    return storage_->queryUser([](const User& user)->bool {return true;});
}

bool AgendaService::createMeeting(std::string userName, std::string title,
std::string participator,
std::string startDate, std::string endDate) {
    // only allows when:
    // 1. the user and the participator haven't joined a meeting that
    // have the same title
    // 2. the time of the meeting is valid
    // 3. both the user and the participator have no meeting during this period
    // 4. the user is not the same as the participator
    // 5. both the user and the participator exists
    if (storage_->queryMeeting(
    [userName, title, participator, startDate, endDate]
    (const Meeting& meeting)->bool {
        return ((meeting.getSponsor() == userName
        || meeting.getSponsor() == participator
        || meeting.getParticipator() == userName
        || meeting.getParticipator() == participator)
        && (meeting.getTitle() == title
        || (!(
        Date::stringToDate(endDate) <= meeting.getStartDate()
        || Date::stringToDate(startDate) >= meeting.getEndDate()))));
    }).empty() == true
    && Date::stringToDate(startDate) < Date::stringToDate(endDate)
    && is_valid_date_string(startDate)
    && is_valid_date_string(endDate)
    && userName != participator
    && storage_->queryUser([userName, participator](const User& user)->bool {
        return (user.getName() == userName || user.getName() == participator);
    }).size() == 2) {
        storage_->createMeeting(
        Meeting(userName, participator,
        Date::stringToDate(startDate), Date::stringToDate(endDate), title));
        return true;
    } else {
        return false;
    }
}

// returns meetings that has a specific title and
// the user sponses or participates
std::list<Meeting> AgendaService::meetingQuery(std::string userName,
std::string title) {
    return storage_->queryMeeting(
    [userName, title](const Meeting& meeting)->bool {
        return ((meeting.getSponsor() == userName
        || meeting.getParticipator() == userName)
        && meeting.getTitle() == title);
    });
}

std::list<Meeting> AgendaService::meetingQuery(std::string userName,
std::string startDate, std::string endDate) {
    // must check if the time period and the two dateStrings are legal
    if  (Date::stringToDate(startDate) <= Date::stringToDate(endDate)
    && is_valid_date_string(startDate)
    && is_valid_date_string(endDate))
        return storage_->queryMeeting(
        [userName, startDate, endDate](const Meeting& meeting)->bool {
            return ((meeting.getSponsor() == userName
                   || meeting.getParticipator() == userName)
                   && (!(
                   // overlap mean the two meetings have a public period
                   Date::stringToDate(endDate) < meeting.getStartDate()
                   || Date::stringToDate(startDate) > meeting.getEndDate())));
        });
    else
        return std::list<Meeting>();
}

std::list<Meeting> AgendaService::listAllMeetings(std::string userName) {
    return storage_->queryMeeting(
    [userName](const Meeting& meeting)->bool {
        return (meeting.getSponsor() == userName
        || meeting.getParticipator() == userName);
    });
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(std::string userName) {
    return storage_->queryMeeting(
    [userName](const Meeting& meeting)->bool {
        return (meeting.getSponsor() == userName);
    });
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
std::string userName) {
    return storage_->queryMeeting(
    [userName](const Meeting& meeting)->bool {
        return (meeting.getParticipator() == userName);
    });
}

// delete one certain meeting
bool AgendaService::deleteMeeting(std::string userName, std::string title) {
    return storage_->deleteMeeting(
    [userName, title](const Meeting& meeting)->bool {
        return (meeting.getSponsor() == userName
        && meeting.getTitle() == title);
    });
}

// delete all the meetings sponsed by the user
bool AgendaService::deleteAllMeetings(std::string userName) {
    return storage_->deleteMeeting(
    [userName](const Meeting& meeting)->bool {
        return meeting.getSponsor() == userName;
    });
}

void AgendaService::startAgenda(void) {
}

void AgendaService::quitAgenda(void) {
}

