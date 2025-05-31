#include "controller/artcontroller.h"
#include "controller/command/addcommand.h"
#include "controller/command/removecommand.h"
#include "controller/command/updatecommand.h"

ArtController::ArtController(std::shared_ptr<Repository> repository, QObject* parent)
    : QObject(parent), m_repository(repository) {
}

void ArtController::executeCommand(std::shared_ptr<Command> command) {
    command->execute();
    m_undoStack.push(command);
    m_redoStack.clear();

    emit artworksChanged();
    emit undoRedoStateChanged();
}

bool ArtController::canUndo() const {
    return !m_undoStack.isEmpty();
}

bool ArtController::canRedo() const {
    return !m_redoStack.isEmpty();
}

void ArtController::undo() {
    if (canUndo()) {
        auto command = m_undoStack.pop();
        command->undo();
        m_redoStack.push(command);

        emit artworksChanged();
        emit undoRedoStateChanged();
    }
}

void ArtController::redo() {
    if (canRedo()) {
        auto command = m_redoStack.pop();
        command->execute();
        m_undoStack.push(command);

        emit artworksChanged();
        emit undoRedoStateChanged();
    }
}

void ArtController::addArtwork(const Artwork& artwork) {
    // Create a copy of the artwork to modify
    Artwork artworkToAdd = artwork;
    
    // Generate an ID if one doesn't exist
    if (artworkToAdd.getId().isEmpty()) {
        artworkToAdd.setId(generateArtworkId());
    }
    
    // Create and execute the command
    auto command = std::make_shared<AddCommand>(m_repository, artworkToAdd);
    executeCommand(command);
}

void ArtController::updateArtwork(const Artwork& artwork) {
    auto command = std::make_shared<UpdateCommand>(m_repository, artwork);
    executeCommand(command);
}

void ArtController::removeArtwork(const QString& id) {
    auto command = std::make_shared<RemoveCommand>(m_repository, id);
    executeCommand(command);
}

QVector<Artwork> ArtController::getAllArtworks() const {
    return m_repository->getAll();
}

Artwork ArtController::getArtworkById(const QString& id) const {
    return m_repository->getById(id);
}

bool ArtController::artworkExists(const QString& id) const {
    return m_repository->exists(id);
}

QVector<Artwork> ArtController::filterArtworks(std::shared_ptr<Filter> filter) const {
    return filter->filter(getAllArtworks());
}

QString ArtController::generateArtworkId() const {
    return m_repository->generateId();
}
