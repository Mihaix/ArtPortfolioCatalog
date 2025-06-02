#include <QtTest>
#include "repository_test.h"
#include "controller_test.h"

int main(int argc, char *argv[])
{
    int status = 0;
    
    {
        RepositoryTest repositoryTest;
        status |= QTest::qExec(&repositoryTest, argc, argv);
    }

    {
        ControllerTest controllerTest;
        status |= QTest::qExec(&controllerTest, argc, argv);
    }
    
    return status;
}
