#ifndef UPDATECOMMAND_H
#define UPDATECOMMAND_H

#include "controller/command/command.h"
#include "models/artwork.h"
#include "repository/repository.h"
#include <memory>

class UpdateCommand : public Command {
public:
    UpdateCommand(std::shared_ptr<Repository> repository, const Artwork& newArtwork);

    void execute() override;
    void undo() override;
    QString description() const override;

private:
    std::shared_ptr<Repository> m_repository;
    Artwork m_oldArtwork;
    Artwork m_newArtwork;
    bool m_executed;
};

#endif // UPDATECOMMAND_H
