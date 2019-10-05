#ifndef MINECRAFT_REPORTEDEXCEPTION_H
#define MINECRAFT_REPORTEDEXCEPTION_H

#include <exception>
#include <string>

class ReportedException : public std::exception
{
public:
    ReportedException(std::string source, std::string message, std::string details)
            : source(source), message(message), details(details)
    {}

    std::string GetSource() const
    { return source; }
    std::string GetMessage() const
    { return message; }
    std::string GetDetails() const
    { return details; }

private:
    std::string source, message, details;
};

#endif //MINECRAFT_REPORTEDEXCEPTION_H
