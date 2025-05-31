#ifndef JSONREPOSITORY_H
#define JSONREPOSITORY_H

#include "repository/repository.h"
#include <QString>
#include <QVector>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class JSONRepository : public Repository {
public:
    explicit JSONRepository(const QString& filePath);
    ~JSONRepository() override;

    // CRUD operations
    void add(const Artwork& artwork) override;
    void update(const Artwork& artwork) override;
    void remove(const QString& id) override;
    Artwork getById(const QString& id) const override;
    QVector<Artwork> getAll() const override;

    // File operations
    bool load() override;
    bool save() override;

    // Utility methods
    bool exists(const QString& id) const override;
    int count() const override;
    QString generateId() const override;

private:
    QString m_filePath;
    QVector<Artwork> m_artworks;

    // Helper methods
    QJsonObject artworkToJson(const Artwork& artwork) const;
    Artwork jsonToArtwork(const QJsonObject& jsonObj) const;
};

#endif // JSONREPOSITORY_H
