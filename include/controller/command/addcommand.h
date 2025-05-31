#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include "controller/command/command.h"
#include "models/artwork.h"
#include "repository/repository.h"
#include <memory>

class AddCommand : public Command {
public:
    AddCommand(std::shared_ptr<Repository> repository, const Artwork& artwork);

    void execute() override;
    void undo() override;
    QString description() const override;

private:
    std::shared_ptr<Repository> m_repository;
    Artwork m_artwork;
    bool m_executed;
};

#endif // ADDCOMMAND_H
