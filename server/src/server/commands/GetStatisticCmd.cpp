// Copyright 2019 Kamil Mankowski

#include "GetStatisticCmd.h"

const buffer_ptr GetStatisticCmd::HEADER = helpers::to_buf("GETSTATISTIC");
const buffer_ptr GetStatisticCmd::SEE_STATISTIC =
    helpers::to_buf("SEESTATISTIC");

void GetStatisticCmd::pushInput(buffer_ptr input) {
  auto stats = users->getStats();
  std::sort(stats.begin(), stats.end(),
            [](auto &left, auto &right) { return left.second > right.second; });

  std::string data = "";
  for (auto &win : stats) {
    data += win.first + "\n" + std::to_string(win.second) + "\n";
  }
  current_user->addMessageToQueue(SEE_STATISTIC);
  current_user->addMessageToQueue(helpers::get_zero_width_size(data.size()));
  current_user->addMessageToQueue(data);
}
