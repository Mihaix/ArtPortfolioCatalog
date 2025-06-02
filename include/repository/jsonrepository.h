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

    void add(const Artwork& artwork) override;
    void update(const Artwork& artwork) override;
    void remove(const QString& id) override;
    Artwork getById(const QString& id) const override;
    QVector<Artwork> getAll() const override;

    bool load() override;
    bool save() override;

    bool exists(const QString& id) const override;
    int count() const override;
    QString generateId() const override;

private:
    QString m_filePath;
    QVector<Artwork> m_artworks;

    QJsonObject artworkToJson(const Artwork& artwork) const;
    Artwork jsonToArtwork(const QJsonObject& jsonObj) const;
};

#endif // JSONREPOSITORY_H
