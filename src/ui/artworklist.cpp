#include "ui/artworklist.h"
#include <QHeaderView>
#include <QVBoxLayout>

ArtworkList::ArtworkList(QWidget* parent)
    : QWidget(parent)
    , m_tableView(new QTableView(this))
    , m_model(new QStandardItemModel(this)) {

    setupUI();
    setupConnections();
}

void ArtworkList::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Configure the model
    QStringList headers;
    headers << "ID" << "Title" << "Artist" << "Medium" << "Creation Date";
    m_model->setHorizontalHeaderLabels(headers);

    // Configure the table view
    m_tableView->setModel(m_model);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    // Hide the ID column
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
    m_model->setRowCount(0); // Clear existing data

    for (const Artwork& artwork : artworks) {
        QList<QStandardItem*> row;

        // Create items for each column
        row.append(new QStandardItem(artwork.getId()));
        row.append(new QStandardItem(artwork.getTitle()));
        row.append(new QStandardItem(artwork.getArtist()));
        row.append(new QStandardItem(artwork.getMedium()));
        row.append(new QStandardItem(artwork.getCreationDate().toString("yyyy-MM-dd")));

        m_model->appendRow(row);
    }

    // Resize columns to content
    m_tableView->resizeColumnsToContents();
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
