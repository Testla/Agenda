#include "User.h"

User::User() : name_(""), password_(""), email_(""), phone_("") {}

User::User(std::string userName, std::string userPassword,
std::string userEmail, std::string userPhone) : name_(userName),
                                                password_(userPassword),
                                                email_(userEmail),
                                                phone_(userPhone) {}

std::string User::getName() const {
    return name_;
}

void User::setName(std::string name) {
    name_ = name;
}

std::string User::getPassword() const {
    return password_;
}

void User::setPassword(std::string password) {
    password_ = password;
}

std::string User::getEmail() const {
    return email_;
}

void User::setEmail(std::string email) {
    email_ = email;
}

std::string User::getPhone() const {
    return phone_;
}

void User::setPhone(std::string phone) {
    phone_ = phone;
}

