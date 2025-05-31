#include "repository/csvrepository.h"
#include <QUuid>
#include <QDateTime>

CSVRepository::CSVRepository(const QString& filePath)
    : m_filePath(filePath) {
    load();
}

CSVRepository::~CSVRepository() {
    save();
}

void CSVRepository::add(const Artwork& artwork) {
    if (exists(artwork.getId())) {
        throw std::runtime_error("Artwork with this ID already exists");
    }

    m_artworks.append(artwork);
    save();
}

void CSVRepository::update(const Artwork& artwork) {
    for (int i = 0; i < m_artworks.size(); ++i) {
        if (m_artworks[i].getId() == artwork.getId()) {
            m_artworks[i] = artwork;
            save();
            return;
        }
    }

    throw std::runtime_error("Artwork not found");
}

void CSVRepository::remove(const QString& id) {
    for (int i = 0; i < m_artworks.size(); ++i) {
        if (m_artworks[i].getId() == id) {
            m_artworks.remove(i);
            save();
            return;
        }
    }

    throw std::runtime_error("Artwork not found");
}

Artwork CSVRepository::getById(const QString& id) const {
    for (const Artwork& artwork : m_artworks) {
        if (artwork.getId() == id) {
            return artwork;
        }
    }

    throw std::runtime_error("Artwork not found");
}

QVector<Artwork> CSVRepository::getAll() const {
    return m_artworks;
}

bool CSVRepository::load() {
    m_artworks.clear();

    QFile file(m_filePath);
    if (!file.exists()) {
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);

    // Skip header line
    if (!in.atEnd()) {
        in.readLine();
    }

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        // Ensure we have the right number of fields
        if (fields.size() >= 7) {
            m_artworks.append(stringListToArtwork(fields));
        }
    }

    file.close();
    return true;
}

bool CSVRepository::save() {
    QFile file(m_filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);

    // Write header
    out << "ID,Title,Artist,Medium,CreationDate,Description,ImageURL\n";

    // Write data
    for (const Artwork& artwork : m_artworks) {
        QStringList fields = artworkToStringList(artwork);
        out << fields.join(",") << "\n";
    }

    file.close();
    return true;
}

bool CSVRepository::exists(const QString& id) const {
    for (const Artwork& artwork : m_artworks) {
        if (artwork.getId() == id) {
            return true;
        }
    }

    return false;
}

int CSVRepository::count() const {
    return m_artworks.size();
}

QString CSVRepository::generateId() const {
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QStringList CSVRepository::artworkToStringList(const Artwork& artwork) const {
    QStringList fields;
    fields << artwork.getId()
           << artwork.getTitle()
           << artwork.getArtist()
           << artwork.getMedium()
           << artwork.getCreationDate().toString("yyyy-MM-dd")
           << artwork.getDescription()
           << artwork.getImageUrl();

    return fields;
}

Artwork CSVRepository::stringListToArtwork(const QStringList& fields) const {
    QString id = fields[0];
    QString title = fields[1];
    QString artist = fields[2];
    QString medium = fields[3];
    QDate creationDate = QDate::fromString(fields[4], "yyyy-MM-dd");
    QString description = fields[5];
    QString imageUrl = fields[6];

    return Artwork(id, title, artist, medium, creationDate, description, imageUrl);
}
