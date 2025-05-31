#ifndef REMOVECOMMAND_H
#define REMOVECOMMAND_H

#include "controller/command/command.h"
#include "models/artwork.h"
#include "repository/repository.h"
#include <memory>

class RemoveCommand : public Command {
public:
    RemoveCommand(std::shared_ptr<Repository> repository, const QString& artworkId);

    void execute() override;
    void undo() override;
    QString description() const override;

private:
    std::shared_ptr<Repository> m_repository;
    QString m_artworkId;
    Artwork m_removedArtwork;
    bool m_executed;
};

#endif // REMOVECOMMAND_H
