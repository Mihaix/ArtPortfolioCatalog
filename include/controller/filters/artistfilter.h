#ifndef ARTISTFILTER_H
#define ARTISTFILTER_H

#include "controller/filters/filter.h"
#include <QString>

class ArtistFilter : public Filter {
public:
    explicit ArtistFilter(const QString& artistPattern);

    bool apply(const Artwork& artwork) const override;

private:
    QString m_artistPattern;
    bool m_caseSensitive;
};

#endif // ARTISTFILTER_H
