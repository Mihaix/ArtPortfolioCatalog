#include "controller_test.h"
#include "repository/jsonrepository.h"
#include "controller/filters/titlefilter.h"
#include "controller/filters/artistfilter.h"
#include "controller/filters/compositefilter.h"
#include <QDir>
#include <QFile>

Artwork ControllerTest::createArtwork1() {
    return Artwork(
        "",  // ID will be generated
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
        "",  // ID will be generated
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
    // Create a temporary file for testing
    QDir tempDir = QDir::temp();
    testFilePath = tempDir.absoluteFilePath("controller_test_artworks.json");
    cleanup();
    
    // Initialize repository and controller
    repository = std::make_shared<JSONRepository>(testFilePath);
    controller = std::make_shared<ArtController>(repository);
}

void ControllerTest::init() {
    // Reset repository before each test
    cleanup();
    repository = std::make_shared<JSONRepository>(testFilePath);
    controller = std::make_shared<ArtController>(repository);
}

void ControllerTest::cleanupTestCase() {
    cleanup();
}

void ControllerTest::testAddArtwork() {
    Artwork artwork = createArtwork1();
    
    // Add artwork should generate an ID
    controller->addArtwork(artwork);
    
    // Verify artwork was added
    QVector<Artwork> artworks = controller->getAllArtworks();
    QCOMPARE(artworks.size(), 1);
    QCOMPARE(artworks[0].getTitle(), testTitle1);
    QCOMPARE(artworks[0].getArtist(), testArtist1);
    
    // The ID should have been generated
    QVERIFY(!artworks[0].getId().isEmpty());
}

void ControllerTest::testUpdateArtwork() {
    // First add an artwork
    Artwork artwork = createArtwork1();
    controller->addArtwork(artwork);
    
    // Get the artwork with the generated ID
    QVector<Artwork> artworks = controller->getAllArtworks();
    QCOMPARE(artworks.size(), 1);
    
    // Update the artwork
    Artwork toUpdate = artworks[0];
    QString updatedTitle = "Updated Title";
    toUpdate.setTitle(updatedTitle);
    controller->updateArtwork(toUpdate);
    
    // Verify the update
    Artwork updated = controller->getArtworkById(toUpdate.getId());
    QCOMPARE(updated.getTitle(), updatedTitle);
    QCOMPARE(updated.getArtist(), testArtist1); // Unchanged field
}

void ControllerTest::testRemoveArtwork() {
    // First add an artwork
    Artwork artwork = createArtwork1();
    controller->addArtwork(artwork);
    
    // Get the artwork with the generated ID
    QVector<Artwork> artworks = controller->getAllArtworks();
    QCOMPARE(artworks.size(), 1);
    
    // Remove the artwork
    controller->removeArtwork(artworks[0].getId());
    
    // Verify the removal
    artworks = controller->getAllArtworks();
    QCOMPARE(artworks.size(), 0);
}

void ControllerTest::testUndoRedo() {
    try {
        // Initial state
        QVERIFY(!controller->canUndo());
        QVERIFY(!controller->canRedo());
        
        // Add an artwork
        Artwork artwork = createArtwork1();
        try {
            controller->addArtwork(artwork);
        } catch (const std::exception& e) {
            QFAIL(QString("Exception during addArtwork: %1").arg(e.what()).toUtf8().constData());
        }
        
        // Should be able to undo now
        QVERIFY(controller->canUndo());
        QVERIFY(!controller->canRedo());
        
        // Undo the add
        try {
            controller->undo();
        } catch (const std::exception& e) {
            QFAIL(QString("Exception during undo: %1").arg(e.what()).toUtf8().constData());
        }
        
        // Verify the undo worked
        QCOMPARE(controller->getAllArtworks().size(), 0);
        QVERIFY(!controller->canUndo());
        QVERIFY(controller->canRedo());
        
        // Redo the add
        try {
            controller->redo();
        } catch (const std::exception& e) {
            QFAIL(QString("Exception during redo: %1").arg(e.what()).toUtf8().constData());
        }
        
        // Rest of the test with try-catch blocks...
        // (Add similar try-catch blocks around the remaining operations)
    } catch (const std::exception& e) {
        QFAIL(QString("Unexpected exception: %1").arg(e.what()).toUtf8().constData());
    }
}

void ControllerTest::testFiltering() {
    // Add two artworks
    Artwork artwork1 = createArtwork1();
    Artwork artwork2 = createArtwork2();
    controller->addArtwork(artwork1);
    controller->addArtwork(artwork2);
    
    // Test title filter
    auto titleFilter = std::make_shared<TitleFilter>("Starry");
    QVector<Artwork> filteredArtworks = controller->filterArtworks(titleFilter);
    QCOMPARE(filteredArtworks.size(), 1);
    QCOMPARE(filteredArtworks[0].getTitle(), testTitle1);
    
    // Test artist filter
    auto artistFilter = std::make_shared<ArtistFilter>("da Vinci");
    filteredArtworks = controller->filterArtworks(artistFilter);
    QCOMPARE(filteredArtworks.size(), 1);
    QCOMPARE(filteredArtworks[0].getTitle(), testTitle2);
    
    // Test composite filter (OR)
    auto compositeFilter = std::make_shared<CompositeFilter>(CompositeFilter::OR);
    compositeFilter->addFilter(std::make_shared<TitleFilter>("Mona"));
    compositeFilter->addFilter(std::make_shared<ArtistFilter>("Gogh"));
    filteredArtworks = controller->filterArtworks(compositeFilter);
    QCOMPARE(filteredArtworks.size(), 2); // Both artworks should match
    
    // Test composite filter (AND)
    auto andFilter = std::make_shared<CompositeFilter>(CompositeFilter::AND);
    andFilter->addFilter(std::make_shared<TitleFilter>("Mona"));
    andFilter->addFilter(std::make_shared<ArtistFilter>("da Vinci"));
    filteredArtworks = controller->filterArtworks(andFilter);
    QCOMPARE(filteredArtworks.size(), 1); // Only Mona Lisa should match
}

void ControllerTest::testComplexUndoRedo() {
    // Add an artwork
    Artwork artwork = createArtwork1();
    controller->addArtwork(artwork);
    
    // Get the artwork with the generated ID
    QVector<Artwork> artworks = controller->getAllArtworks();
    QString artworkId = artworks[0].getId();
    
    // Update the artwork
    Artwork toUpdate = artworks[0];
    toUpdate.setTitle("Updated Title");
    controller->updateArtwork(toUpdate);
    
    // Remove the artwork
    controller->removeArtwork(artworkId);
    
    // Repository should be empty
    QCOMPARE(controller->getAllArtworks().size(), 0);
    
    // Undo the remove
    controller->undo();
    QCOMPARE(controller->getAllArtworks().size(), 1);
    QCOMPARE(controller->getArtworkById(artworkId).getTitle(), "Updated Title");
    
    // Undo the update
    controller->undo();
    QCOMPARE(controller->getAllArtworks().size(), 1);
    QCOMPARE(controller->getArtworkById(artworkId).getTitle(), testTitle1);
    
    // Undo the add
    controller->undo();
    QCOMPARE(controller->getAllArtworks().size(), 0);
    
    // Redo everything
    controller->redo(); // Redo add
    controller->redo(); // Redo update
    controller->redo(); // Redo remove
    
    // Repository should be empty again
    QCOMPARE(controller->getAllArtworks().size(), 0);
}