#ifndef TITLEFILTER_H
#define TITLEFILTER_H

#include "controller/filters/filter.h"
#include <QString>

class TitleFilter : public Filter {
public:
    explicit TitleFilter(const QString& titlePattern);

    bool apply(const Artwork& artwork) const override;

private:
    QString m_titlePattern;
    bool m_caseSensitive;
};

#endif // TITLEFILTER_H
