#include "controller/filters/titlefilter.h"

TitleFilter::TitleFilter(const QString& titlePattern)
    : m_titlePattern(titlePattern), m_caseSensitive(false) {
}

bool TitleFilter::apply(const Artwork& artwork) const {
    return artwork.getTitle().contains(m_titlePattern, m_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
