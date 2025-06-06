#include "ui/mainwindow.h"
#include "repository/jsonrepository.h"
#include "controller/filters/titlefilter.h"
#include "controller/filters/artistfilter.h"
#include "controller/filters/compositefilter.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QDir>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stackedWidget(new QStackedWidget(this))
    , m_listPage(new QWidget(this))
    , m_formPage(new QWidget(this))
    , m_artworkList(new ArtworkList(this))
    , m_artworkForm(new ArtworkForm(this))
    , m_filterTextEdit(new QLineEdit(this))
    , m_filterTypeComboBox(new QComboBox(this))
    , m_applyFilterButton(new QPushButton("Apply Filter", this))
    , m_useMultipleFilters(new QCheckBox("Use Multiple Filters", this))
    , m_logicalOperator(new QComboBox(this))
    , m_filterTextEdit2(new QLineEdit(this))
    , m_filterTypeComboBox2(new QComboBox(this))
    , m_toolbar(new QToolBar(this))
    , m_statusBar(new QStatusBar(this)) {

    setupRepository();
    setupController();
    setupUI();
    setupActions();
    setupToolbar();
    setupListPage();
    setupFormPage();
    setupConnections();

    showListPage();
    loadArtworks();
    updateUndoRedoActions();

    setWindowTitle("Art Portfolio Catalog");
    setMinimumSize(800, 600);
}

MainWindow::~MainWindow() {

}

void MainWindow::setupRepository() {
    QDir dataDir(QApplication::applicationDirPath() + "/data");
    if (!dataDir.exists()) {
        dataDir.mkpath(".");
    }

    QString filePath = dataDir.absolutePath() + "/artworks.json";
    m_repository = std::make_shared<JSONRepository>(filePath);
}

void MainWindow::setupController() {
    m_controller = std::make_shared<ArtController>(m_repository);
}

void MainWindow::setupUI() {
    setCentralWidget(m_stackedWidget);
    m_stackedWidget->addWidget(m_listPage);
    m_stackedWidget->addWidget(m_formPage);

    setStatusBar(m_statusBar);
    m_statusBar->showMessage("Ready");
}

void MainWindow::setupActions() {
    m_addAction = new QAction(QIcon::fromTheme("list-add"), "Add Artwork", this);
    m_editAction = new QAction(QIcon::fromTheme("document-edit"), "Edit Artwork", this);
    m_removeAction = new QAction(QIcon::fromTheme("list-remove"), "Remove Artwork", this);
    m_undoAction = new QAction(QIcon::fromTheme("edit-undo"), "Undo", this);
    m_redoAction = new QAction(QIcon::fromTheme("edit-redo"), "Redo", this);

    m_addAction->setShortcut(QKeySequence::New);
    m_editAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_E));
    m_removeAction->setShortcut(QKeySequence::Delete);
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_redoAction->setShortcut(QKeySequence::Redo);

    m_editAction->setEnabled(false);
    m_removeAction->setEnabled(false);
    m_undoAction->setEnabled(false);
    m_redoAction->setEnabled(false);
}

void MainWindow::setupToolbar() {
    addToolBar(m_toolbar);
    m_toolbar->addAction(m_addAction);
    m_toolbar->addAction(m_editAction);
    m_toolbar->addAction(m_removeAction);
    m_toolbar->addSeparator();
    m_toolbar->addAction(m_undoAction);
    m_toolbar->addAction(m_redoAction);
}

void MainWindow::setupListPage() {
    QVBoxLayout* mainLayout = new QVBoxLayout(m_listPage);

    QGroupBox* filterGroupBox = new QGroupBox("Filter Options", m_listPage);
    QVBoxLayout* filterGroupLayout = new QVBoxLayout(filterGroupBox);

    QHBoxLayout* filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Filter:"));

    m_filterTypeComboBox->addItem("Title", "title");
    m_filterTypeComboBox->addItem("Artist", "artist");
    filterLayout->addWidget(m_filterTypeComboBox);

    filterLayout->addWidget(m_filterTextEdit);

    QHBoxLayout* multipleFilterLayout = new QHBoxLayout();
    multipleFilterLayout->addWidget(m_useMultipleFilters);

    m_logicalOperator->addItem("AND", "AND");
    m_logicalOperator->addItem("OR", "OR");
    m_logicalOperator->setToolTip("Select how to combine multiple filters");
    multipleFilterLayout->addWidget(new QLabel("Operator:"));
    multipleFilterLayout->addWidget(m_logicalOperator);
    multipleFilterLayout->addStretch();

    QHBoxLayout* filter2Layout = new QHBoxLayout();
    filter2Layout->addWidget(new QLabel("Filter 2:"));

    m_filterTypeComboBox2->addItem("Title", "title");
    m_filterTypeComboBox2->addItem("Artist", "artist");
    filter2Layout->addWidget(m_filterTypeComboBox2);

    filter2Layout->addWidget(m_filterTextEdit2);

    m_logicalOperator->setVisible(false);
    m_filterTypeComboBox2->setVisible(false);
    m_filterTextEdit2->setVisible(false);

    QHBoxLayout* applyLayout = new QHBoxLayout();
    applyLayout->addStretch();
    applyLayout->addWidget(m_applyFilterButton);

    filterGroupLayout->addLayout(filterLayout);
    filterGroupLayout->addLayout(multipleFilterLayout);
    filterGroupLayout->addLayout(filter2Layout);
    filterGroupLayout->addLayout(applyLayout);

    mainLayout->addWidget(filterGroupBox);
    mainLayout->addWidget(m_artworkList);
}

void MainWindow::setupFormPage() {
    QVBoxLayout* layout = new QVBoxLayout(m_formPage);
    layout->addWidget(m_artworkForm);
}

void MainWindow::setupConnections() {
    connect(m_addAction, &QAction::triggered, this, &MainWindow::showAddPage);
    connect(m_editAction, &QAction::triggered, this, &MainWindow::onEditArtwork);
    connect(m_removeAction, &QAction::triggered, this, &MainWindow::onRemoveArtwork);
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::onUndo);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::onRedo);

    connect(m_artworkList, &ArtworkList::artworkSelected, [this](const QString& id) {
        m_editAction->setEnabled(true);
        m_removeAction->setEnabled(true);
    });

    connect(m_artworkList, &ArtworkList::artworkDoubleClicked, this, &MainWindow::showEditPage);

    connect(m_artworkForm, &ArtworkForm::saveClicked, this, &MainWindow::onSaveArtwork);
    connect(m_artworkForm, &ArtworkForm::cancelClicked, this, &MainWindow::onCancelForm);

    connect(m_applyFilterButton, &QPushButton::clicked, this, &MainWindow::onApplyFilter);
    connect(m_filterTextEdit, &QLineEdit::returnPressed, this, &MainWindow::onApplyFilter);
    connect(m_filterTextEdit2, &QLineEdit::returnPressed, this, &MainWindow::onApplyFilter);

    connect(m_useMultipleFilters, &QCheckBox::stateChanged, this, &MainWindow::onUseMultipleFiltersChanged);

    connect(m_controller.get(), &ArtController::artworksChanged, this, &MainWindow::onArtworksChanged);
    connect(m_controller.get(), &ArtController::undoRedoStateChanged, this, &MainWindow::onUndoRedoStateChanged);
}

void MainWindow::onUseMultipleFiltersChanged(int state) {
    bool useMultiple = (state == Qt::Checked);
    m_logicalOperator->setVisible(useMultiple);
    m_filterTypeComboBox2->setVisible(useMultiple);
    m_filterTextEdit2->setVisible(useMultiple);
}

void MainWindow::showListPage() {
    m_stackedWidget->setCurrentWidget(m_listPage);
}

void MainWindow::showAddPage() {
    m_artworkForm->clear();
    m_stackedWidget->setCurrentWidget(m_formPage);
}

void MainWindow::showEditPage(const QString& id) {
    try {
        Artwork artwork = m_controller->getArtworkById(id);
        m_artworkForm->setArtwork(artwork);
        m_stackedWidget->setCurrentWidget(m_formPage);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load artwork: %1").arg(e.what()));
    }
}

void MainWindow::onAddArtwork() {
    showAddPage();
}

void MainWindow::onEditArtwork() {
    if (m_artworkList->hasSelection()) {
        showEditPage(m_artworkList->getSelectedArtworkId());
    }
}

void MainWindow::onRemoveArtwork() {
    if (m_artworkList->hasSelection()) {
        QString id = m_artworkList->getSelectedArtworkId();

        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Confirm Removal",
            "Are you sure you want to remove this artwork?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes) {
            try {
                m_controller->removeArtwork(id);
                m_statusBar->showMessage("Artwork removed successfully", 3000);
            } catch (const std::exception& e) {
                QMessageBox::critical(this, "Error", QString("Failed to remove artwork: %1").arg(e.what()));
            }
        }
    }
}

void MainWindow::onSaveArtwork(const Artwork& artwork) {
    try {
        if (artwork.getId().isEmpty()) {
            Artwork newArtwork = artwork;
            newArtwork.setId(m_controller->generateArtworkId());
            m_controller->addArtwork(newArtwork);
            m_statusBar->showMessage("Artwork added successfully", 3000);
        } else {
            m_controller->updateArtwork(artwork);
            m_statusBar->showMessage("Artwork updated successfully", 3000);
        }

        showListPage();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to save artwork: %1").arg(e.what()));
    }
}

void MainWindow::onCancelForm() {
    showListPage();
}

void MainWindow::onUndo() {
    try {
        m_controller->undo();
        m_statusBar->showMessage("Undo successful", 3000);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to undo: %1").arg(e.what()));
    }
}

void MainWindow::onRedo() {
    try {
        m_controller->redo();
        m_statusBar->showMessage("Redo successful", 3000);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to redo: %1").arg(e.what()));
    }
}

void MainWindow::onApplyFilter() {
    QString filterText1 = m_filterTextEdit->text().trimmed();
    QString filterType1 = m_filterTypeComboBox->currentData().toString();

    if (m_useMultipleFilters->isChecked()) {
        QString filterText2 = m_filterTextEdit2->text().trimmed();
        QString filterType2 = m_filterTypeComboBox2->currentData().toString();
        bool isAndOperator = (m_logicalOperator->currentData().toString() == "AND");

        if (filterText1.isEmpty() && filterText2.isEmpty()) {
            loadArtworks();
            return;
        }

        auto compositeFilter = std::make_shared<CompositeFilter>(
            isAndOperator ? CompositeFilter::AND : CompositeFilter::OR);

        if (!filterText1.isEmpty()) {
            if (filterType1 == "title") {
                compositeFilter->addFilter(std::make_shared<TitleFilter>(filterText1));
            } else {
                compositeFilter->addFilter(std::make_shared<ArtistFilter>(filterText1));
            }
        }
        if (!filterText2.isEmpty()) {
            if (filterType2 == "title") {
                compositeFilter->addFilter(std::make_shared<TitleFilter>(filterText2));
            } else {
                compositeFilter->addFilter(std::make_shared<ArtistFilter>(filterText2));
            }
        }

        QVector<Artwork> filteredArtworks = m_controller->filterArtworks(compositeFilter);
        m_artworkList->setArtworks(filteredArtworks);
        m_statusBar->showMessage(QString("Found %1 artwork(s)").arg(filteredArtworks.size()), 3000);
    }
    else {
        if (filterText1.isEmpty()) {
            loadArtworks();
            return;
        }

        std::shared_ptr<Filter> filter;
        if (filterType1 == "title") {
            filter = std::make_shared<TitleFilter>(filterText1);
        } else {
            filter = std::make_shared<ArtistFilter>(filterText1);
        }

        QVector<Artwork> filteredArtworks = m_controller->filterArtworks(filter);
        m_artworkList->setArtworks(filteredArtworks);
        m_statusBar->showMessage(QString("Found %1 artwork(s)").arg(filteredArtworks.size()), 3000);
    }
}

void MainWindow::onArtworksChanged() {
    loadArtworks();
}

void MainWindow::onUndoRedoStateChanged() {
    updateUndoRedoActions();
}

void MainWindow::loadArtworks() {
    try {
        QVector<Artwork> artworks = m_controller->getAllArtworks();
        m_artworkList->setArtworks(artworks);
        m_statusBar->showMessage(QString("Loaded %1 artwork(s)").arg(artworks.size()), 3000);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load artworks: %1").arg(e.what()));
    }
}

void MainWindow::updateUndoRedoActions() {
    m_undoAction->setEnabled(m_controller->canUndo());
    m_redoAction->setEnabled(m_controller->canRedo());
}
