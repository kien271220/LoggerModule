#include "LoggerManager.h"
#include <iostream>

#define LOG_MGR                 (LoggerManager::getInstance())
#define SYS_SLEEP(MS)           (std::this_thread::sleep_for(std::chrono::milliseconds(MS)))

int main() {
    int ret = -1;
    log_mgr_configs_t cfg = log_mgr_configs_t();
    cfg.data_count = 10;
    cfg.data_size = 100;
    cfg.task_cfg.use_thread = true;
    cfg.task_cfg.event_count = 3;
    cfg.task_cfg.event_size = 64;
    cfg.log_cfgs.level = LOG_LEVEL_VERBOSE;
    cfg.log_cfgs.configs[0].type = LOG_TYPE_CONSOLE;
    cfg.log_cfgs.configs[1].type = LOG_TYPE_FILE;
    cfg.log_cfgs.configs[1].cfgs.file.log_dir = "log/";

    do {
        ret = LOG_MGR.init(&cfg);
        std::cout << "[Main] Main thread is operating ..................................\n";

        LOG_MGR.log(LOG_LEVEL_WARN, log_type_t::LOG_TYPE_CONSOLE,
            "this is log for console %f %f\n", 21.004257657, 105.3516565);
        SYS_SLEEP(10);

        LOG_MGR.logE(log_type_t::LOG_TYPE_CONSOLE,
            "this is log for console %d %f\n", 10, 1.32f);
        SYS_SLEEP(100);

        LOG_MGR.logW(log_type_t::LOG_TYPE_CONSOLE,
            "this is log for console and file: %d %f\n", 768, 1.32f);
        SYS_SLEEP(100);

        LOG_MGR.logI(log_type_t::LOG_TYPE_CONSOLE | log_type_t::LOG_TYPE_FILE,
            "this is log for console and file: %d %f %d\n", 1500, 1.32f, 922);
        SYS_SLEEP(100);

        LOG_MGR.logD(log_type_t::LOG_TYPE_CONSOLE | log_type_t::LOG_TYPE_FILE,
            "this is log for console and file: %d %f %d %.4f\n", 1500, 1.32f, 922, 21.6843);
        SYS_SLEEP(100);

        LOG_MGR.logV(log_type_t::LOG_TYPE_CONSOLE | log_type_t::LOG_TYPE_FILE,
            "this is log for console and file: %d %f %d %.6f\n", 1500, 1.32f, 922, 105.682615);
        SYS_SLEEP(100);

        if (ret != 0)
        {
            break;
        }
    } while (0);
    return ret;
}
