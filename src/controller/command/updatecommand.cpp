#include "controller/command/updatecommand.h"

UpdateCommand::UpdateCommand(std::shared_ptr<Repository> repository, const Artwork& newArtwork)
    : m_repository(repository), m_newArtwork(newArtwork), m_executed(false) {

    // Store the old artwork state for undo operation
    if (m_repository->exists(newArtwork.getId())) {
        m_oldArtwork = m_repository->getById(newArtwork.getId());
    }
}

void UpdateCommand::execute() {
    if (!m_executed && m_repository->exists(m_newArtwork.getId())) {
        m_repository->update(m_newArtwork);
        m_executed = true;
    }
}

void UpdateCommand::undo() {
    if (m_executed) {
        m_repository->update(m_oldArtwork);
        m_executed = false;
    }
}

QString UpdateCommand::description() const {
    return QString("Update artwork: %1 by %2").arg(m_newArtwork.getTitle(), m_newArtwork.getArtist());
}
