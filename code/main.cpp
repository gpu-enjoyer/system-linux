
#include "backupd.hpp"

int main()
{
    Daemon D("/etc/backupd/config.yaml");
    D.backup();
    return 0;
}
