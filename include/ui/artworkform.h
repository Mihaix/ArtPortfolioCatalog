#ifndef ARTWORKFORM_H
#define ARTWORKFORM_H

#include "models/artwork.h"
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QDialogButtonBox>

class ArtworkForm : public QWidget {
    Q_OBJECT

public:
    explicit ArtworkForm(QWidget* parent = nullptr);

    void setArtwork(const Artwork& artwork);
    Artwork getArtwork() const;
    void clear();
    bool validateForm();

signals:
    void saveClicked(const Artwork& artwork);
    void cancelClicked();

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    QString m_artworkId;
    QLineEdit* m_titleEdit;
    QLineEdit* m_artistEdit;
    QLineEdit* m_mediumEdit;
    QDateEdit* m_dateEdit;
    QTextEdit* m_descriptionEdit;
    QLineEdit* m_imageUrlEdit;
    QDialogButtonBox* m_buttonBox;

    void setupUI();
    void setupConnections();
};

#endif // ARTWORKFORM_H
