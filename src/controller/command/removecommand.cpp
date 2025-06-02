#include "controller/command/removecommand.h"

RemoveCommand::RemoveCommand(std::shared_ptr<Repository> repository, const QString& artworkId)
    : m_repository(repository), m_artworkId(artworkId), m_executed(false) {

    if (m_repository->exists(artworkId)) {
        m_removedArtwork = m_repository->getById(artworkId);
    }
}

void RemoveCommand::execute() {
    if (!m_executed && m_repository->exists(m_artworkId)) {
        m_repository->remove(m_artworkId);
        m_executed = true;
    }
}

void RemoveCommand::undo() {
    if (m_executed) {
        m_repository->add(m_removedArtwork);
        m_executed = false;
    }
}

QString RemoveCommand::description() const {
    return QString("Remove artwork: %1 by %2").arg(m_removedArtwork.getTitle(), m_removedArtwork.getArtist());
}
