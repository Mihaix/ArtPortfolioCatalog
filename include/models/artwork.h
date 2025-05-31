#ifndef ARTWORK_H
#define ARTWORK_H

#include <QString>
#include <QDate>

class Artwork {
public:
    Artwork();
    Artwork(const QString& id, const QString& title, const QString& artist,
            const QString& medium, const QDate& creationDate,
            const QString& description, const QString& imageUrl);

    // Getters
    QString getId() const;
    QString getTitle() const;
    QString getArtist() const;
    QString getMedium() const;
    QDate getCreationDate() const;
    QString getDescription() const;
    QString getImageUrl() const;

    // Setters
    void setId(const QString& id);
    void setTitle(const QString& title);
    void setArtist(const QString& artist);
    void setMedium(const QString& medium);
    void setCreationDate(const QDate& creationDate);
    void setDescription(const QString& description);
    void setImageUrl(const QString& imageUrl);

    // For comparison
    bool operator==(const Artwork& other) const;
    bool operator!=(const Artwork& other) const;

private:
    QString m_id;
    QString m_title;
    QString m_artist;
    QString m_medium;
    QDate m_creationDate;
    QString m_description;
    QString m_imageUrl;
};

#endif // ARTWORK_H
