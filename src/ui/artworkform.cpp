#include "ui/artworkform.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

ArtworkForm::ArtworkForm(QWidget* parent)
    : QWidget(parent)
    , m_titleEdit(new QLineEdit(this))
    , m_artistEdit(new QLineEdit(this))
    , m_mediumEdit(new QLineEdit(this))
    , m_dateEdit(new QDateEdit(this))
    , m_descriptionEdit(new QTextEdit(this))
    , m_imageUrlEdit(new QLineEdit(this))
    , m_buttonBox(new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this)) {

    setupUI();
    setupConnections();
}

void ArtworkForm::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QFormLayout* formLayout = new QFormLayout();

    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDate(QDate::currentDate());

    formLayout->addRow("Title:", m_titleEdit);
    formLayout->addRow("Artist:", m_artistEdit);
    formLayout->addRow("Medium:", m_mediumEdit);
    formLayout->addRow("Creation Date:", m_dateEdit);
    formLayout->addRow("Description:", m_descriptionEdit);
    formLayout->addRow("Image URL:", m_imageUrlEdit);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(m_buttonBox);

    setLayout(mainLayout);
}

void ArtworkForm::setupConnections() {
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &ArtworkForm::onSaveClicked);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &ArtworkForm::onCancelClicked);
}

void ArtworkForm::setArtwork(const Artwork& artwork) {
    m_artworkId = artwork.getId();
    m_titleEdit->setText(artwork.getTitle());
    m_artistEdit->setText(artwork.getArtist());
    m_mediumEdit->setText(artwork.getMedium());
    m_dateEdit->setDate(artwork.getCreationDate());
    m_descriptionEdit->setText(artwork.getDescription());
    m_imageUrlEdit->setText(artwork.getImageUrl());
}

Artwork ArtworkForm::getArtwork() const {
    return Artwork(
        m_artworkId,
        m_titleEdit->text(),
        m_artistEdit->text(),
        m_mediumEdit->text(),
        m_dateEdit->date(),
        m_descriptionEdit->toPlainText(),
        m_imageUrlEdit->text()
        );
}

void ArtworkForm::clear() {
    m_artworkId = QString();
    m_titleEdit->clear();
    m_artistEdit->clear();
    m_mediumEdit->clear();
    m_dateEdit->setDate(QDate::currentDate());
    m_descriptionEdit->clear();
    m_imageUrlEdit->clear();
}

bool ArtworkForm::validateForm() {
    if (m_titleEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Title cannot be empty.");
        m_titleEdit->setFocus();
        return false;
    }

    if (m_artistEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Artist cannot be empty.");
        m_artistEdit->setFocus();
        return false;
    }

    return true;
}

void ArtworkForm::onSaveClicked() {
    if (validateForm()) {
        emit saveClicked(getArtwork());
    }
}

void ArtworkForm::onCancelClicked() {
    emit cancelClicked();
}
