#include <iostream>
#include <string>
#include <unordered_map>

class SessionHandler {
    public:
        static SessionHandler& getInstance() {
            static SessionHandler instance;
            return instance;
        }
        bool isStringEndingSession(const std::string& input) {
            return sessionEndings.find(input) != sessionEndings.end();
        }
        void printValidSessionEndings() {
            std::cout << "Valid session endings: [ ";
            for (const auto& item : sessionEndings) {
                std::cout << item.first << " ";
            }
            std::cout  << "]" << std::endl;
        }
    private:
        SessionHandler() {
            sessionEndings = {
                {"exit", true},
                {"Exit", true},
                {"EXIT", true}
            };
        }
        SessionHandler(const SessionHandler&) = delete;
        SessionHandler& operator=(const SessionHandler&) = delete;
        std::unordered_map<std::string, bool> sessionEndings;
};