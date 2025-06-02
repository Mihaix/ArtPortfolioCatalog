#include "controller_test.h"
#include "repository/jsonrepository.h"
#include "controller/filters/titlefilter.h"
#include "controller/filters/artistfilter.h"
#include "controller/filters/compositefilter.h"
#include <QDir>
#include <QFile>

Artwork ControllerTest::createArtwork1() {
    return Artwork(
        "",
        testTitle1,
        testArtist1,
        "Oil on Canvas",
        QDate(1889, 6, 1),
        "A famous night scene",
        "http://example.com/starry_night.jpg"
    );
}

Artwork ControllerTest::createArtwork2() {
    return Artwork(
        "",
        testTitle2,
        testArtist2,
        "Oil on Poplar panel",
        QDate(1503, 1, 1),
        "A portrait of a woman",
        "http://example.com/mona_lisa.jpg"
    );
}

void ControllerTest::cleanup() {
    QFile file(testFilePath);
    if (file.exists()) {
        file.remove();
    }
}

void ControllerTest::initTestCase() {
    QDir tempDir = QDir::temp();
    testFilePath = tempDir.absoluteFilePath("controller_test_artworks.json");
    cleanup();

    repository = std::make_shared<JSONRepository>(testFilePath);
    controller = std::make_shared<ArtController>(repository);
}

void ControllerTest::init() {
    cleanup();
    repository = std::make_shared<JSONRepository>(testFilePath);
    controller = std::make_shared<ArtController>(repository);
}

void ControllerTest::cleanupTestCase() {
    cleanup();
}

void ControllerTest::testAddArtwork() {
    Artwork artwork = createArtwork1();
    
    controller->addArtwork(artwork);

    QVector<Artwork> artworks = controller->getAllArtworks();
    QCOMPARE(artworks.size(), 1);
    QCOMPARE(artworks[0].getTitle(), testTitle1);
    QCOMPARE(artworks[0].getArtist(), testArtist1);
    
    QVERIFY(!artworks[0].getId().isEmpty());
}

void ControllerTest::testUpdateArtwork() {
    Artwork artwork = createArtwork1();
    controller->addArtwork(artwork);
    
    QVector<Artwork> artworks = controller->getAllArtworks();
    QCOMPARE(artworks.size(), 1);
    
    Artwork toUpdate = artworks[0];
    QString updatedTitle = "Updated Title";
    toUpdate.setTitle(updatedTitle);
    controller->updateArtwork(toUpdate);
    
    Artwork updated = controller->getArtworkById(toUpdate.getId());
    QCOMPARE(updated.getTitle(), updatedTitle);
    QCOMPARE(updated.getArtist(), testArtist1);
}

void ControllerTest::testRemoveArtwork() {
    Artwork artwork = createArtwork1();
    controller->addArtwork(artwork);
    
    QVector<Artwork> artworks = controller->getAllArtworks();
    QCOMPARE(artworks.size(), 1);
    
    controller->removeArtwork(artworks[0].getId());

    artworks = controller->getAllArtworks();
    QCOMPARE(artworks.size(), 0);
}

void ControllerTest::testUndoRedo() {
    try {
        QVERIFY(!controller->canUndo());
        QVERIFY(!controller->canRedo());

        Artwork artwork = createArtwork1();
        try {
            controller->addArtwork(artwork);
        } catch (const std::exception& e) {
            QFAIL(QString("Exception during addArtwork: %1").arg(e.what()).toUtf8().constData());
        }

        QVERIFY(controller->canUndo());
        QVERIFY(!controller->canRedo());

        try {
            controller->undo();
        } catch (const std::exception& e) {
            QFAIL(QString("Exception during undo: %1").arg(e.what()).toUtf8().constData());
        }

        QCOMPARE(controller->getAllArtworks().size(), 0);
        QVERIFY(!controller->canUndo());
        QVERIFY(controller->canRedo());

        try {
            controller->redo();
        } catch (const std::exception& e) {
            QFAIL(QString("Exception during redo: %1").arg(e.what()).toUtf8().constData());
        }
        
    } catch (const std::exception& e) {
        QFAIL(QString("Unexpected exception: %1").arg(e.what()).toUtf8().constData());
    }
}

void ControllerTest::testFiltering() {
    Artwork artwork1 = createArtwork1();
    Artwork artwork2 = createArtwork2();
    controller->addArtwork(artwork1);
    controller->addArtwork(artwork2);

    auto titleFilter = std::make_shared<TitleFilter>("Starry");
    QVector<Artwork> filteredArtworks = controller->filterArtworks(titleFilter);
    QCOMPARE(filteredArtworks.size(), 1);
    QCOMPARE(filteredArtworks[0].getTitle(), testTitle1);

    auto artistFilter = std::make_shared<ArtistFilter>("da Vinci");
    filteredArtworks = controller->filterArtworks(artistFilter);
    QCOMPARE(filteredArtworks.size(), 1);
    QCOMPARE(filteredArtworks[0].getTitle(), testTitle2);

    auto compositeFilter = std::make_shared<CompositeFilter>(CompositeFilter::OR);
    compositeFilter->addFilter(std::make_shared<TitleFilter>("Mona"));
    compositeFilter->addFilter(std::make_shared<ArtistFilter>("Gogh"));
    filteredArtworks = controller->filterArtworks(compositeFilter);
    QCOMPARE(filteredArtworks.size(), 2);
    
    auto andFilter = std::make_shared<CompositeFilter>(CompositeFilter::AND);
    andFilter->addFilter(std::make_shared<TitleFilter>("Mona"));
    andFilter->addFilter(std::make_shared<ArtistFilter>("da Vinci"));
    filteredArtworks = controller->filterArtworks(andFilter);
    QCOMPARE(filteredArtworks.size(), 1);
}

void ControllerTest::testComplexUndoRedo() {
    Artwork artwork = createArtwork1();
    controller->addArtwork(artwork);

    QVector<Artwork> artworks = controller->getAllArtworks();
    QString artworkId = artworks[0].getId();

    Artwork toUpdate = artworks[0];
    toUpdate.setTitle("Updated Title");
    controller->updateArtwork(toUpdate);

    controller->removeArtwork(artworkId);

    QCOMPARE(controller->getAllArtworks().size(), 0);

    controller->undo();
    QCOMPARE(controller->getAllArtworks().size(), 1);
    QCOMPARE(controller->getArtworkById(artworkId).getTitle(), "Updated Title");

    controller->undo();
    QCOMPARE(controller->getAllArtworks().size(), 1);
    QCOMPARE(controller->getArtworkById(artworkId).getTitle(), testTitle1);

    controller->undo();
    QCOMPARE(controller->getAllArtworks().size(), 0);

    controller->redo();
    controller->redo();
    controller->redo();
    
    QCOMPARE(controller->getAllArtworks().size(), 0);
}
