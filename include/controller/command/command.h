#ifndef COMMAND_H
#define COMMAND_H

#include <QString>

// Command Pattern: Encapsulate a request as an object
class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;

    virtual void undo() = 0;

    virtual QString description() const = 0;
};

#endif // COMMAND_H
