#ifndef ARTWORKLIST_H
#define ARTWORKLIST_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QItemSelection>
#include <QVector>
#include "models/artwork.h"

class QNetworkAccessManager;

class ArtworkList : public QWidget {
    Q_OBJECT

public:
    explicit ArtworkList(QWidget* parent = nullptr);

    void setArtworks(const QVector<Artwork>& artworks);
    QString getSelectedArtworkId() const;
    bool hasSelection() const;
    void clearSelection();

signals:
    void artworkSelected(const QString& artworkId);
    void artworkDoubleClicked(const QString& artworkId);

private slots:
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void onDoubleClicked(const QModelIndex& index);

private:
    void setupUI();
    void setupConnections();
    void loadImageForRow(QStandardItem* item, const QString& url, int row);
    int getIdColumn() const;

    QTableView* m_tableView;
    QStandardItemModel* m_model;
    QVector<Artwork> m_artworks;
    QNetworkAccessManager* m_networkManager;
};

#endif // ARTWORKLIST_H
