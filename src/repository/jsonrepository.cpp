#include "repository/jsonrepository.h"
#include <QFile>
#include <QUuid>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

JSONRepository::JSONRepository(const QString& filePath)
    : m_filePath(filePath) {
    load();
}

JSONRepository::~JSONRepository() {
    save();
}

void JSONRepository::add(const Artwork& artwork) {
    if (exists(artwork.getId())) {
        throw std::runtime_error("Artwork with this ID already exists");
    }

    m_artworks.append(artwork);
    save();
}

void JSONRepository::update(const Artwork& artwork) {
    for (int i = 0; i < m_artworks.size(); ++i) {
        if (m_artworks[i].getId() == artwork.getId()) {
            m_artworks[i] = artwork;
            save();
            return;
        }
    }

    throw std::runtime_error("Artwork not found");
}

void JSONRepository::remove(const QString& id) {
    for (int i = 0; i < m_artworks.size(); ++i) {
        if (m_artworks[i].getId() == id) {
            m_artworks.remove(i);
            save();
            return;
        }
    }

    throw std::runtime_error("Artwork not found");
}

Artwork JSONRepository::getById(const QString& id) const {
    for (const Artwork& artwork : m_artworks) {
        if (artwork.getId() == id) {
            return artwork;
        }
    }

    throw std::runtime_error("Artwork not found");
}

QVector<Artwork> JSONRepository::getAll() const {
    return m_artworks;
}

bool JSONRepository::load() {
    m_artworks.clear();

    QFile file(m_filePath);
    if (!file.exists()) {
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isArray()) {
        return false;
    }

    QJsonArray artworksArray = doc.array();
    for (const QJsonValue& value : artworksArray) {
        if (value.isObject()) {
            QJsonObject artworkObj = value.toObject();
            m_artworks.append(jsonToArtwork(artworkObj));
        }
    }

    return true;
}

bool JSONRepository::save() {
    QJsonArray artworksArray;

    for (const Artwork& artwork : m_artworks) {
        artworksArray.append(artworkToJson(artwork));
    }

    QJsonDocument doc(artworksArray);
    QFile file(m_filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(doc.toJson());
    file.close();

    return true;
}

bool JSONRepository::exists(const QString& id) const {
    for (const Artwork& artwork : m_artworks) {
        if (artwork.getId() == id) {
            return true;
        }
    }

    return false;
}

int JSONRepository::count() const {
    return m_artworks.size();
}

QString JSONRepository::generateId() const {
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QJsonObject JSONRepository::artworkToJson(const Artwork& artwork) const {
    QJsonObject obj;

    obj["id"] = artwork.getId();
    obj["title"] = artwork.getTitle();
    obj["artist"] = artwork.getArtist();
    obj["medium"] = artwork.getMedium();
    obj["creationDate"] = artwork.getCreationDate().toString("yyyy-MM-dd");
    obj["description"] = artwork.getDescription();
    obj["imageUrl"] = artwork.getImageUrl();

    return obj;
}

Artwork JSONRepository::jsonToArtwork(const QJsonObject& jsonObj) const {
    QString id = jsonObj["id"].toString();
    QString title = jsonObj["title"].toString();
    QString artist = jsonObj["artist"].toString();
    QString medium = jsonObj["medium"].toString();
    QDate creationDate = QDate::fromString(jsonObj["creationDate"].toString(), "yyyy-MM-dd");
    QString description = jsonObj["description"].toString();
    QString imageUrl = jsonObj["imageUrl"].toString();

    return Artwork(id, title, artist, medium, creationDate, description, imageUrl);
}
