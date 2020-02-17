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

#include "scenario/Taxes.h"

#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "ModelRun.h"
#include "acclimate.h"
#include "model/Government.h"
#include "model/Model.h"
#include "model/Region.h"
#include "settingsnode.h"

namespace acclimate {

Taxes::Taxes(const settings::SettingsNode& settings_p, settings::SettingsNode scenario_node_p, Model* model_p)
    : Scenario(settings_p, std::move(scenario_node_p), model_p) {}

void Taxes::start() {
    for (auto& region : model()->regions) {
        region->set_government(new Government(region.get()));
    }
}

bool Taxes::iterate() {
    if (model()->time() > model()->stop_time()) {
        return false;
    }

    for (const settings::SettingsNode& tax : scenario_node["taxes"].as_sequence()) {
        const Time start_tax = tax["start_tax"].as<Time>();
        const Time full_tax = tax["full_tax"].as<Time>();
        if (model()->time() >= start_tax - model()->delta_t() && model()->time() <= full_tax - model()->delta_t()) {
            const Ratio tax_ratio =
                tax["tax_ratio"].as<Ratio>() * (model()->time() + 2 * model()->delta_t() - start_tax) / (full_tax + model()->delta_t() - start_tax);
            const std::string& sector = tax["sector"].as<std::string>();
            if (tax.has("region") && tax["region"].as<std::string>() != "ALL") {
                Region* region = model()->find_region(tax["region"].as<std::string>());
                if (region == nullptr) {
                    throw log::error(this, "Could not find region '", tax["region"], "'");
                }
                region->government()->define_tax(sector, tax_ratio);
            } else {
                for (auto& region : model()->regions) {
                    region->government()->define_tax(sector, tax_ratio);
                }
            }
        }
    }

    return true;
}

}  // namespace acclimate
