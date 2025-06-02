#ifndef CSVREPOSITORY_H
#define CSVREPOSITORY_H

#include "repository/repository.h"
#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>

class CSVRepository : public Repository {
public:
    explicit CSVRepository(const QString& filePath);
    ~CSVRepository() override;

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

    QStringList artworkToStringList(const Artwork& artwork) const;
    Artwork stringListToArtwork(const QStringList& fields) const;
};

#endif // CSVREPOSITORY_H
