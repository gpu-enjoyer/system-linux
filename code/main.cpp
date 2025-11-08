
#include "daemon.hpp"

//
// todo: что именно указывают в этих файлах?
// todo: как организовать с ними взаимодействие?
//
//   /etc/systemd/system/backupd.service
//   /etc/systemd/system/backupd.timer
//

int main()
{
    Daemon D("/etc/backupd/config.yaml");
    D.backup();
    return 0;
}
