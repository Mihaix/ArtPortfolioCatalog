#ifndef REPOSITORY_TEST_H
#define REPOSITORY_TEST_H

#include <QtTest>
#include "models/artwork.h"

class RepositoryTest : public QObject {
    Q_OBJECT

private:
    QString jsonTestFilePath;
    QString csvTestFilePath;
    
    QString testId = "test-id-1";
    QString testTitle = "Test Artwork";
    QString testArtist = "Test Artist";
    QString testMedium = "Oil on Canvas";
    QDate testDate = QDate(2023, 5, 15);
    QString testDescription = "This is a test artwork description";
    QString testImageUrl = "http://example.com/image.jpg";
    
    Artwork createTestArtwork();
    void cleanup();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testJsonRepositoryCrud();
    void testJsonRepositoryPersistence();
    void testCsvRepositoryCrud();
    void testCsvRepositoryPersistence();
    void testRepositoryEdgeCases();
    void testGenerateId();
};

#endif // REPOSITORY_TEST_H
