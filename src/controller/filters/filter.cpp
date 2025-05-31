#include "controller/filters/filter.h"

QVector<Artwork> Filter::filter(const QVector<Artwork>& artworks) const {
    QVector<Artwork> result;

    for (const Artwork& artwork : artworks) {
        if (apply(artwork)) {
            result.append(artwork);
        }
    }

    return result;
}
