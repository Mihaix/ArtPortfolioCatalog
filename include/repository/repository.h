#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "models/artwork.h"
#include <QVector>
#include <memory>
#include <QString>
#include <QStringList>

class Repository {
public:
    virtual ~Repository() = default;

    virtual void add(const Artwork& artwork) = 0;
    virtual void update(const Artwork& artwork) = 0;
    virtual void remove(const QString& id) = 0;
    virtual Artwork getById(const QString& id) const = 0;
    virtual QVector<Artwork> getAll() const = 0;

    virtual bool load() = 0;
    virtual bool save() = 0;

    virtual bool exists(const QString& id) const = 0;
    virtual int count() const = 0;
    virtual QString generateId() const = 0;
};

#endif // REPOSITORY_H
