#include "repository_test.h"
#include "repository/jsonrepository.h"
#include "repository/csvrepository.h"
#include <QDir>
#include <QFile>

Artwork RepositoryTest::createTestArtwork() {
    return Artwork(
        testId,
        testTitle,
        testArtist,
        testMedium,
        testDate,
        testDescription,
        testImageUrl
    );
}

void RepositoryTest::cleanup() {
    // Clean up test files after tests
    QFile jsonFile(jsonTestFilePath);
    if (jsonFile.exists()) {
        jsonFile.remove();
    }

    QFile csvFile(csvTestFilePath);
    if (csvFile.exists()) {
        csvFile.remove();
    }
}

void RepositoryTest::initTestCase() {
    // Create temporary test files
    QDir tempDir = QDir::temp();
    jsonTestFilePath = tempDir.absoluteFilePath("test_artworks.json");
    csvTestFilePath = tempDir.absoluteFilePath("test_artworks.csv");

    cleanup(); // Ensure clean state at start
}

void RepositoryTest::cleanupTestCase() {
    cleanup();
}

void RepositoryTest::testJsonRepositoryCrud() {
    JSONRepository repo(jsonTestFilePath);

    // Test adding artwork
    Artwork artwork = createTestArtwork();
    repo.add(artwork);
    QCOMPARE(repo.count(), 1);

    // Test retrieving artwork
    Artwork retrieved = repo.getById(testId);
    QCOMPARE(retrieved.getId(), testId);
    QCOMPARE(retrieved.getTitle(), testTitle);
    QCOMPARE(retrieved.getArtist(), testArtist);

    // Test updating artwork
    Artwork updatedArtwork = artwork;
    QString updatedTitle = "Updated Title";
    updatedArtwork.setTitle(updatedTitle);
    repo.update(updatedArtwork);

    retrieved = repo.getById(testId);
    QCOMPARE(retrieved.getTitle(), updatedTitle);

    // Test removing artwork
    repo.remove(testId);
    QCOMPARE(repo.count(), 0);

    // Test exception when getting non-existent artwork
    QVERIFY_EXCEPTION_THROWN(repo.getById(testId), std::runtime_error);
}

void RepositoryTest::testJsonRepositoryPersistence() {
    // Test that data persists to file
    {
        JSONRepository repo(jsonTestFilePath);
        repo.add(createTestArtwork());
        // File should be saved when repo goes out of scope
    }

    // Create a new repository instance to read from the file
    JSONRepository repo2(jsonTestFilePath);
    QCOMPARE(repo2.count(), 1);

    Artwork retrieved = repo2.getById(testId);
    QCOMPARE(retrieved.getTitle(), testTitle);
}

void RepositoryTest::testCsvRepositoryCrud() {
    CSVRepository repo(csvTestFilePath);

    // Test adding artwork
    Artwork artwork = createTestArtwork();
    repo.add(artwork);
    QCOMPARE(repo.count(), 1);

    // Test retrieving artwork
    Artwork retrieved = repo.getById(testId);
    QCOMPARE(retrieved.getId(), testId);
    QCOMPARE(retrieved.getTitle(), testTitle);
    QCOMPARE(retrieved.getArtist(), testArtist);

    // Test updating artwork
    Artwork updatedArtwork = artwork;
    QString updatedTitle = "Updated Title";
    updatedArtwork.setTitle(updatedTitle);
    repo.update(updatedArtwork);

    retrieved = repo.getById(testId);
    QCOMPARE(retrieved.getTitle(), updatedTitle);

    // Test removing artwork
    repo.remove(testId);
    QCOMPARE(repo.count(), 0);
}

void RepositoryTest::testCsvRepositoryPersistence() {
    // Test that data persists to file
    {
        CSVRepository repo(csvTestFilePath);
        repo.add(createTestArtwork());
        // File should be saved when repo goes out of scope
    }

    // Create a new repository instance to read from the file
    CSVRepository repo2(csvTestFilePath);
    QCOMPARE(repo2.count(), 1);

    Artwork retrieved = repo2.getById(testId);
    QCOMPARE(retrieved.getTitle(), testTitle);
}

void RepositoryTest::testRepositoryEdgeCases() {
    // Ensure clean state
    cleanup();
    
    try {
        JSONRepository repo(jsonTestFilePath);

        // Test adding duplicate artwork - use a DIFFERENT ID than standard test artwork
        QString uniqueTestId = "edge-case-test-id"; // Different ID
        Artwork artwork(
            uniqueTestId,
            testTitle,
            testArtist,
            testMedium,
            testDate,
            testDescription,
            testImageUrl
        );

        // First add should succeed
        try {
            repo.add(artwork);
        } catch (const std::exception& e) {
            QFAIL(QString("First add failed unexpectedly: %1").arg(e.what()).toUtf8().constData());
        }

        // Second add should throw std::runtime_error
        QVERIFY_EXCEPTION_THROWN(repo.add(artwork), std::runtime_error);

        // Test updating non-existent artwork
        Artwork nonExistentArtwork("non-existent-id", "Title", "Artist", "Medium", QDate::currentDate(), "", "");
        QVERIFY_EXCEPTION_THROWN(repo.update(nonExistentArtwork), std::runtime_error);

        // Test removing non-existent artwork
        QVERIFY_EXCEPTION_THROWN(repo.remove("non-existent-id"), std::runtime_error);
    } catch (const std::exception& e) {
        QFAIL(QString("Unexpected exception: %1").arg(e.what()).toUtf8().constData());
    }
}

void RepositoryTest::testGenerateId() {
    JSONRepository repo(jsonTestFilePath);
    QString id1 = repo.generateId();
    QString id2 = repo.generateId();

    // IDs should be non-empty and unique
    QVERIFY(!id1.isEmpty());
    QVERIFY(!id2.isEmpty());
    QVERIFY(id1 != id2);
}
