#include "chat.h"
#include <utility>

// User class implementation
template<typename T>
User<T>::User(T username, T password, T name)
    : username(std::move(username)), password(std::move(password)), name(std::move(name)) {}

template<typename T>
const T& User<T>::getUsername() const { return username; }

template<typename T>
const T& User<T>::getPassword() const { return password; }

template<typename T>
const T& User<T>::getName() const { return name; }

template<typename T>
std::vector<std::pair<std::string, T>>& User<T>::getMessages() { return messages; }

// Chat class implementation
template<typename T>
void Chat<T>::registerUser(const T& username, const T& password, const T& name) {
    // Check if the username already exists. If it does, throw an exception.
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            throw std::invalid_argument("Username already exists. Please choose a different username.");
        }
    }

    // If username is unique, register the user
    users.emplace_back(username, password, name);
}

template<typename T>
User<T>* Chat<T>::loginUser(const T& username, const T& password) {
    for (auto& user : users) {
        if (user.getUsername() == username && user.getPassword() == password) {
            currentUser = &user;

            // Display messages sent to the user
            std::cout << "Login successful. Welcome, " << currentUser->getName() << "!\n";
            std::cout << "Messages:\n";
            for (const auto& message : currentUser->getMessages()) {
                std::cout << message.first << " " << message.second << '\n';
            }
            return currentUser;
        }
    }
    return nullptr;
}

template<typename T>
void Chat<T>::sendMessage(User<T>& sender, User<T>& receiver, const T& message) {
    // Add timestamp to the message
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(timeinfo, "[%H:%M:%S]");
    std::string timestamp = oss.str();

    receiver.getMessages().emplace_back(timestamp, sender.getName() + ": " + message);
}

template<typename T>
void Chat<T>::broadcastMessage(User<T>& sender, const T& message) {
    // Add timestamp to the broadcast message
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(timeinfo, "[%H:%M:%S]");
    std::string timestamp = oss.str();

    for (auto& user : users) {
        if (&user != &sender) {
            user.getMessages().emplace_back(timestamp, sender.getName() + " (Broadcast): " + message);
        }
    }
}

template<typename T>
const std::vector<User<T>>& Chat<T>::getUsers() const {
    return users;
}