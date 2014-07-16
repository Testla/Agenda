#include <iostream>
#include <string>
#include <list>
#include <iomanip>
#include "AgendaUI.h"

using std::endl;

extern pthread_mutex_t charge;
extern pthread_mutex_t onStart;

void delay(int ms) {
    struct timeval tv;
    tv.tv_sec = ms / 1000;
    tv.tv_usec = 1000 * (ms % 1000);
    select(0, 0, 0, 0, &tv);
}

AgendaUI::AgendaUI(std::stringstream* in, std::stringstream* out) {
    input = in;
    output = out;
}

void AgendaUI::OperationLoop(void) {
	bool res = true;
	showTips();
	if (userName_ == "")
	     (*output) << "Agenda : ~$ ";
	else
		(*output) << "Agenda@" << userName_ << " : # ";
    pthread_mutex_lock(&onStart);
    delay(100);  // wait to let the server thread get the mutex first
    while (res) {
        pthread_mutex_lock(&charge);
        delay(10);
        res = executeOperation(getOperation());
    	if (userName_ == "")
    	     (*output) << "Agenda : ~$ ";
   		else
   			(*output) << "Agenda@" << userName_ << " : # ";
        pthread_mutex_unlock(&charge);
        delay(100);
    }
}

void AgendaUI::startAgenda() {
}

std::string AgendaUI::getOperation() {
    std::string op;
    (*input) >> op;
    input->str("");
    input->clear();
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
        	(*output) << "wrong command, you may type \"help\" to get some tips"
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
        	(*output) << "wrong command, you may type \"help\" to get some tips"
        		 << endl;
        }
    }
    return true;
}

void AgendaUI::userLogIn(void) {
    std::string userName, password;
    (*output) << "[log in] " << "[user name] [password]" << endl
              << "[log in] ";
    pthread_mutex_unlock(&charge);
    delay(100);
    pthread_mutex_lock(&charge);
    delay(10);
    std::cout << "the input is " << (*input).str() << std::endl;
    (*input) >> userName >> password;
    std::cout << "the input is " << (*input).str() << std::endl
              << "username : " << userName << std::endl
              << "password : " << password << std::endl;
    input->str("");
    input->clear();
    if (agendaService_.userLogIn(userName, password)) {
        (*output) << "[log in] " << "succeed!" << endl;
        userName_ = userName;
        userPassword_ = password;
    	showTips();
    } else {
        (*output) << "[error] " << "log in fail!" << endl;
    }
}

void AgendaUI::userRegister(void) {
    std::string userName, password, email, phone;
    (*output) << "[register] " << "[user name] [password] [email] [phone]"
              << endl
              << "[register] ";
    pthread_mutex_unlock(&charge);
    delay(100);
    pthread_mutex_lock(&charge);
    delay(10);
    (*input) >> userName >> password >> email >> phone;
    input->str("");
    input->clear();
    if (agendaService_.userRegister(userName, password, email, phone))
        (*output) << "[register] " << "succeed!" << endl;
    else
        (*output) << "[error] " << "register fail!" << endl;
}

void AgendaUI::quitAgenda(void) {
}

void AgendaUI::userLogOut(void) {
    userName_ = "";
    userPassword_ = "";
	(*output) << "[log out] " << "succeed!" << endl;
	showTips();
}

void AgendaUI::deleteUser(void) {
    if (agendaService_.deleteUser(userName_, userPassword_)) {
		userName_ = "";
		userPassword_ = "";
        (*output) << "[delete agenda account] " << "succeed!" << endl;
        showTips();
    } else {
        (*output) << "[error] " << "delete agenda account fail!" << endl;
    }
}

void AgendaUI::listAllUsers(void) {
    (*output) << "[list all users] " << endl
         << std::left << std::setw(20) << "name"
         << std::left << std::setw(20) << "email"
         << "phone" << endl;
    std::list<User> userList = agendaService_.listAllUsers();
    for (auto it = userList.begin(); it != userList.end(); ++it)
        (*output) << std::left << std::setw(20) << it->getName()
             << std::left << std::setw(20) << it->getEmail()
             << it->getPhone() << endl;
}

void AgendaUI::createMeeting(void) {
    std::string title, participator, startTime, endTime;
    (*output) << "[create meeting] " << "[title] " << "[participator] "
         << "[start time(yyyy-mm-dd/hh:mm)] " << "[end time(yyyy-mm-dd/hh:mm)]"
         << endl << "[create meeting] ";
    pthread_mutex_unlock(&charge);
    delay(100);
    pthread_mutex_lock(&charge);
    delay(10);
    (*input) >> title >> participator >> startTime >> endTime;
    input->str("");
    input->clear();
    if (agendaService_.createMeeting(userName_,
                                     title,
                                     participator,
                                     startTime,
                                     endTime)) {
        (*output) << "[create meeting] " << "succeed!" << endl;
    } else {
        (*output) << "[error] " << "create meeting fail!" << endl;
    }
}

void AgendaUI::listAllMeetings(void) {
    (*output) << "[list all meetings]" << endl;
    printMeetings(agendaService_.listAllMeetings(userName_));
}

void AgendaUI::listAllSponsorMeetings(void) {
    (*output) << "[list all sponsor meetings]" << endl;
    printMeetings(agendaService_.listAllSponsorMeetings(userName_));
}

void AgendaUI::listAllParticipateMeetings(void) {
    (*output) << "[list all participate meetings]" << endl;
    printMeetings(agendaService_.listAllParticipateMeetings(userName_));
}

void AgendaUI::queryMeetingByTitle(void) {
    std::string title;
    (*output) << "[query meeting] " << "[title]" << endl
         << "[query meeting] ";
    pthread_mutex_unlock(&charge);
    pthread_mutex_lock(&charge);
    (*input) >> title;
    input->str("");
    input->clear();
    printMeetings(agendaService_.meetingQuery(userName_, title));
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    std::string startTime, endTime;
    (*output) << "[query meeting] " << endl
         << "[query meeting] " << "[start time(yyyy-mm-dd/hh:mm)] "
         << "[end time(yyyy-mm-dd/hh:mm)]" << endl;
    pthread_mutex_unlock(&charge);
    delay(100);
    pthread_mutex_lock(&charge);
    delay(10);
    (*input) >> startTime >> endTime;
    input->str("");
    input->clear();
    printMeetings(agendaService_.meetingQuery(userName_, startTime, endTime));
}

void AgendaUI::deleteMeetingByTitle(void) {
    std::string title;
    (*output) << "[delete meeting] " << "[title]" << endl
         << "[delete meeting] ";
    pthread_mutex_unlock(&charge);
    delay(100);
    pthread_mutex_lock(&charge);
    delay(10);
    (*input) >> title;
    input->str("");
    input->clear();
    if (agendaService_.deleteMeeting(userName_, title))
        (*output) << "[delete meeting by title] " << "succeed!" << endl;
    else
        (*output) << "[error] " << "delete meeting fail" << endl;
    pthread_mutex_unlock(&charge);
}

void AgendaUI::deleteAllMeetings(void) {
    if (agendaService_.deleteAllMeetings(userName_))
        (*output) << "[delete all meetings] " << "succeed!" << endl;
    else
        (*output) << "[error] " << "delete all meetings fail" << endl;
}

void AgendaUI::printMeetings(std::list<Meeting> meetings) {
    (*output) << std::left << std::setw(15) << "title"
         << std::left << std::setw(16) << "sponsor"
         << std::left << std::setw(16) << "participator"
         << std::left << std::setw(17) << "start time"
         << "end time" << endl;
    for (auto it = meetings.begin(); it != meetings.end(); ++it)
        (*output) << std::left << std::setw(15) << it->getTitle()
             << std::left << std::setw(16) << it->getSponsor()
             << std::left << std::setw(16) << it->getParticipator()
             << std::left << std::setw(17) << Date::dateToString(it->getStartDate())
             << Date::dateToString(it->getEndDate()) << endl;
}

void AgendaUI::showTips(void) {
	if (userName_ == "") {
    	(*output) << "--------------------- Agenda ---------------------" << endl
    	     << "Action:" << endl
    	     << "l - log in Agenda by user name and password" << endl
    	     << "r - register an Agenda account" << endl
    	     << "q - quit Agenda" << endl
    	     << "--------------------------------------------------" << endl;
   	} else {
   		(*output) << "--------------------- Agenda ---------------------" << endl
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

