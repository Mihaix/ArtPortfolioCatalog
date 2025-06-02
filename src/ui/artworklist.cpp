#include "ui/artworklist.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPixmap>
#include <QLabel>

ArtworkList::ArtworkList(QWidget* parent)
    : QWidget(parent)
    , m_tableView(new QTableView(this))
    , m_model(new QStandardItemModel(this))
    , m_networkManager(new QNetworkAccessManager(this)) {

    setupUI();
    setupConnections();
}

void ArtworkList::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    QStringList headers;
    headers << "ID" << "Image" << "Title" << "Artist" << "Medium" << "Description" << "Creation Date";
    m_model->setHorizontalHeaderLabels(headers);

    m_tableView->setModel(m_model);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->horizontalHeader()->setStretchLastSection(true);

    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    m_tableView->verticalHeader()->setDefaultSectionSize(160);

    m_tableView->hideColumn(getIdColumn());

    layout->addWidget(m_tableView);
    setLayout(layout);
}


void ArtworkList::setupConnections() {
    connect(m_tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &ArtworkList::onSelectionChanged);
    connect(m_tableView, &QTableView::doubleClicked,
            this, &ArtworkList::onDoubleClicked);
}

void ArtworkList::setArtworks(const QVector<Artwork>& artworks) {
    m_artworks = artworks;
    m_model->setRowCount(0);

    for (const Artwork& artwork : artworks) {
        QList<QStandardItem*> row;

        row.append(new QStandardItem(artwork.getId()));

        QStandardItem* imageItem = new QStandardItem();
        imageItem->setData(artwork.getImageUrl(), Qt::UserRole);
        row.append(imageItem);

        row.append(new QStandardItem(artwork.getTitle()));
        row.append(new QStandardItem(artwork.getArtist()));
        row.append(new QStandardItem(artwork.getMedium()));
        row.append(new QStandardItem(artwork.getDescription()));
        row.append(new QStandardItem(artwork.getCreationDate().toString("yyyy-MM-dd")));

        m_model->appendRow(row);

        loadImageForRow(row.at(1), artwork.getImageUrl(), m_model->rowCount() - 1);
    }

    m_tableView->resizeColumnsToContents();

    m_tableView->setColumnWidth(1, 160);
}

void ArtworkList::loadImageForRow(QStandardItem* item, const QString& url, int row) {
    if (url.isEmpty()) {
        return;
    }

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QPixmap pixmap;
            pixmap.loadFromData(reply->readAll());

            QPixmap scaledPixmap = pixmap.scaled(QSize(160, 160),
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation);

            item->setData(scaledPixmap, Qt::DecorationRole);
        }
        reply->deleteLater();
    });
}

QString ArtworkList::getSelectedArtworkId() const {
    QModelIndexList selectedIndexes = m_tableView->selectionModel()->selectedRows();
    if (selectedIndexes.isEmpty()) {
        return QString();
    }

    QModelIndex selectedIndex = selectedIndexes.first();
    return m_model->data(m_model->index(selectedIndex.row(), getIdColumn())).toString();
}

bool ArtworkList::hasSelection() const {
    return !m_tableView->selectionModel()->selectedRows().isEmpty();
}

void ArtworkList::clearSelection() {
    m_tableView->selectionModel()->clearSelection();
}

void ArtworkList::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
    Q_UNUSED(deselected);

    if (!selected.indexes().isEmpty()) {
        QString id = getSelectedArtworkId();
        emit artworkSelected(id);
    }
}

void ArtworkList::onDoubleClicked(const QModelIndex& index) {
    if (index.isValid()) {
        QString id = m_model->data(m_model->index(index.row(), getIdColumn())).toString();
        emit artworkDoubleClicked(id);
    }
}

int ArtworkList::getIdColumn() const {
    return 0;
}
