#include "controller/filters/artistfilter.h"

ArtistFilter::ArtistFilter(const QString& artistPattern)
    : m_artistPattern(artistPattern), m_caseSensitive(false) {
}

bool ArtistFilter::apply(const Artwork& artwork) const {
    return artwork.getArtist().contains(m_artistPattern, m_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
}
