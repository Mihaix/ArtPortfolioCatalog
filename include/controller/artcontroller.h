#ifndef ARTCONTROLLER_H
#define ARTCONTROLLER_H

#include "models/artwork.h"
#include "repository/repository.h"
#include "controller/command/command.h"
#include "controller/filters/filter.h"
#include <QVector>
#include <QStack>
#include <memory>
#include <QObject>

class ArtController : public QObject {
    Q_OBJECT

public:
    explicit ArtController(std::shared_ptr<Repository> repository, QObject* parent = nullptr);

    void executeCommand(std::shared_ptr<Command> command);
    bool canUndo() const;
    bool canRedo() const;
    void undo();
    void redo();

    void addArtwork(const Artwork& artwork);
    void updateArtwork(const Artwork& artwork);
    void removeArtwork(const QString& id);

    QVector<Artwork> getAllArtworks() const;
    Artwork getArtworkById(const QString& id) const;
    bool artworkExists(const QString& id) const;

    QVector<Artwork> filterArtworks(std::shared_ptr<Filter> filter) const;

    QString generateArtworkId() const;


signals:
    void artworksChanged();
    void undoRedoStateChanged();

private:
    std::shared_ptr<Repository> m_repository;
    QStack<std::shared_ptr<Command>> m_undoStack;
    QStack<std::shared_ptr<Command>> m_redoStack;
};

#endif // ARTCONTROLLER_H
