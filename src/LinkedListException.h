#ifndef LAB4_LINKEDLISTEXCEPTION_H
#define LAB4_LINKEDLISTEXCEPTION_H

#include <exception>
#include <string>

class LinkedListException : public std::exception{
    private:
        std::string exceptionText;
    public:
        explicit LinkedListException(std::string error): exceptionText(move(error)){};
        [[nodiscard]] const char* what() const noexcept override { return exceptionText.c_str();}
};

#endif //LAB4_LINKEDLISTEXCEPTION_H
