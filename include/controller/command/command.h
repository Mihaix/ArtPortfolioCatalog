#ifndef COMMAND_H
#define COMMAND_H

#include <QString>

// Command Pattern: Encapsulate a request as an object
class Command {
public:
    virtual ~Command() = default;

    // Execute the command
    virtual void execute() = 0;

    // Undo the command
    virtual void undo() = 0;

    // Get a description of this command (for UI display)
    virtual QString description() const = 0;
};

#endif // COMMAND_H
