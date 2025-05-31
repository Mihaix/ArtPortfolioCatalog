#ifndef CONTROLLER_TEST_H
#define CONTROLLER_TEST_H

#include <QtTest>
#include "controller/artcontroller.h"
#include "repository/repository.h"

class ControllerTest : public QObject {
    Q_OBJECT

private:
    QString testFilePath;
    std::shared_ptr<Repository> repository;
    std::shared_ptr<ArtController> controller;
    
    QString testTitle1 = "Starry Night";
    QString testTitle2 = "Mona Lisa";
    QString testArtist1 = "Vincent van Gogh";
    QString testArtist2 = "Leonardo da Vinci";
    
    Artwork createArtwork1();
    Artwork createArtwork2();
    void cleanup();

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void testAddArtwork();
    void testUpdateArtwork();
    void testRemoveArtwork();
    void testUndoRedo();
    void testFiltering();
    void testComplexUndoRedo();
};

#endif // CONTROLLER_TEST_H
