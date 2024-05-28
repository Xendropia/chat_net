#ifndef CHAT_H
#define CHAT_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <unordered_map>

template<typename T>
class User {
public:
    User(T username, T password, T name);
    const T& getUsername() const;
    const T& getPassword() const;
    const T& getName() const;
    std::vector<std::pair<std::string, T>>& getMessages();

private:
    T username;
    T password;
    T name;
    std::vector<std::pair<std::string, T>> messages; // pair of timestamp and message
};

template<typename T>
class Chat {
public:
    void registerUser(const T& username, const T& password, const T& name);
    User<T>* loginUser(const T& username, const T& password);
    void sendMessage(User<T>& sender, User<T>& receiver, const T& message);
    void broadcastMessage(User<T>& sender, const T& message);
    const std::vector<User<T>>& getUsers() const;

private:
    std::vector<User<T>> users;
    User<T>* currentUser;
    // You can add socket related members here if needed
};

#endif // CHAT_H