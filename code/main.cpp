
#include "daemon.hpp"


int main()
{
    try
    {
        Daemon daemon("/etc/backupd/config.yaml");
        // daemon.backup();
        return 0; 
    }
    catch (const std::exception& e)
    {
        std::cerr << "backup-daemon failed: " << e.what() << std::endl; 
        return 1;
    }
}
