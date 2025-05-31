#ifndef ARTWORKLIST_H
#define ARTWORKLIST_H

#include "models/artwork.h"
#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QVector>
#include <QItemSelectionModel>

class ArtworkList : public QWidget {
    Q_OBJECT

public:
    explicit ArtworkList(QWidget* parent = nullptr);

    void setArtworks(const QVector<Artwork>& artworks);
    QString getSelectedArtworkId() const;
    bool hasSelection() const;
    void clearSelection();

signals:
    void artworkSelected(const QString& id);
    void artworkDoubleClicked(const QString& id);

private slots:
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void onDoubleClicked(const QModelIndex& index);

private:
    QTableView* m_tableView;
    QStandardItemModel* m_model;
    QVector<Artwork> m_artworks;

    void setupUI();
    void setupConnections();
    int getIdColumn() const { return 0; }
};

#endif // ARTWORKLIST_H
