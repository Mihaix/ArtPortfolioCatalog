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
    QDir tempDir = QDir::temp();
    jsonTestFilePath = tempDir.absoluteFilePath("test_artworks.json");
    csvTestFilePath = tempDir.absoluteFilePath("test_artworks.csv");

    cleanup();
}

void RepositoryTest::cleanupTestCase() {
    cleanup();
}

void RepositoryTest::testJsonRepositoryCrud() {
    JSONRepository repo(jsonTestFilePath);

    Artwork artwork = createTestArtwork();
    repo.add(artwork);
    QCOMPARE(repo.count(), 1);

    Artwork retrieved = repo.getById(testId);
    QCOMPARE(retrieved.getId(), testId);
    QCOMPARE(retrieved.getTitle(), testTitle);
    QCOMPARE(retrieved.getArtist(), testArtist);

    Artwork updatedArtwork = artwork;
    QString updatedTitle = "Updated Title";
    updatedArtwork.setTitle(updatedTitle);
    repo.update(updatedArtwork);

    retrieved = repo.getById(testId);
    QCOMPARE(retrieved.getTitle(), updatedTitle);

    repo.remove(testId);
    QCOMPARE(repo.count(), 0);

    QVERIFY_EXCEPTION_THROWN(repo.getById(testId), std::runtime_error);
}

void RepositoryTest::testJsonRepositoryPersistence() {
    {
        JSONRepository repo(jsonTestFilePath);
        repo.add(createTestArtwork());
    }

    JSONRepository repo2(jsonTestFilePath);
    QCOMPARE(repo2.count(), 1);

    Artwork retrieved = repo2.getById(testId);
    QCOMPARE(retrieved.getTitle(), testTitle);
}

void RepositoryTest::testCsvRepositoryCrud() {
    CSVRepository repo(csvTestFilePath);

    Artwork artwork = createTestArtwork();
    repo.add(artwork);
    QCOMPARE(repo.count(), 1);

    Artwork retrieved = repo.getById(testId);
    QCOMPARE(retrieved.getId(), testId);
    QCOMPARE(retrieved.getTitle(), testTitle);
    QCOMPARE(retrieved.getArtist(), testArtist);

    Artwork updatedArtwork = artwork;
    QString updatedTitle = "Updated Title";
    updatedArtwork.setTitle(updatedTitle);
    repo.update(updatedArtwork);

    retrieved = repo.getById(testId);
    QCOMPARE(retrieved.getTitle(), updatedTitle);

    repo.remove(testId);
    QCOMPARE(repo.count(), 0);
}

void RepositoryTest::testCsvRepositoryPersistence() {
    {
        CSVRepository repo(csvTestFilePath);
        repo.add(createTestArtwork());
    }

    CSVRepository repo2(csvTestFilePath);
    QCOMPARE(repo2.count(), 1);

    Artwork retrieved = repo2.getById(testId);
    QCOMPARE(retrieved.getTitle(), testTitle);
}

void RepositoryTest::testRepositoryEdgeCases() {
    cleanup();
    
    try {
        JSONRepository repo(jsonTestFilePath);

        QString uniqueTestId = "edge-case-test-id";
        Artwork artwork(
            uniqueTestId,
            testTitle,
            testArtist,
            testMedium,
            testDate,
            testDescription,
            testImageUrl
        );

        try {
            repo.add(artwork);
        } catch (const std::exception& e) {
            QFAIL(QString("First add failed unexpectedly: %1").arg(e.what()).toUtf8().constData());
        }

        QVERIFY_EXCEPTION_THROWN(repo.add(artwork), std::runtime_error);

        Artwork nonExistentArtwork("non-existent-id", "Title", "Artist", "Medium", QDate::currentDate(), "", "");
        QVERIFY_EXCEPTION_THROWN(repo.update(nonExistentArtwork), std::runtime_error);

        QVERIFY_EXCEPTION_THROWN(repo.remove("non-existent-id"), std::runtime_error);
    } catch (const std::exception& e) {
        QFAIL(QString("Unexpected exception: %1").arg(e.what()).toUtf8().constData());
    }
}

void RepositoryTest::testGenerateId() {
    JSONRepository repo(jsonTestFilePath);
    QString id1 = repo.generateId();
    QString id2 = repo.generateId();

    QVERIFY(!id1.isEmpty());
    QVERIFY(!id2.isEmpty());
    QVERIFY(id1 != id2);
}
