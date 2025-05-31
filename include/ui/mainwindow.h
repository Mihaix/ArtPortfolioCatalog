#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controller/artcontroller.h"
#include "ui/artworklist.h"
#include "ui/artworkform.h"
#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <memory>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // UI navigation
    void showListPage();
    void showAddPage();
    void showEditPage(const QString& id);

    // Actions
    void onAddArtwork();
    void onEditArtwork();
    void onRemoveArtwork();
    void onSaveArtwork(const Artwork& artwork);
    void onCancelForm();
    void onUndo();
    void onRedo();

    // Filtering
    void onApplyFilter();

    // Updates from controller
    void onArtworksChanged();
    void onUndoRedoStateChanged();

private:
    // UI Components
    QStackedWidget* m_stackedWidget;
    QWidget* m_listPage;
    QWidget* m_formPage;

    ArtworkList* m_artworkList;
    ArtworkForm* m_artworkForm;

    QLineEdit* m_filterTextEdit;
    QComboBox* m_filterTypeComboBox;
    QPushButton* m_applyFilterButton;

    QToolBar* m_toolbar;
    QStatusBar* m_statusBar;

    // Actions
    QAction* m_addAction;
    QAction* m_editAction;
    QAction* m_removeAction;
    QAction* m_undoAction;
    QAction* m_redoAction;

    // Controller
    std::shared_ptr<ArtController> m_controller;
    std::shared_ptr<Repository> m_repository;

    // UI Setup methods
    void setupRepository();
    void setupController();
    void setupUI();
    void setupActions();
    void setupToolbar();
    void setupListPage();
    void setupFormPage();
    void setupConnections();

    // Helper methods
    void loadArtworks();
    void updateUndoRedoActions();
};

#endif // MAINWINDOW_H
