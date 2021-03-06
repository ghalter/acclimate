/*
  Copyright (C) 2014-2017 Sven Willner <sven.willner@pik-potsdam.de>
                          Christian Otto <christian.otto@pik-potsdam.de>

  This file is part of Acclimate.

  Acclimate is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  Acclimate is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with Acclimate.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ACCLIMATE_H
#define ACCLIMATE_H

#include <memory>
#include "run.h"
#include "settingsnode.h"
#include "types.h"

namespace acclimate {


enum class ModelVariantType {
#ifdef VARIANT_BASIC
    BASIC,
#endif
#ifdef VARIANT_DEMAND
    DEMAND,
#endif
#ifdef VARIANT_PRICES
    PRICES,
#endif
};


class Acclimate {

  protected:
    ModelVariantType variant_m;
    std::shared_ptr<void> run_m;

  public:
    Acclimate(settings::SettingsNode settings_p);

    inline const ModelVariantType& variant() const { return variant_m; }
    int run();
};
}  // namespace acclimate

#endif
