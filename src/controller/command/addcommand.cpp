#include "controller/command/addcommand.h"

AddCommand::AddCommand(std::shared_ptr<Repository> repository, const Artwork& artwork)
    : m_repository(repository), m_artwork(artwork), m_executed(false) {
}

void AddCommand::execute() {
    if (!m_executed) {
        m_repository->add(m_artwork);
        m_executed = true;
    }
}

void AddCommand::undo() {
    if (m_executed) {
        m_repository->remove(m_artwork.getId());
        m_executed = false;
    }
}

QString AddCommand::description() const {
    return QString("Add artwork: %1 by %2").arg(m_artwork.getTitle(), m_artwork.getArtist());
}
