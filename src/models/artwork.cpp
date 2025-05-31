#include "models/artwork.h"

Artwork::Artwork() = default;

Artwork::Artwork(const QString& id, const QString& title, const QString& artist,
                 const QString& medium, const QDate& creationDate,
                 const QString& description, const QString& imageUrl)
    : m_id(id), m_title(title), m_artist(artist), m_medium(medium),
    m_creationDate(creationDate), m_description(description), m_imageUrl(imageUrl) {
}

QString Artwork::getId() const {
    return m_id;
}

QString Artwork::getTitle() const {
    return m_title;
}

QString Artwork::getArtist() const {
    return m_artist;
}

QString Artwork::getMedium() const {
    return m_medium;
}

QDate Artwork::getCreationDate() const {
    return m_creationDate;
}

QString Artwork::getDescription() const {
    return m_description;
}

QString Artwork::getImageUrl() const {
    return m_imageUrl;
}

void Artwork::setId(const QString& id) {
    m_id = id;
}

void Artwork::setTitle(const QString& title) {
    m_title = title;
}

void Artwork::setArtist(const QString& artist) {
    m_artist = artist;
}

void Artwork::setMedium(const QString& medium) {
    m_medium = medium;
}

void Artwork::setCreationDate(const QDate& creationDate) {
    m_creationDate = creationDate;
}

void Artwork::setDescription(const QString& description) {
    m_description = description;
}

void Artwork::setImageUrl(const QString& imageUrl) {
    m_imageUrl = imageUrl;
}

bool Artwork::operator==(const Artwork& other) const {
    return m_id == other.m_id;
}

bool Artwork::operator!=(const Artwork& other) const {
    return !(*this == other);
}
