#include <iostream>
#include <string>
#include <list>
#include <iomanip>
#include "AgendaUI.h"

using std::cin;
using std::cout;
using std::endl;

AgendaUI::AgendaUI() {
}

void AgendaUI::OperationLoop(void) {
	bool res = true;
	showTips();
    while (res) {
    	if (userName_ == "")
    	     cout << "Agenda : ~$ ";
   		else
   			cout << "Agenda@" << userName_ << " : # ";
        res = executeOperation(getOperation());
    }
}

void AgendaUI::startAgenda() {
}

std::string AgendaUI::getOperation() {
    std::string op;
    cin >> op;
    return op;
}

bool AgendaUI::executeOperation(std::string op) {
    if (userName_ == "") {
        if (op == "l") {
            userLogIn();
        } else if (op == "r") {
            userRegister();
        } else if (op == "q") {
            quitAgenda();
            return false;
        } else if (op == "help") {
            showTips();
        } else {
        	cout << "wrong command, you may type \"help\" to get some tips"
        		 << endl;
        }
    } else {
        if (op == "o") {
            userLogOut();
        } else if (op == "dc") {
            deleteUser();
        } else if (op == "lu") {
            listAllUsers();
        } else if (op == "cm") {
            createMeeting();
        } else if (op == "la") {
            listAllMeetings();
        } else if (op == "las") {
            listAllSponsorMeetings();
        } else if (op == "lap") {
            listAllParticipateMeetings();
        } else if (op == "qm") {
            queryMeetingByTitle();
        } else if (op == "qt") {
            queryMeetingByTimeInterval();
        } else if (op == "dm") {
            deleteMeetingByTitle();
        } else if (op == "da") {
            deleteAllMeetings();
        } else if (op == "help") {
            showTips();
        } else {
        	cout << "wrong command, you may type \"help\" to get some tips"
        		 << endl;
        }
    }
    return true;
}

void AgendaUI::userLogIn(void) {
    cout << "[log in] " << "[user name] [password]"<< endl
         << "[log in] ";
    std::string userName, password;
    cin >> userName >> password;
    if (agendaService_.userLogIn(userName, password)) {
        cout << "[log in] " << "succeed!" << endl;
        userName_ = userName;
        userPassword_ = password;
    	showTips();
    } else {
        cout << "[error] " << "log in fail!" << endl;
    }
}

void AgendaUI::userRegister(void) {
    std::string userName, password, email, phone;
    cout << "[register] " << "[user name] [password] [email] [phone]" << endl
         << "[register] ";
    cin >> userName >> password >> email >> phone;
    if (agendaService_.userRegister(userName, password, email, phone))
        cout << "[register] " << "succeed!" << endl;
    else
        cout << "[error] " << "register fail!" << endl;
}

void AgendaUI::quitAgenda(void) {
}

void AgendaUI::userLogOut(void) {
    userName_ = "";
    userPassword_ = "";
	cout << "[log out] " << "succeed!" << endl;
	showTips();
}

void AgendaUI::deleteUser(void) {
    if (agendaService_.deleteUser(userName_, userPassword_)) {
		userName_ = "";
		userPassword_ = "";
        cout << "[delete agenda account] " << "succeed!" << endl;
        showTips();
    } else {
        cout << "[error] " << "delete agenda account fail!" << endl;
    }
}

void AgendaUI::listAllUsers(void) {
    cout << "[list all users] " << endl
         << std::left << std::setw(20) << "name"
         << std::left << std::setw(20) << "email"
         << "phone" << endl;
    std::list<User> userList = agendaService_.listAllUsers();
    for (auto it = userList.begin(); it != userList.end(); ++it)
        cout << std::left << std::setw(20) << it->getName()
             << std::left << std::setw(20) << it->getEmail()
             << it->getPhone() << endl;
}

void AgendaUI::createMeeting(void) {
    std::string title, participator, startTime, endTime;
    cout << "[create meeting] " << "[title] " << "[participator] "
         << "[start time(yyyy-mm-dd/hh:mm)] " << "[end time(yyyy-mm-dd/hh:mm)]"
         << endl << "[create meeting] ";
    cin >> title >> participator >> startTime >> endTime;
    if (agendaService_.createMeeting(userName_,
                                     title,
                                     participator,
                                     startTime,
                                     endTime)) {
        cout << "[create meeting] " << "succeed!" << endl;
    } else {
        cout << "[error] " << "create meeting fail!" << endl;
    }
}

void AgendaUI::listAllMeetings(void) {
    cout << "[list all meetings]" << endl;
    printMeetings(agendaService_.listAllMeetings(userName_));
}

void AgendaUI::listAllSponsorMeetings(void) {
    cout << "[list all sponsor meetings]" << endl;
    printMeetings(agendaService_.listAllSponsorMeetings(userName_));
}

void AgendaUI::listAllParticipateMeetings(void) {
    cout << "[list all participate meetings]" << endl;
    printMeetings(agendaService_.listAllParticipateMeetings(userName_));
}

void AgendaUI::queryMeetingByTitle(void) {
    cout << "[query meeting] " << "[title]" << endl
         << "[query meeting] ";
    std::string title;
    cin >> title;
    printMeetings(agendaService_.meetingQuery(userName_, title));
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    cout << "[query meeting] " << endl
         << "[query meeting] " << "[start time(yyyy-mm-dd/hh:mm)] "
         << "[end time(yyyy-mm-dd/hh:mm)]" << endl;
    std::string startTime, endTime;
    cin >> startTime >> endTime;
    printMeetings(agendaService_.meetingQuery(userName_, startTime, endTime));
}

void AgendaUI::deleteMeetingByTitle(void) {
    cout << "[delete meeting] " << "[title]" << endl
         << "[delete meeting] ";
    std::string title;
    cin >> title;
    if (agendaService_.deleteMeeting(userName_, title))
        cout << "[delete meeting by title] " << "succeed!" << endl;
    else
        cout << "[error] " << "delete meeting fail" << endl;
}

void AgendaUI::deleteAllMeetings(void) {
    if (agendaService_.deleteAllMeetings(userName_))
        cout << "[delete all meetings] " << "succeed!" << endl;
    else
        cout << "[error] " << "delete all meetings fail" << endl;
}

void AgendaUI::printMeetings(std::list<Meeting> meetings) {
    cout << std::left << std::setw(15) << "title"
         << std::left << std::setw(16) << "sponsor"
         << std::left << std::setw(16) << "participator"
         << std::left << std::setw(17) << "start time"
         << "end time" << endl;
    for (auto it = meetings.begin(); it != meetings.end(); ++it)
        cout << std::left << std::setw(15) << it->getTitle()
             << std::left << std::setw(16) << it->getSponsor()
             << std::left << std::setw(16) << it->getParticipator()
             << std::left << std::setw(17) << Date::dateToString(it->getStartDate())
             << Date::dateToString(it->getEndDate()) << endl;
}

void AgendaUI::showTips(void) {
	if (userName_ == "") {
    	cout << "--------------------- Agenda ---------------------" << endl
    	     << "Action:" << endl
    	     << "l - log in Agenda by user name and password" << endl
    	     << "r - register an Agenda account" << endl
    	     << "q - quit Agenda" << endl
    	     << "--------------------------------------------------" << endl;
   	} else {
   		cout << "--------------------- Agenda ---------------------" << endl
    	     << "Action:" << endl
   			 << "o - log out Agenda" << endl
   			 << "dc - delete Agenda account" << endl
   			 << "lu - list all Agenda user" << endl
   			 << "cm - create a meeting" << endl
   			 << "la - list all meetings" << endl
   			 << "las - list all sponsor meetings" << endl
   			 << "lap - list all participate meetings" << endl
   			 << "qm - query meeting by title" << endl
   			 << "qt - query meeting by time interval" << endl
   			 << "dm - delete meeting by title" << endl
   			 << "da - delete all meetings" << endl
   			 << "--------------------------------------------------" << endl;
   	}
}

