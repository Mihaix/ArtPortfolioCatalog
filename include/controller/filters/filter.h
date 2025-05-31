#ifndef FILTER_H
#define FILTER_H

#include "models/artwork.h"
#include <QVector>

// Strategy Pattern: Define a family of algorithms, encapsulate each one, and make them interchangeable
class Filter {
public:
    virtual ~Filter() = default;

    // The strategy interface - apply a filtering algorithm
    virtual bool apply(const Artwork& artwork) const = 0;

    // Utility method to filter a collection
    QVector<Artwork> filter(const QVector<Artwork>& artworks) const;
};

#endif // FILTER_H
